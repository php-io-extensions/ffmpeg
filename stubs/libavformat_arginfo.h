/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: cdb1d9f622e556bcc5f49b9024e127eed0adf985 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_avformat_open_input, 0, 4, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(1, ps, AVFormatContext, 1)
	ZEND_ARG_TYPE_INFO(0, url, IS_STRING, 0)
	ZEND_ARG_OBJ_INFO(0, fmt, AVInputFormat, 1)
	ZEND_ARG_OBJ_INFO(1, options, AVDictionary, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_avformat_find_stream_info, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, ic, AVFormatContext, 0)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_av_find_best_stream, 0, 6, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, ic, AVFormatContext, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, wanted_stream_nb, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, related_stream, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(1, decoder_ret, AVCodec, 1)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_av_read_frame, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, s, AVFormatContext, 0)
	ZEND_ARG_OBJ_INFO(0, pkt, AVPacket, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_av_seek_frame, 0, 4, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, s, AVFormatContext, 0)
	ZEND_ARG_TYPE_INFO(0, stream_index, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, timestamp, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_avformat_close_input, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(1, s, AVFormatContext, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_AVInputFormat___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AVInputFormat_pointer, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AVInputFormat_fromPointer, 0, 1, IS_STATIC, 1)
	ZEND_ARG_TYPE_INFO(0, pointer, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AVInputFormat_name, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AVInputFormat_long_name, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_AVFormatContext___construct arginfo_class_AVInputFormat___construct

#define arginfo_class_AVFormatContext_pointer arginfo_class_AVInputFormat_pointer

#define arginfo_class_AVFormatContext_fromPointer arginfo_class_AVInputFormat_fromPointer

#define arginfo_class_AVFormatContext_nb_streams arginfo_class_AVInputFormat_pointer

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_AVFormatContext_streams, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_AVFormatContext_iformat, 0, 0, AVInputFormat, 1)
ZEND_END_ARG_INFO()

#define arginfo_class_AVFormatContext_duration arginfo_class_AVInputFormat_pointer

#define arginfo_class_AVFormatContext_start_time arginfo_class_AVInputFormat_pointer

#define arginfo_class_AVFormatContext_bit_rate arginfo_class_AVInputFormat_pointer

#define arginfo_class_AVStream___construct arginfo_class_AVInputFormat___construct

#define arginfo_class_AVStream_pointer arginfo_class_AVInputFormat_pointer

#define arginfo_class_AVStream_fromPointer arginfo_class_AVInputFormat_fromPointer

#define arginfo_class_AVStream_index arginfo_class_AVInputFormat_pointer

#define arginfo_class_AVStream_id arginfo_class_AVInputFormat_pointer

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_AVStream_time_base, 0, 0, AVRational, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_AVStream_start_time arginfo_class_AVInputFormat_pointer

#define arginfo_class_AVStream_duration arginfo_class_AVInputFormat_pointer

#define arginfo_class_AVStream_nb_frames arginfo_class_AVInputFormat_pointer

#define arginfo_class_AVStream_avg_frame_rate arginfo_class_AVStream_time_base

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_AVStream_codecpar, 0, 0, AVCodecParameters, 0)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(avformat_open_input);
ZEND_FUNCTION(avformat_find_stream_info);
ZEND_FUNCTION(av_find_best_stream);
ZEND_FUNCTION(av_read_frame);
ZEND_FUNCTION(av_seek_frame);
ZEND_FUNCTION(avformat_close_input);
ZEND_METHOD(AVInputFormat, __construct);
ZEND_METHOD(AVInputFormat, pointer);
ZEND_METHOD(AVInputFormat, fromPointer);
ZEND_METHOD(AVInputFormat, name);
ZEND_METHOD(AVInputFormat, long_name);
ZEND_METHOD(AVFormatContext, __construct);
ZEND_METHOD(AVFormatContext, pointer);
ZEND_METHOD(AVFormatContext, fromPointer);
ZEND_METHOD(AVFormatContext, nb_streams);
ZEND_METHOD(AVFormatContext, streams);
ZEND_METHOD(AVFormatContext, iformat);
ZEND_METHOD(AVFormatContext, duration);
ZEND_METHOD(AVFormatContext, start_time);
ZEND_METHOD(AVFormatContext, bit_rate);
ZEND_METHOD(AVStream, __construct);
ZEND_METHOD(AVStream, pointer);
ZEND_METHOD(AVStream, fromPointer);
ZEND_METHOD(AVStream, index);
ZEND_METHOD(AVStream, id);
ZEND_METHOD(AVStream, time_base);
ZEND_METHOD(AVStream, start_time);
ZEND_METHOD(AVStream, duration);
ZEND_METHOD(AVStream, nb_frames);
ZEND_METHOD(AVStream, avg_frame_rate);
ZEND_METHOD(AVStream, codecpar);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(avformat_open_input, arginfo_avformat_open_input)
	ZEND_FE(avformat_find_stream_info, arginfo_avformat_find_stream_info)
	ZEND_FE(av_find_best_stream, arginfo_av_find_best_stream)
	ZEND_FE(av_read_frame, arginfo_av_read_frame)
	ZEND_FE(av_seek_frame, arginfo_av_seek_frame)
	ZEND_FE(avformat_close_input, arginfo_avformat_close_input)
	ZEND_FE_END
};

