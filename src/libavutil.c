/* libavutil: frames, errors, rationals, sample buffers, channel layouts, format names. */

#include "runtime.h"

#define zim_AVFrame_width ffmpeg_field
#define zim_AVFrame_height ffmpeg_field
#define zim_AVFrame_format ffmpeg_field
#define zim_AVFrame_data ffmpeg_field
#define zim_AVFrame_linesize ffmpeg_field
#define zim_AVFrame_nb_samples ffmpeg_field
#define zim_AVFrame_sample_rate ffmpeg_field
#define zim_AVFrame_ch_layout ffmpeg_field
#define zim_AVFrame_pts ffmpeg_field
#define zim_AVFrame_best_effort_timestamp ffmpeg_field
#define zim_AVFrame_duration ffmpeg_field
#define zim_AVFrame_time_base ffmpeg_field

#include "../stubs/libavutil_arginfo.h"

zend_class_entry *ffmpeg_ce_AVRational;
zend_class_entry *ffmpeg_ce_AVChannelLayout;
zend_class_entry *ffmpeg_ce_AVFrame;
zend_class_entry *ffmpeg_ce_AVDictionary;

FFMPEG_HANDLE_METHODS(AVFrame)
FFMPEG_HANDLE_METHODS(AVDictionary)

ZEND_METHOD(AVRational, __construct)
{
	zend_long num = 0, den = 0;

	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(num)
		Z_PARAM_LONG(den)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_long(ffmpeg_ce_AVRational, Z_OBJ_P(ZEND_THIS), "num", sizeof("num") - 1, num);
	zend_update_property_long(ffmpeg_ce_AVRational, Z_OBJ_P(ZEND_THIS), "den", sizeof("den") - 1, den);
}

ZEND_METHOD(AVChannelLayout, __construct)
{
	zend_long order = 0, nb_channels = 0, u = 0, opaque = 0;

	ZEND_PARSE_PARAMETERS_START(0, 4)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(order)
		Z_PARAM_LONG(nb_channels)
		Z_PARAM_LONG(u)
		Z_PARAM_LONG(opaque)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_property_long(ffmpeg_ce_AVChannelLayout, Z_OBJ_P(ZEND_THIS), "order", sizeof("order") - 1, order);
	zend_update_property_long(ffmpeg_ce_AVChannelLayout, Z_OBJ_P(ZEND_THIS), "nb_channels", sizeof("nb_channels") - 1, nb_channels);
	zend_update_property_long(ffmpeg_ce_AVChannelLayout, Z_OBJ_P(ZEND_THIS), "u", sizeof("u") - 1, u);
	zend_update_property_long(ffmpeg_ce_AVChannelLayout, Z_OBJ_P(ZEND_THIS), "opaque", sizeof("opaque") - 1, opaque);
}

/* libavutil/frame.h */
PHP_FUNCTION(av_frame_alloc)
{
	ZEND_PARSE_PARAMETERS_NONE();
	ffmpeg_box(return_value, av_frame_alloc(), ffmpeg_ce_AVFrame);
}

PHP_FUNCTION(av_frame_unref)
{
	zend_object *frame;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJ_OF_CLASS(frame, ffmpeg_ce_AVFrame)
	ZEND_PARSE_PARAMETERS_END();

	av_frame_unref(ffmpeg_ptr(frame));
}

PHP_FUNCTION(av_frame_free)
{
	zval *ref;
	zend_object *before = NULL;
	AVFrame *frame;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ref)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(Z_REFVAL_P(ref)) == IS_OBJECT) {
		before = Z_OBJ_P(Z_REFVAL_P(ref));
	}
	frame = ffmpeg_ptr_or_null(before);
	av_frame_free(&frame);
	ffmpeg_ref_result(ref, before, frame, ffmpeg_ce_AVFrame);
}

/* libavutil/error.h */
PHP_FUNCTION(av_strerror)
{
	zend_long errnum, size;
	zval *errbuf;
	char *buffer;
	int result;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(errnum)
		Z_PARAM_ZVAL(errbuf)
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();

	buffer = ecalloc(MAX((size_t) size, 1), 1);
	result = av_strerror((int) errnum, buffer, (size_t) size);
	ZEND_TRY_ASSIGN_REF_STRINGL(errbuf, buffer, size > 0 ? strnlen(buffer, (size_t) size) : 0);
	efree(buffer);
	RETURN_LONG(result);
}

PHP_FUNCTION(AVERROR)
{
	zend_long e;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(e)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG(AVERROR(e));
}

/* libavutil/rational.h */
PHP_FUNCTION(av_q2d)
{
	zend_object *a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJ_OF_CLASS(a, ffmpeg_ce_AVRational)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_DOUBLE(av_q2d(ffmpeg_rational_read(a)));
}

