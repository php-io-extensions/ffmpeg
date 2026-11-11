/* libavformat: demuxing. */

#include "runtime.h"

#define zim_AVInputFormat_name ffmpeg_field
#define zim_AVInputFormat_long_name ffmpeg_field
#define zim_AVFormatContext_nb_streams ffmpeg_field
#define zim_AVFormatContext_streams ffmpeg_field
#define zim_AVFormatContext_iformat ffmpeg_field
#define zim_AVFormatContext_duration ffmpeg_field
#define zim_AVFormatContext_start_time ffmpeg_field
#define zim_AVFormatContext_bit_rate ffmpeg_field
#define zim_AVStream_index ffmpeg_field
#define zim_AVStream_id ffmpeg_field
#define zim_AVStream_time_base ffmpeg_field
#define zim_AVStream_start_time ffmpeg_field
#define zim_AVStream_duration ffmpeg_field
#define zim_AVStream_nb_frames ffmpeg_field
#define zim_AVStream_avg_frame_rate ffmpeg_field
#define zim_AVStream_codecpar ffmpeg_field

#include "../stubs/libavformat_arginfo.h"

zend_class_entry *ffmpeg_ce_AVInputFormat;
zend_class_entry *ffmpeg_ce_AVFormatContext;
zend_class_entry *ffmpeg_ce_AVStream;

FFMPEG_HANDLE_METHODS(AVInputFormat)
FFMPEG_HANDLE_METHODS(AVFormatContext)
FFMPEG_HANDLE_METHODS(AVStream)

/* The handle an in-and-out reference holds, if it holds one of class ce. */
static zend_object *ffmpeg_ref_handle(zval *ref, zend_class_entry *ce)
{
	zval *value = Z_REFVAL_P(ref);

	return Z_TYPE_P(value) == IS_OBJECT && Z_OBJCE_P(value) == ce ? Z_OBJ_P(value) : NULL;
}

PHP_FUNCTION(avformat_open_input)
{
	zval *ps, *options;
	zend_string *url;
	zend_object *fmt = NULL, *context_before, *options_before;
	AVFormatContext *context;
	AVDictionary *dictionary;
	int result;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_ZVAL(ps)
		Z_PARAM_PATH_STR(url)
		Z_PARAM_OBJ_OF_CLASS_OR_NULL(fmt, ffmpeg_ce_AVInputFormat)
		Z_PARAM_ZVAL(options)
	ZEND_PARSE_PARAMETERS_END();

	context_before = ffmpeg_ref_handle(ps, ffmpeg_ce_AVFormatContext);
	options_before = ffmpeg_ref_handle(options, ffmpeg_ce_AVDictionary);
	context = ffmpeg_ptr_or_null(context_before);
	dictionary = ffmpeg_ptr_or_null(options_before);
	/* On failure FFmpeg frees a context the caller allocated and sets *ps to NULL. */
	result = avformat_open_input(&context, ZSTR_VAL(url), ffmpeg_ptr_or_null(fmt), &dictionary);
	ffmpeg_ref_result(ps, context_before, context, ffmpeg_ce_AVFormatContext);
	ffmpeg_ref_result(options, options_before, dictionary, ffmpeg_ce_AVDictionary);
	RETURN_LONG(result);
}

PHP_FUNCTION(avformat_find_stream_info)
{
	zend_object *ic;
	HashTable *list = NULL;
	AVFormatContext *context;
	AVDictionary **options = NULL;
	int result;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJ_OF_CLASS(ic, ffmpeg_ce_AVFormatContext)
		Z_PARAM_ARRAY_HT_OR_NULL(list)
	ZEND_PARSE_PARAMETERS_END();

	context = ffmpeg_ptr(ic);
	/* FFmpeg reads one entry per stream: shorter lists end in NULL entries. */
	if (list != NULL && !ffmpeg_list_dictionaries(list, 2, context != NULL ? context->nb_streams : 0, &options)) {
		RETURN_THROWS();
	}
	result = avformat_find_stream_info(context, options);
	if (options != NULL) {
		efree(options);
	}
	RETURN_LONG(result);
}

PHP_FUNCTION(av_find_best_stream)
{
	zend_object *ic;
	zend_long type, wanted, related, flags;
	zval *decoder_ret;
	const AVCodec *decoder = NULL;
	int result;
	zval boxed;

	ZEND_PARSE_PARAMETERS_START(6, 6)
		Z_PARAM_OBJ_OF_CLASS(ic, ffmpeg_ce_AVFormatContext)
		Z_PARAM_LONG(type)
		Z_PARAM_LONG(wanted)
		Z_PARAM_LONG(related)
		Z_PARAM_ZVAL(decoder_ret)
		Z_PARAM_LONG(flags)
	ZEND_PARSE_PARAMETERS_END();

	result = av_find_best_stream(ffmpeg_ptr(ic), (enum AVMediaType) type, (int) wanted, (int) related, &decoder, (int) flags);
	ffmpeg_box(&boxed, (void *) decoder, ffmpeg_ce_AVCodec);
	ZEND_TRY_ASSIGN_REF_TMP(decoder_ret, &boxed);
	RETURN_LONG(result);
}

PHP_FUNCTION(av_read_frame)
{
	zend_object *s, *pkt;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJ_OF_CLASS(s, ffmpeg_ce_AVFormatContext)
		Z_PARAM_OBJ_OF_CLASS(pkt, ffmpeg_ce_AVPacket)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG(av_read_frame(ffmpeg_ptr(s), ffmpeg_ptr(pkt)));
}

PHP_FUNCTION(av_seek_frame)
{
	zend_object *s;
	zend_long stream_index, timestamp, flags;

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_OBJ_OF_CLASS(s, ffmpeg_ce_AVFormatContext)
		Z_PARAM_LONG(stream_index)
		Z_PARAM_LONG(timestamp)
		Z_PARAM_LONG(flags)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG(av_seek_frame(ffmpeg_ptr(s), (int) stream_index, (int64_t) timestamp, (int) flags));
}

PHP_FUNCTION(avformat_close_input)
{
	zval *ref;
	zend_object *before;
	AVFormatContext *context;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ref)
	ZEND_PARSE_PARAMETERS_END();

	before = ffmpeg_ref_handle(ref, ffmpeg_ce_AVFormatContext);
	context = ffmpeg_ptr_or_null(before);
	avformat_close_input(&context);
	ffmpeg_ref_result(ref, before, context, ffmpeg_ce_AVFormatContext);
}

void ffmpeg_register_libavformat(int module_number)
{
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	register_libavformat_symbols(module_number);
	ffmpeg_ce_AVInputFormat = register_class_AVInputFormat();
	ffmpeg_handle_setup(ffmpeg_ce_AVInputFormat);
	ffmpeg_ce_AVFormatContext = register_class_AVFormatContext();
	ffmpeg_handle_setup(ffmpeg_ce_AVFormatContext);
	ffmpeg_ce_AVStream = register_class_AVStream();
	ffmpeg_handle_setup(ffmpeg_ce_AVStream);
}
