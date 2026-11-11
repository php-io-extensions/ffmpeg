#include "runtime.h"

static zend_object_handlers ffmpeg_handle_handlers;
static zend_object_handlers ffmpeg_struct_handlers;

static zend_object *ffmpeg_create_object(zend_class_entry *ce)
{
	ffmpeg_handle *intern = zend_object_alloc(sizeof(ffmpeg_handle), ce);

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);
	intern->std.handlers = &ffmpeg_handle_handlers;
	intern->ptr = NULL;
	ZVAL_UNDEF(&intern->keep);

	return &intern->std;
}

/* Dropping the PHP object does not free the native object: only the binding's free functions do. */
static void ffmpeg_free_object(zend_object *object)
{
	ffmpeg_handle *intern = ffmpeg_handle_from(object);

	if (intern->ptr != NULL) {
		zend_hash_index_del(&FFMPEG_G(boxes), (zend_ulong) (uintptr_t) intern->ptr);
		intern->ptr = NULL;
	}
	zval_ptr_dtor(&intern->keep);
	ZVAL_UNDEF(&intern->keep);
	zend_object_std_dtor(object);
}

void ffmpeg_handle_setup(zend_class_entry *ce)
{
	static bool handlers_ready = false;

	if (!handlers_ready) {
		memcpy(&ffmpeg_handle_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
		ffmpeg_handle_handlers.offset = XtOffsetOf(ffmpeg_handle, std);
		ffmpeg_handle_handlers.free_obj = ffmpeg_free_object;
		ffmpeg_handle_handlers.clone_obj = NULL;
		handlers_ready = true;
	}

	ce->create_object = ffmpeg_create_object;
	ce->default_object_handlers = &ffmpeg_handle_handlers;
}

void ffmpeg_struct_setup(zend_class_entry *ce)
{
	static bool handlers_ready = false;

	if (!handlers_ready) {
		memcpy(&ffmpeg_struct_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
		ffmpeg_struct_handlers.clone_obj = NULL;
		handlers_ready = true;
	}

	ce->default_object_handlers = &ffmpeg_struct_handlers;
}

void ffmpeg_release(zend_object *obj)
{
	ffmpeg_handle *intern = ffmpeg_handle_from(obj);
	zval keep;

	if (intern->ptr != NULL) {
		zend_hash_index_del(&FFMPEG_G(boxes), (zend_ulong) (uintptr_t) intern->ptr);
		intern->ptr = NULL;
	}

	ZVAL_COPY_VALUE(&keep, &intern->keep);
	ZVAL_UNDEF(&intern->keep);
	if (Z_TYPE(keep) == IS_ARRAY) {
		zval *zv;

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(keep), zv) {
			if (Z_TYPE_P(zv) == IS_OBJECT && Z_OBJ_P(zv)->handlers == &ffmpeg_handle_handlers) {
				ffmpeg_release(Z_OBJ_P(zv));
			}
		} ZEND_HASH_FOREACH_END();
	}
	zval_ptr_dtor(&keep);
}

void ffmpeg_box(zval *rv, void *ptr, zend_class_entry *ce)
{
	zend_object *existing;

	if (ptr == NULL) {
		ZVAL_NULL(rv);
		return;
	}

	existing = zend_hash_index_find_ptr(&FFMPEG_G(boxes), (zend_ulong) (uintptr_t) ptr);
	if (existing != NULL && existing->ce == ce) {
		ZVAL_OBJ_COPY(rv, existing);
		return;
	}
	if (existing != NULL) {
		/* Another class boxed this address (a struct's first member shares its address). Detach it so its free_obj cannot drop the new entry. */
		ffmpeg_handle_from(existing)->ptr = NULL;
	}

	object_init_ex(rv, ce);
	ffmpeg_handle_from(Z_OBJ_P(rv))->ptr = ptr;
	zend_hash_index_update_ptr(&FFMPEG_G(boxes), (zend_ulong) (uintptr_t) ptr, Z_OBJ_P(rv));
}

void ffmpeg_box_kept(zval *rv, zend_object *owner, void *ptr, zend_class_entry *ce)
{
	ffmpeg_handle *intern = ffmpeg_handle_from(owner);

	ffmpeg_box(rv, ptr, ce);
	if (Z_TYPE_P(rv) != IS_OBJECT) {
		return;
	}
	if (Z_TYPE(intern->keep) != IS_ARRAY) {
		array_init(&intern->keep);
	}
	if (!zend_hash_index_exists(Z_ARRVAL(intern->keep), Z_OBJ_HANDLE_P(rv))) {
		Z_ADDREF_P(rv);
		zend_hash_index_add_new(Z_ARRVAL(intern->keep), Z_OBJ_HANDLE_P(rv), rv);
	}
}

void ffmpeg_ref_result(zval *ref, zend_object *before, void *after, zend_class_entry *ce)
{
	zval result;

	if (before != NULL && ffmpeg_ptr(before) != after) {
		ffmpeg_release(before);
	}
	ffmpeg_box(&result, after, ce);
	ZEND_TRY_ASSIGN_REF_TMP(ref, &result);
}

bool ffmpeg_list_longs(HashTable *list, uint32_t arg_num, uint32_t minimum, int64_t **out, uint32_t *count)
{
	uint32_t length = zend_hash_num_elements(list);
	uint32_t size = MAX(length, minimum);
	uint32_t i = 0;
	zval *zv;

	*out = ecalloc(MAX(size, 1), sizeof(int64_t));
	ZEND_HASH_FOREACH_VAL(list, zv) {
		ZVAL_DEREF(zv);
		if (Z_TYPE_P(zv) != IS_LONG) {
			zend_argument_type_error(arg_num, "must be a list of int, %s given at index %u", zend_zval_value_name(zv), i);
			efree(*out);
			*out = NULL;
			return false;
		}
		(*out)[i++] = (int64_t) Z_LVAL_P(zv);
	} ZEND_HASH_FOREACH_END();
	if (count != NULL) {
		*count = length;
	}

	return true;
}

bool ffmpeg_list_doubles(HashTable *list, uint32_t arg_num, uint32_t minimum, double **out)
{
	uint32_t size = MAX(zend_hash_num_elements(list), minimum);
	uint32_t i = 0;
	zval *zv;

	*out = ecalloc(MAX(size, 1), sizeof(double));
	ZEND_HASH_FOREACH_VAL(list, zv) {
		ZVAL_DEREF(zv);
		if (Z_TYPE_P(zv) == IS_DOUBLE) {
			(*out)[i++] = Z_DVAL_P(zv);
		} else if (Z_TYPE_P(zv) == IS_LONG) {
			(*out)[i++] = (double) Z_LVAL_P(zv);
		} else {
			zend_argument_type_error(arg_num, "must be a list of float, %s given at index %u", zend_zval_value_name(zv), i);
			efree(*out);
			*out = NULL;
			return false;
		}
	} ZEND_HASH_FOREACH_END();

	return true;
}

bool ffmpeg_list_dictionaries(HashTable *list, uint32_t arg_num, uint32_t minimum, AVDictionary ***out)
{
	uint32_t size = MAX(zend_hash_num_elements(list), minimum);
	uint32_t i = 0;
	zval *zv;

	*out = ecalloc(MAX(size, 1), sizeof(AVDictionary *));
	ZEND_HASH_FOREACH_VAL(list, zv) {
		ZVAL_DEREF(zv);
		if (Z_TYPE_P(zv) == IS_NULL) {
			i++;
		} else if (Z_TYPE_P(zv) == IS_OBJECT && Z_OBJCE_P(zv) == ffmpeg_ce_AVDictionary) {
			(*out)[i++] = ffmpeg_ptr(Z_OBJ_P(zv));
		} else {
			zend_argument_type_error(arg_num, "must be a list of ?AVDictionary, %s given at index %u", zend_zval_value_name(zv), i);
			efree(*out);
			*out = NULL;
			return false;
		}
	} ZEND_HASH_FOREACH_END();

	return true;
}

/* The value classes' properties are typed int, declared in this order; an unset one reads as the zero C's {0} gives. */
static zend_always_inline zend_long ffmpeg_prop(zend_object *obj, uint32_t slot)
{
	zval *zv = OBJ_PROP_NUM(obj, slot);

	return Z_TYPE_P(zv) == IS_LONG ? Z_LVAL_P(zv) : 0;
}

static zend_always_inline void ffmpeg_prop_set(zend_object *obj, uint32_t slot, zend_long value)
{
	zval *zv = OBJ_PROP_NUM(obj, slot);

	zval_ptr_dtor(zv);
	ZVAL_LONG(zv, value);
}

AVRational ffmpeg_rational_read(zend_object *obj)
{
	return av_make_q((int) ffmpeg_prop(obj, 0), (int) ffmpeg_prop(obj, 1));
}

void ffmpeg_rational_new(zval *rv, AVRational value)
{
	object_init_ex(rv, ffmpeg_ce_AVRational);
	ffmpeg_prop_set(Z_OBJ_P(rv), 0, value.num);
	ffmpeg_prop_set(Z_OBJ_P(rv), 1, value.den);
}

void ffmpeg_layout_read(zend_object *obj, AVChannelLayout *layout)
{
	memset(layout, 0, sizeof(*layout));
	layout->order = (enum AVChannelOrder) ffmpeg_prop(obj, 0);
	layout->nb_channels = (int) ffmpeg_prop(obj, 1);
	if (layout->order == AV_CHANNEL_ORDER_CUSTOM) {
		layout->u.map = (AVChannelCustom *) (uintptr_t) ffmpeg_prop(obj, 2);
	} else {
		layout->u.mask = (uint64_t) ffmpeg_prop(obj, 2);
	}
	layout->opaque = (void *) (uintptr_t) ffmpeg_prop(obj, 3);
}

void ffmpeg_layout_write(zend_object *obj, const AVChannelLayout *layout)
{
	ffmpeg_prop_set(obj, 0, (zend_long) layout->order);
	ffmpeg_prop_set(obj, 1, (zend_long) layout->nb_channels);
	ffmpeg_prop_set(obj, 2, layout->order == AV_CHANNEL_ORDER_CUSTOM ? (zend_long) (uintptr_t) layout->u.map : (zend_long) layout->u.mask);
	ffmpeg_prop_set(obj, 3, (zend_long) (uintptr_t) layout->opaque);
}

void ffmpeg_layout_new(zval *rv, const AVChannelLayout *layout)
{
	object_init_ex(rv, ffmpeg_ce_AVChannelLayout);
	ffmpeg_layout_write(Z_OBJ_P(rv), layout);
}

#ifdef FFMPEG_PROBES
bool ffmpeg_probe_off(const char *id)
{
	const char *off = getenv("FFMPEG_PROBE_OFF");

	return off != NULL && strcmp(off, id) == 0;
}
#endif
