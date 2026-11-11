/* libavcodec: decoders, their contexts and parameters, packets. */

#include "runtime.h"

#define zim_AVCodec_name ffmpeg_field
#define zim_AVCodec_long_name ffmpeg_field
#define zim_AVCodec_type ffmpeg_field
#define zim_AVCodec_id ffmpeg_field
#define zim_AVCodecContext_codec_type ffmpeg_field
#define zim_AVCodecContext_codec_id ffmpeg_field
#define zim_AVCodecContext_width ffmpeg_field
#define zim_AVCodecContext_height ffmpeg_field
#define zim_AVCodecContext_pix_fmt ffmpeg_field
#define zim_AVCodecContext_sample_fmt ffmpeg_field
#define zim_AVCodecContext_sample_rate ffmpeg_field
#define zim_AVCodecContext_ch_layout ffmpeg_field
#define zim_AVCodecContext_time_base ffmpeg_field
#define zim_AVCodecParameters_codec_type ffmpeg_field
#define zim_AVCodecParameters_codec_id ffmpeg_field
#define zim_AVCodecParameters_format ffmpeg_field
#define zim_AVCodecParameters_width ffmpeg_field
#define zim_AVCodecParameters_height ffmpeg_field
#define zim_AVCodecParameters_sample_rate ffmpeg_field
#define zim_AVCodecParameters_ch_layout ffmpeg_field
#define zim_AVPacket_stream_index ffmpeg_field
#define zim_AVPacket_pts ffmpeg_field
#define zim_AVPacket_dts ffmpeg_field
#define zim_AVPacket_duration ffmpeg_field
#define zim_AVPacket_size ffmpeg_field
#define zim_AVPacket_flags ffmpeg_field

#include "../stubs/libavcodec_arginfo.h"

zend_class_entry *ffmpeg_ce_AVCodec;
zend_class_entry *ffmpeg_ce_AVCodecContext;
zend_class_entry *ffmpeg_ce_AVCodecParameters;
zend_class_entry *ffmpeg_ce_AVPacket;

FFMPEG_HANDLE_METHODS(AVCodec)
FFMPEG_HANDLE_METHODS(AVCodecContext)
FFMPEG_HANDLE_METHODS(AVCodecParameters)
FFMPEG_HANDLE_METHODS(AVPacket)

/* libavcodec/codec.h */
PHP_FUNCTION(avcodec_find_decoder)
{
	zend_long id;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(id)
	ZEND_PARSE_PARAMETERS_END();

	ffmpeg_box(return_value, (void *) avcodec_find_decoder((enum AVCodecID) id), ffmpeg_ce_AVCodec);
}

/* libavcodec/codec_id.h */
PHP_FUNCTION(avcodec_get_name)
{
	zend_long id;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(id)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_STRING(avcodec_get_name((enum AVCodecID) id));
}

/* libavcodec/avcodec.h */
PHP_FUNCTION(avcodec_alloc_context3)
{
	zend_object *codec = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJ_OF_CLASS_OR_NULL(codec, ffmpeg_ce_AVCodec)
	ZEND_PARSE_PARAMETERS_END();

	ffmpeg_box(return_value, avcodec_alloc_context3(ffmpeg_ptr_or_null(codec)), ffmpeg_ce_AVCodecContext);
}

PHP_FUNCTION(avcodec_parameters_to_context)
{
	zend_object *codec, *par;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJ_OF_CLASS(codec, ffmpeg_ce_AVCodecContext)
		Z_PARAM_OBJ_OF_CLASS(par, ffmpeg_ce_AVCodecParameters)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG(avcodec_parameters_to_context(ffmpeg_ptr(codec), ffmpeg_ptr(par)));
}

