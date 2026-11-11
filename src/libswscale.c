/* libswscale: image scaling and pixel format conversion. */

#include "runtime.h"
#include "../stubs/libswscale_arginfo.h"

zend_class_entry *ffmpeg_ce_SwsContext;
zend_class_entry *ffmpeg_ce_SwsFilter;

FFMPEG_HANDLE_METHODS(SwsContext)
FFMPEG_HANDLE_METHODS(SwsFilter)

/* sws_scale reads four plane addresses and four strides from each array (swscale.h: "the array containing the pointers to the planes"). */
#define FFMPEG_SWS_PLANES 4
/* sws_getContext reads two parameters when param is not NULL. */
#define FFMPEG_SWS_PARAMS 2

PHP_FUNCTION(sws_getContext)
{
	zend_long src_w, src_h, src_format, dst_w, dst_h, dst_format, flags;
	zend_object *src_filter = NULL, *dst_filter = NULL;
	HashTable *list = NULL;
	double *param = NULL;
	struct SwsContext *context;  /* FFmpeg 7.1 declares no SwsContext typedef */

	ZEND_PARSE_PARAMETERS_START(10, 10)
		Z_PARAM_LONG(src_w)
		Z_PARAM_LONG(src_h)
		Z_PARAM_LONG(src_format)
		Z_PARAM_LONG(dst_w)
		Z_PARAM_LONG(dst_h)
		Z_PARAM_LONG(dst_format)
		Z_PARAM_LONG(flags)
		Z_PARAM_OBJ_OF_CLASS_OR_NULL(src_filter, ffmpeg_ce_SwsFilter)
		Z_PARAM_OBJ_OF_CLASS_OR_NULL(dst_filter, ffmpeg_ce_SwsFilter)
		Z_PARAM_ARRAY_HT_OR_NULL(list)
	ZEND_PARSE_PARAMETERS_END();

	if (list != NULL && !ffmpeg_list_doubles(list, 10, FFMPEG_SWS_PARAMS, &param)) {
		RETURN_THROWS();
	}
	context = sws_getContext((int) src_w, (int) src_h, (enum AVPixelFormat) src_format, (int) dst_w, (int) dst_h, (enum AVPixelFormat) dst_format,
		(int) flags, ffmpeg_ptr_or_null(src_filter), ffmpeg_ptr_or_null(dst_filter), param);
	if (param != NULL) {
		efree(param);
	}
	ffmpeg_box(return_value, context, ffmpeg_ce_SwsContext);
}

PHP_FUNCTION(sws_scale)
{
	zend_object *c;
	HashTable *src_slice, *src_stride, *dst, *dst_stride;
	zend_long src_slice_y, src_slice_h;
	int64_t *src_addresses = NULL, *src_strides = NULL, *dst_addresses = NULL, *dst_strides = NULL;
	const uint8_t **src_planes;
	uint8_t **dst_planes;
	int *src_lines, *dst_lines;
	uint32_t src_count, src_stride_count, dst_count, dst_stride_count, i;
	int result;

	ZEND_PARSE_PARAMETERS_START(7, 7)
		Z_PARAM_OBJ_OF_CLASS(c, ffmpeg_ce_SwsContext)
		Z_PARAM_ARRAY_HT(src_slice)
		Z_PARAM_ARRAY_HT(src_stride)
		Z_PARAM_LONG(src_slice_y)
		Z_PARAM_LONG(src_slice_h)
		Z_PARAM_ARRAY_HT(dst)
		Z_PARAM_ARRAY_HT(dst_stride)
	ZEND_PARSE_PARAMETERS_END();

	if (!ffmpeg_list_longs(src_slice, 2, FFMPEG_SWS_PLANES, &src_addresses, &src_count)
		|| !ffmpeg_list_longs(src_stride, 3, FFMPEG_SWS_PLANES, &src_strides, &src_stride_count)
		|| !ffmpeg_list_longs(dst, 6, FFMPEG_SWS_PLANES, &dst_addresses, &dst_count)
		|| !ffmpeg_list_longs(dst_stride, 7, FFMPEG_SWS_PLANES, &dst_strides, &dst_stride_count)) {
		if (src_addresses) efree(src_addresses);
		if (src_strides) efree(src_strides);
		if (dst_addresses) efree(dst_addresses);
		RETURN_THROWS();
	}

	src_count = MAX(src_count, FFMPEG_SWS_PLANES);
	src_stride_count = MAX(src_stride_count, FFMPEG_SWS_PLANES);
	dst_count = MAX(dst_count, FFMPEG_SWS_PLANES);
	dst_stride_count = MAX(dst_stride_count, FFMPEG_SWS_PLANES);
	src_planes = ecalloc(src_count, sizeof(uint8_t *));
	src_lines = ecalloc(src_stride_count, sizeof(int));
	dst_planes = ecalloc(dst_count, sizeof(uint8_t *));
	dst_lines = ecalloc(dst_stride_count, sizeof(int));
	for (i = 0; i < src_count; i++) src_planes[i] = (const uint8_t *) (uintptr_t) src_addresses[i];
	for (i = 0; i < src_stride_count; i++) src_lines[i] = (int) src_strides[i];
	for (i = 0; i < dst_count; i++) dst_planes[i] = (uint8_t *) (uintptr_t) dst_addresses[i];
	for (i = 0; i < dst_stride_count; i++) dst_lines[i] = (int) dst_strides[i];

	result = sws_scale(ffmpeg_ptr(c), src_planes, src_lines, (int) src_slice_y, (int) src_slice_h, dst_planes, dst_lines);

	efree(src_addresses);
	efree(src_strides);
	efree(dst_addresses);
	efree(dst_strides);
	efree(src_planes);
	efree(src_lines);
	efree(dst_planes);
	efree(dst_lines);
	RETURN_LONG(result);
}

PHP_FUNCTION(sws_freeContext)
{
	zend_object *context = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJ_OF_CLASS_OR_NULL(context, ffmpeg_ce_SwsContext)
	ZEND_PARSE_PARAMETERS_END();

	sws_freeContext(ffmpeg_ptr_or_null(context));
	if (context != NULL) {
		ffmpeg_release(context);
	}
}

void ffmpeg_register_libswscale(int module_number)
{
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	register_libswscale_symbols(module_number);
	ffmpeg_ce_SwsContext = register_class_SwsContext();
	ffmpeg_handle_setup(ffmpeg_ce_SwsContext);
	ffmpeg_ce_SwsFilter = register_class_SwsFilter();
	ffmpeg_handle_setup(ffmpeg_ce_SwsFilter);
}
