/* libswresample: audio resampling, sample format and channel layout conversion. */

#include "runtime.h"
#include "../stubs/libswresample_arginfo.h"

zend_class_entry *ffmpeg_ce_SwrContext;

FFMPEG_HANDLE_METHODS(SwrContext)

/* swr_convert reads one plane address per channel of its layout; a layout has at most this many (libswresample/swresample_internal.h: SWR_CH_MAX). */
#define FFMPEG_SWR_PLANES 64

PHP_FUNCTION(swr_alloc_set_opts2)
{
	zval *ps;
	zend_object *out_layout, *in_layout, *before = NULL;
	zend_long out_fmt, out_rate, in_fmt, in_rate, log_offset, log_ctx;
	AVChannelLayout out_ch_layout, in_ch_layout;
	SwrContext *context;
	int result;

	ZEND_PARSE_PARAMETERS_START(9, 9)
		Z_PARAM_ZVAL(ps)
		Z_PARAM_OBJ_OF_CLASS(out_layout, ffmpeg_ce_AVChannelLayout)
		Z_PARAM_LONG(out_fmt)
		Z_PARAM_LONG(out_rate)
		Z_PARAM_OBJ_OF_CLASS(in_layout, ffmpeg_ce_AVChannelLayout)
		Z_PARAM_LONG(in_fmt)
		Z_PARAM_LONG(in_rate)
		Z_PARAM_LONG(log_offset)
		Z_PARAM_LONG(log_ctx)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(Z_REFVAL_P(ps)) == IS_OBJECT && Z_OBJCE_P(Z_REFVAL_P(ps)) == ffmpeg_ce_SwrContext) {
		before = Z_OBJ_P(Z_REFVAL_P(ps));
	}
	context = ffmpeg_ptr_or_null(before);
	ffmpeg_layout_read(out_layout, &out_ch_layout);
	ffmpeg_layout_read(in_layout, &in_ch_layout);
	result = swr_alloc_set_opts2(&context, &out_ch_layout, (enum AVSampleFormat) out_fmt, (int) out_rate,
		&in_ch_layout, (enum AVSampleFormat) in_fmt, (int) in_rate, (int) log_offset, (void *) (uintptr_t) log_ctx);
	ffmpeg_ref_result(ps, before, context, ffmpeg_ce_SwrContext);
	RETURN_LONG(result);
}

PHP_FUNCTION(swr_init)
{
	zend_object *s;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJ_OF_CLASS(s, ffmpeg_ce_SwrContext)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG(swr_init(ffmpeg_ptr(s)));
}

PHP_FUNCTION(swr_convert)
{
	zend_object *s;
	HashTable *out_list = NULL, *in_list = NULL;
	zend_long out_count, in_count;
	int64_t *out_addresses = NULL, *in_addresses = NULL;
	uint8_t **out = NULL;
	const uint8_t **in = NULL;
	uint32_t i;
	int result;

	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_OBJ_OF_CLASS(s, ffmpeg_ce_SwrContext)
		Z_PARAM_ARRAY_HT_OR_NULL(out_list)
		Z_PARAM_LONG(out_count)
		Z_PARAM_ARRAY_HT_OR_NULL(in_list)
		Z_PARAM_LONG(in_count)
	ZEND_PARSE_PARAMETERS_END();

	if (out_list != NULL && !ffmpeg_list_longs(out_list, 2, FFMPEG_SWR_PLANES, &out_addresses, NULL)) {
		RETURN_THROWS();
	}
	if (in_list != NULL && !ffmpeg_list_longs(in_list, 4, FFMPEG_SWR_PLANES, &in_addresses, NULL)) {
		if (out_addresses) efree(out_addresses);
		RETURN_THROWS();
	}
	if (out_addresses != NULL) {
		uint32_t n = MAX(zend_hash_num_elements(out_list), FFMPEG_SWR_PLANES);

		out = ecalloc(n, sizeof(uint8_t *));
		for (i = 0; i < n; i++) out[i] = (uint8_t *) (uintptr_t) out_addresses[i];
	}
	if (in_addresses != NULL) {
		uint32_t n = MAX(zend_hash_num_elements(in_list), FFMPEG_SWR_PLANES);

		in = ecalloc(n, sizeof(uint8_t *));
		for (i = 0; i < n; i++) in[i] = (const uint8_t *) (uintptr_t) in_addresses[i];
	}

	result = swr_convert(ffmpeg_ptr(s), out, (int) out_count, in, (int) in_count);

	if (out_addresses) efree(out_addresses);
	if (in_addresses) efree(in_addresses);
	if (out) efree(out);
	if (in) efree((void *) in);
	RETURN_LONG(result);
}

PHP_FUNCTION(swr_get_delay)
{
	zend_object *s;
	zend_long base;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJ_OF_CLASS(s, ffmpeg_ce_SwrContext)
		Z_PARAM_LONG(base)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG((zend_long) swr_get_delay(ffmpeg_ptr(s), (int64_t) base));
}

PHP_FUNCTION(swr_get_out_samples)
{
	zend_object *s;
	zend_long in_samples;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJ_OF_CLASS(s, ffmpeg_ce_SwrContext)
		Z_PARAM_LONG(in_samples)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG(swr_get_out_samples(ffmpeg_ptr(s), (int) in_samples));
}

PHP_FUNCTION(swr_free)
{
	zval *ref;
	zend_object *before = NULL;
	SwrContext *context;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ref)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(Z_REFVAL_P(ref)) == IS_OBJECT && Z_OBJCE_P(Z_REFVAL_P(ref)) == ffmpeg_ce_SwrContext) {
		before = Z_OBJ_P(Z_REFVAL_P(ref));
	}
	context = ffmpeg_ptr_or_null(before);
	swr_free(&context);
	ffmpeg_ref_result(ref, before, context, ffmpeg_ce_SwrContext);
}

void ffmpeg_register_libswresample(int module_number)
{
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	ffmpeg_ce_SwrContext = register_class_SwrContext();
	ffmpeg_handle_setup(ffmpeg_ce_SwrContext);
}