PHP_FUNCTION(avcodec_open2)
{
	zend_object *avctx, *codec = NULL, *before = NULL;
	zval *options;
	AVDictionary *dictionary;
	int result;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_OBJ_OF_CLASS(avctx, ffmpeg_ce_AVCodecContext)
		Z_PARAM_OBJ_OF_CLASS_OR_NULL(codec, ffmpeg_ce_AVCodec)
		Z_PARAM_ZVAL(options)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(Z_REFVAL_P(options)) == IS_OBJECT && Z_OBJCE_P(Z_REFVAL_P(options)) == ffmpeg_ce_AVDictionary) {
		before = Z_OBJ_P(Z_REFVAL_P(options));
	}
	dictionary = ffmpeg_ptr_or_null(before);
	result = avcodec_open2(ffmpeg_ptr(avctx), ffmpeg_ptr_or_null(codec), &dictionary);
	ffmpeg_ref_result(options, before, dictionary, ffmpeg_ce_AVDictionary);
	RETURN_LONG(result);
}

PHP_FUNCTION(avcodec_send_packet)
{
	zend_object *avctx, *avpkt = NULL;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJ_OF_CLASS(avctx, ffmpeg_ce_AVCodecContext)
		Z_PARAM_OBJ_OF_CLASS_OR_NULL(avpkt, ffmpeg_ce_AVPacket)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG(avcodec_send_packet(ffmpeg_ptr(avctx), ffmpeg_ptr_or_null(avpkt)));
}

PHP_FUNCTION(avcodec_receive_frame)
{
	zend_object *avctx, *frame;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_OBJ_OF_CLASS(avctx, ffmpeg_ce_AVCodecContext)
		Z_PARAM_OBJ_OF_CLASS(frame, ffmpeg_ce_AVFrame)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_LONG(avcodec_receive_frame(ffmpeg_ptr(avctx), ffmpeg_ptr(frame)));
}

PHP_FUNCTION(avcodec_flush_buffers)
{
	zend_object *avctx;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJ_OF_CLASS(avctx, ffmpeg_ce_AVCodecContext)
	ZEND_PARSE_PARAMETERS_END();

	avcodec_flush_buffers(ffmpeg_ptr(avctx));
}

PHP_FUNCTION(avcodec_free_context)
{
	zval *ref;
	zend_object *before = NULL;
	AVCodecContext *avctx;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ref)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(Z_REFVAL_P(ref)) == IS_OBJECT) {
		before = Z_OBJ_P(Z_REFVAL_P(ref));
	}
	avctx = ffmpeg_ptr_or_null(before);
	avcodec_free_context(&avctx);
	ffmpeg_ref_result(ref, before, avctx, ffmpeg_ce_AVCodecContext);
}

/* libavcodec/packet.h */
PHP_FUNCTION(av_packet_alloc)
{
	ZEND_PARSE_PARAMETERS_NONE();
	ffmpeg_box(return_value, av_packet_alloc(), ffmpeg_ce_AVPacket);
}

PHP_FUNCTION(av_packet_unref)
{
	zend_object *pkt;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJ_OF_CLASS(pkt, ffmpeg_ce_AVPacket)
	ZEND_PARSE_PARAMETERS_END();

	av_packet_unref(ffmpeg_ptr(pkt));
}

PHP_FUNCTION(av_packet_free)
{
	zval *ref;
	zend_object *before = NULL;
	AVPacket *pkt;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ref)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(Z_REFVAL_P(ref)) == IS_OBJECT) {
		before = Z_OBJ_P(Z_REFVAL_P(ref));
	}
	pkt = ffmpeg_ptr_or_null(before);
	av_packet_free(&pkt);
	ffmpeg_ref_result(ref, before, pkt, ffmpeg_ce_AVPacket);
}

void ffmpeg_register_libavcodec(int module_number)
{
	zend_register_functions(NULL, ext_functions, NULL, MODULE_PERSISTENT);
	ffmpeg_ce_AVCodec = register_class_AVCodec();
	ffmpeg_handle_setup(ffmpeg_ce_AVCodec);
	ffmpeg_ce_AVCodecContext = register_class_AVCodecContext();
	ffmpeg_handle_setup(ffmpeg_ce_AVCodecContext);
	ffmpeg_ce_AVCodecParameters = register_class_AVCodecParameters();
	ffmpeg_handle_setup(ffmpeg_ce_AVCodecParameters);
	ffmpeg_ce_AVPacket = register_class_AVPacket();
	ffmpeg_handle_setup(ffmpeg_ce_AVPacket);
}