/* libavutil/samplefmt.h */
PHP_FUNCTION(av_samples_alloc)
{
	zval *audio_data, *linesize;
	zend_long nb_channels, nb_samples, sample_fmt, align;
	uint8_t **planes;
	int line = 0, result, count, i;

	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_ZVAL(audio_data)
		Z_PARAM_ZVAL(linesize)
		Z_PARAM_LONG(nb_channels)
		Z_PARAM_LONG(nb_samples)
		Z_PARAM_LONG(sample_fmt)
		Z_PARAM_LONG(align)
	ZEND_PARSE_PARAMETERS_END();

	/* FFmpeg writes one plane address per channel when planar, else one: the caller's array holds nb_channels. */
	count = nb_channels > 0 && nb_channels <= INT_MAX ? (int) nb_channels : 1;
	planes = safe_emalloc(count, sizeof(uint8_t *), 0);
	memset(planes, 0, count * sizeof(uint8_t *));
	result = av_samples_alloc(planes, &line, (int) nb_channels, (int) nb_samples, (enum AVSampleFormat) sample_fmt, (int) align);

	if (result >= 0) {
		zval list;
		int written = av_sample_fmt_is_planar((enum AVSampleFormat) sample_fmt) ? count : 1;

		array_init_size(&list, written);
		for (i = 0; i < written; i++) {
			add_next_index_long(&list, (zend_long) (uintptr_t) planes[i]);
		}
		ZEND_TRY_ASSIGN_REF_TMP(audio_data, &list);
		ZEND_TRY_ASSIGN_REF_LONG(linesize, line);
	}
	efree(planes);
	RETURN_LONG(result);
}

PHP_FUNCTION(av_get_bytes_per_sample)
{
	zend_long sample_fmt;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(sample_fmt)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG(av_get_bytes_per_sample((enum AVSampleFormat) sample_fmt));
}

PHP_FUNCTION(av_sample_fmt_is_planar)
{
	zend_long sample_fmt;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(sample_fmt)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG(av_sample_fmt_is_planar((enum AVSampleFormat) sample_fmt));
}

PHP_FUNCTION(av_get_sample_fmt_name)
{
	zend_long sample_fmt;
	const char *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(sample_fmt)
	ZEND_PARSE_PARAMETERS_END();

	name = av_get_sample_fmt_name((enum AVSampleFormat) sample_fmt);
	if (name == NULL) {
		RETURN_NULL();
	}
	RETURN_STRING(name);
}

/* libavutil/pixdesc.h */
PHP_FUNCTION(av_get_pix_fmt_name)
{
	zend_long pix_fmt;
	const char *name;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(pix_fmt)
	ZEND_PARSE_PARAMETERS_END();

	name = av_get_pix_fmt_name((enum AVPixelFormat) pix_fmt);
	if (name == NULL) {
		RETURN_NULL();
	}
	RETURN_STRING(name);
}

/* libavutil/mem.h */
PHP_FUNCTION(av_free)
{
	zend_long ptr;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(ptr)
	ZEND_PARSE_PARAMETERS_END();

	av_free((void *) (uintptr_t) ptr);
}

/* libavutil/channel_layout.h */
PHP_FUNCTION(av_channel_layout_default)
{
	zend_object *object;
	zend_long nb_channels;
	AVChannelLayout layout;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJ_OF_CLASS(object, ffmpeg_ce_AVChannelLayout)
		Z_PARAM_LONG(nb_channels)
	ZEND_PARSE_PARAMETERS_END();

	ffmpeg_layout_read(object, &layout);
	av_channel_layout_default(&layout, (int) nb_channels);
	ffmpeg_layout_write(object, &layout);
}

PHP_FUNCTION(av_channel_layout_uninit)
{
	zend_object *object;
	AVChannelLayout layout;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJ_OF_CLASS(object, ffmpeg_ce_AVChannelLayout)
	ZEND_PARSE_PARAMETERS_END();

	ffmpeg_layout_read(object, &layout);
	av_channel_layout_uninit(&layout);
	ffmpeg_layout_write(object, &layout);
}

void ffmpeg_register_libavutil(int module_number)
{
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	register_libavutil_symbols(module_number);
	ffmpeg_ce_AVRational = register_class_AVRational();
	ffmpeg_struct_setup(ffmpeg_ce_AVRational);
	ffmpeg_ce_AVChannelLayout = register_class_AVChannelLayout();
	ffmpeg_struct_setup(ffmpeg_ce_AVChannelLayout);
	ffmpeg_ce_AVFrame = register_class_AVFrame();
	ffmpeg_handle_setup(ffmpeg_ce_AVFrame);
	ffmpeg_ce_AVDictionary = register_class_AVDictionary();
	ffmpeg_handle_setup(ffmpeg_ce_AVDictionary);
}