static const zend_function_entry class_AVInputFormat_methods[] = {
	ZEND_ME(AVInputFormat, __construct, arginfo_class_AVInputFormat___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(AVInputFormat, pointer, arginfo_class_AVInputFormat_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(AVInputFormat, fromPointer, arginfo_class_AVInputFormat_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(AVInputFormat, name, arginfo_class_AVInputFormat_name, ZEND_ACC_PUBLIC)
	ZEND_ME(AVInputFormat, long_name, arginfo_class_AVInputFormat_long_name, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_AVFormatContext_methods[] = {
	ZEND_ME(AVFormatContext, __construct, arginfo_class_AVFormatContext___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(AVFormatContext, pointer, arginfo_class_AVFormatContext_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(AVFormatContext, fromPointer, arginfo_class_AVFormatContext_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(AVFormatContext, nb_streams, arginfo_class_AVFormatContext_nb_streams, ZEND_ACC_PUBLIC)
	ZEND_ME(AVFormatContext, streams, arginfo_class_AVFormatContext_streams, ZEND_ACC_PUBLIC)
	ZEND_ME(AVFormatContext, iformat, arginfo_class_AVFormatContext_iformat, ZEND_ACC_PUBLIC)
	ZEND_ME(AVFormatContext, duration, arginfo_class_AVFormatContext_duration, ZEND_ACC_PUBLIC)
	ZEND_ME(AVFormatContext, start_time, arginfo_class_AVFormatContext_start_time, ZEND_ACC_PUBLIC)
	ZEND_ME(AVFormatContext, bit_rate, arginfo_class_AVFormatContext_bit_rate, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static const zend_function_entry class_AVStream_methods[] = {
	ZEND_ME(AVStream, __construct, arginfo_class_AVStream___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(AVStream, pointer, arginfo_class_AVStream_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(AVStream, fromPointer, arginfo_class_AVStream_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(AVStream, index, arginfo_class_AVStream_index, ZEND_ACC_PUBLIC)
	ZEND_ME(AVStream, id, arginfo_class_AVStream_id, ZEND_ACC_PUBLIC)
	ZEND_ME(AVStream, time_base, arginfo_class_AVStream_time_base, ZEND_ACC_PUBLIC)
	ZEND_ME(AVStream, start_time, arginfo_class_AVStream_start_time, ZEND_ACC_PUBLIC)
	ZEND_ME(AVStream, duration, arginfo_class_AVStream_duration, ZEND_ACC_PUBLIC)
	ZEND_ME(AVStream, nb_frames, arginfo_class_AVStream_nb_frames, ZEND_ACC_PUBLIC)
	ZEND_ME(AVStream, avg_frame_rate, arginfo_class_AVStream_avg_frame_rate, ZEND_ACC_PUBLIC)
	ZEND_ME(AVStream, codecpar, arginfo_class_AVStream_codecpar, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static void register_libavformat_symbols(int module_number)
{
	REGISTER_LONG_CONSTANT("AVSEEK_FLAG_BACKWARD", AVSEEK_FLAG_BACKWARD, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AVSEEK_FLAG_BYTE", AVSEEK_FLAG_BYTE, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AVSEEK_FLAG_ANY", AVSEEK_FLAG_ANY, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("AVSEEK_FLAG_FRAME", AVSEEK_FLAG_FRAME, CONST_PERSISTENT);
}

static zend_class_entry *register_class_AVInputFormat(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "AVInputFormat", class_AVInputFormat_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_AVFormatContext(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "AVFormatContext", class_AVFormatContext_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_AVStream(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "AVStream", class_AVStream_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
