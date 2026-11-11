/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 4d26223acc7153e268ffe5352e309d5dea8f9ccb */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_avcodec_find_decoder, 0, 1, AVCodec, 1)
	ZEND_ARG_TYPE_INFO(0, id, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_avcodec_get_name, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, id, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_avcodec_alloc_context3, 0, 1, AVCodecContext, 1)
	ZEND_ARG_OBJ_INFO(0, codec, AVCodec, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_avcodec_parameters_to_context, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, codec, AVCodecContext, 0)
	ZEND_ARG_OBJ_INFO(0, par, AVCodecParameters, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_avcodec_open2, 0, 3, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, avctx, AVCodecContext, 0)
	ZEND_ARG_OBJ_INFO(0, codec, AVCodec, 1)
	ZEND_ARG_OBJ_INFO(1, options, AVDictionary, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_avcodec_send_packet, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, avctx, AVCodecContext, 0)
	ZEND_ARG_OBJ_INFO(0, avpkt, AVPacket, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_avcodec_receive_frame, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, avctx, AVCodecContext, 0)
	ZEND_ARG_OBJ_INFO(0, frame, AVFrame, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_avcodec_flush_buffers, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, avctx, AVCodecContext, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_avcodec_free_context, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(1, avctx, AVCodecContext, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_av_packet_alloc, 0, 0, AVPacket, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_av_packet_unref, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, pkt, AVPacket, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_av_packet_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(1, pkt, AVPacket, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_AVCodec___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AVCodec_pointer, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AVCodec_fromPointer, 0, 1, IS_STATIC, 1)
	ZEND_ARG_TYPE_INFO(0, pointer, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AVCodec_name, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AVCodec_long_name, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_AVCodec_type arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodec_id arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecContext___construct arginfo_class_AVCodec___construct

#define arginfo_class_AVCodecContext_pointer arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecContext_fromPointer arginfo_class_AVCodec_fromPointer

#define arginfo_class_AVCodecContext_codec_type arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecContext_codec_id arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecContext_width arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecContext_height arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecContext_pix_fmt arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecContext_sample_fmt arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecContext_sample_rate arginfo_class_AVCodec_pointer

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_AVCodecContext_ch_layout, 0, 0, AVChannelLayout, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_AVCodecContext_time_base, 0, 0, AVRational, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_AVCodecParameters___construct arginfo_class_AVCodec___construct

#define arginfo_class_AVCodecParameters_pointer arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecParameters_fromPointer arginfo_class_AVCodec_fromPointer

#define arginfo_class_AVCodecParameters_codec_type arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecParameters_codec_id arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecParameters_format arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecParameters_width arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecParameters_height arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecParameters_sample_rate arginfo_class_AVCodec_pointer

#define arginfo_class_AVCodecParameters_ch_layout arginfo_class_AVCodecContext_ch_layout

#define arginfo_class_AVPacket___construct arginfo_class_AVCodec___construct

#define arginfo_class_AVPacket_pointer arginfo_class_AVCodec_pointer

#define arginfo_class_AVPacket_fromPointer arginfo_class_AVCodec_fromPointer

#define arginfo_class_AVPacket_stream_index arginfo_class_AVCodec_pointer

#define arginfo_class_AVPacket_pts arginfo_class_AVCodec_pointer

#define arginfo_class_AVPacket_dts arginfo_class_AVCodec_pointer

#define arginfo_class_AVPacket_duration arginfo_class_AVCodec_pointer

#define arginfo_class_AVPacket_size arginfo_class_AVCodec_pointer

#define arginfo_class_AVPacket_flags arginfo_class_AVCodec_pointer

ZEND_FUNCTION(avcodec_find_decoder);
ZEND_FUNCTION(avcodec_get_name);
ZEND_FUNCTION(avcodec_alloc_context3);
ZEND_FUNCTION(avcodec_parameters_to_context);
ZEND_FUNCTION(avcodec_open2);
ZEND_FUNCTION(avcodec_send_packet);
ZEND_FUNCTION(avcodec_receive_frame);
ZEND_FUNCTION(avcodec_flush_buffers);
ZEND_FUNCTION(avcodec_free_context);
ZEND_FUNCTION(av_packet_alloc);
ZEND_FUNCTION(av_packet_unref);
ZEND_FUNCTION(av_packet_free);
ZEND_METHOD(AVCodec, __construct);
ZEND_METHOD(AVCodec, pointer);
ZEND_METHOD(AVCodec, fromPointer);
ZEND_METHOD(AVCodec, name);
ZEND_METHOD(AVCodec, long_name);
ZEND_METHOD(AVCodec, type);
ZEND_METHOD(AVCodec, id);
ZEND_METHOD(AVCodecContext, __construct);
ZEND_METHOD(AVCodecContext, pointer);
ZEND_METHOD(AVCodecContext, fromPointer);
ZEND_METHOD(AVCodecContext, codec_type);
ZEND_METHOD(AVCodecContext, codec_id);
ZEND_METHOD(AVCodecContext, width);
ZEND_METHOD(AVCodecContext, height);
ZEND_METHOD(AVCodecContext, pix_fmt);
ZEND_METHOD(AVCodecContext, sample_fmt);
ZEND_METHOD(AVCodecContext, sample_rate);
ZEND_METHOD(AVCodecContext, ch_layout);
ZEND_METHOD(AVCodecContext, time_base);
ZEND_METHOD(AVCodecParameters, __construct);
ZEND_METHOD(AVCodecParameters, pointer);
ZEND_METHOD(AVCodecParameters, fromPointer);
ZEND_METHOD(AVCodecParameters, codec_type);
ZEND_METHOD(AVCodecParameters, codec_id);
ZEND_METHOD(AVCodecParameters, format);
ZEND_METHOD(AVCodecParameters, width);
ZEND_METHOD(AVCodecParameters, height);
ZEND_METHOD(AVCodecParameters, sample_rate);
ZEND_METHOD(AVCodecParameters, ch_layout);
ZEND_METHOD(AVPacket, __construct);
ZEND_METHOD(AVPacket, pointer);
ZEND_METHOD(AVPacket, fromPointer);
ZEND_METHOD(AVPacket, stream_index);
ZEND_METHOD(AVPacket, pts);
ZEND_METHOD(AVPacket, dts);
ZEND_METHOD(AVPacket, duration);
ZEND_METHOD(AVPacket, size);
ZEND_METHOD(AVPacket, flags);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(avcodec_find_decoder, arginfo_avcodec_find_decoder)
	ZEND_FE(avcodec_get_name, arginfo_avcodec_get_name)
	ZEND_FE(avcodec_alloc_context3, arginfo_avcodec_alloc_context3)
	ZEND_FE(avcodec_parameters_to_context, arginfo_avcodec_parameters_to_context)
	ZEND_FE(avcodec_open2, arginfo_avcodec_open2)
	ZEND_FE(avcodec_send_packet, arginfo_avcodec_send_packet)
	ZEND_FE(avcodec_receive_frame, arginfo_avcodec_receive_frame)
	ZEND_FE(avcodec_flush_buffers, arginfo_avcodec_flush_buffers)
	ZEND_FE(avcodec_free_context, arginfo_avcodec_free_context)
	ZEND_FE(av_packet_alloc, arginfo_av_packet_alloc)
	ZEND_FE(av_packet_unref, arginfo_av_packet_unref)
	ZEND_FE(av_packet_free, arginfo_av_packet_free)
	ZEND_FE_END
};

static const zend_function_entry class_AVCodec_methods[] = {
	ZEND_ME(AVCodec, __construct, arginfo_class_AVCodec___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(AVCodec, pointer, arginfo_class_AVCodec_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodec, fromPointer, arginfo_class_AVCodec_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(AVCodec, name, arginfo_class_AVCodec_name, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodec, long_name, arginfo_class_AVCodec_long_name, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodec, type, arginfo_class_AVCodec_type, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodec, id, arginfo_class_AVCodec_id, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_AVCodecContext_methods[] = {
	ZEND_ME(AVCodecContext, __construct, arginfo_class_AVCodecContext___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(AVCodecContext, pointer, arginfo_class_AVCodecContext_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecContext, fromPointer, arginfo_class_AVCodecContext_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(AVCodecContext, codec_type, arginfo_class_AVCodecContext_codec_type, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecContext, codec_id, arginfo_class_AVCodecContext_codec_id, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecContext, width, arginfo_class_AVCodecContext_width, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecContext, height, arginfo_class_AVCodecContext_height, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecContext, pix_fmt, arginfo_class_AVCodecContext_pix_fmt, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecContext, sample_fmt, arginfo_class_AVCodecContext_sample_fmt, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecContext, sample_rate, arginfo_class_AVCodecContext_sample_rate, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecContext, ch_layout, arginfo_class_AVCodecContext_ch_layout, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecContext, time_base, arginfo_class_AVCodecContext_time_base, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_AVCodecParameters_methods[] = {
	ZEND_ME(AVCodecParameters, __construct, arginfo_class_AVCodecParameters___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(AVCodecParameters, pointer, arginfo_class_AVCodecParameters_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecParameters, fromPointer, arginfo_class_AVCodecParameters_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(AVCodecParameters, codec_type, arginfo_class_AVCodecParameters_codec_type, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecParameters, codec_id, arginfo_class_AVCodecParameters_codec_id, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecParameters, format, arginfo_class_AVCodecParameters_format, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecParameters, width, arginfo_class_AVCodecParameters_width, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecParameters, height, arginfo_class_AVCodecParameters_height, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecParameters, sample_rate, arginfo_class_AVCodecParameters_sample_rate, ZEND_ACC_PUBLIC)
	ZEND_ME(AVCodecParameters, ch_layout, arginfo_class_AVCodecParameters_ch_layout, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_AVPacket_methods[] = {
	ZEND_ME(AVPacket, __construct, arginfo_class_AVPacket___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(AVPacket, pointer, arginfo_class_AVPacket_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(AVPacket, fromPointer, arginfo_class_AVPacket_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(AVPacket, stream_index, arginfo_class_AVPacket_stream_index, ZEND_ACC_PUBLIC)
	ZEND_ME(AVPacket, pts, arginfo_class_AVPacket_pts, ZEND_ACC_PUBLIC)
	ZEND_ME(AVPacket, dts, arginfo_class_AVPacket_dts, ZEND_ACC_PUBLIC)
	ZEND_ME(AVPacket, duration, arginfo_class_AVPacket_duration, ZEND_ACC_PUBLIC)
	ZEND_ME(AVPacket, size, arginfo_class_AVPacket_size, ZEND_ACC_PUBLIC)
	ZEND_ME(AVPacket, flags, arginfo_class_AVPacket_flags, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_AVCodec(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "AVCodec", class_AVCodec_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_AVCodecContext(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "AVCodecContext", class_AVCodecContext_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_AVCodecParameters(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "AVCodecParameters", class_AVCodecParameters_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_AVPacket(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "AVPacket", class_AVPacket_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
