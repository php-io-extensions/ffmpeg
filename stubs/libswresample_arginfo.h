/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 744bb3c4eef5b96c38fb1c3724117cc82f5eb99c */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_swr_alloc_set_opts2, 0, 9, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(1, ps, SwrContext, 1)
	ZEND_ARG_OBJ_INFO(0, out_ch_layout, AVChannelLayout, 0)
	ZEND_ARG_TYPE_INFO(0, out_sample_fmt, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, out_sample_rate, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, in_ch_layout, AVChannelLayout, 0)
	ZEND_ARG_TYPE_INFO(0, in_sample_fmt, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, in_sample_rate, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, log_offset, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, log_ctx, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_swr_init, 0, 1, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, s, SwrContext, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_swr_convert, 0, 5, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, s, SwrContext, 0)
	ZEND_ARG_TYPE_INFO(0, out, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, out_count, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, in, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, in_count, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_swr_get_delay, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, s, SwrContext, 0)
	ZEND_ARG_TYPE_INFO(0, base, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_swr_get_out_samples, 0, 2, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, s, SwrContext, 0)
	ZEND_ARG_TYPE_INFO(0, in_samples, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_swr_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(1, s, SwrContext, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_SwrContext___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SwrContext_pointer, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SwrContext_fromPointer, 0, 1, IS_STATIC, 1)
	ZEND_ARG_TYPE_INFO(0, pointer, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(swr_alloc_set_opts2);
ZEND_FUNCTION(swr_init);
ZEND_FUNCTION(swr_convert);
ZEND_FUNCTION(swr_get_delay);
ZEND_FUNCTION(swr_get_out_samples);
ZEND_FUNCTION(swr_free);
ZEND_METHOD(SwrContext, __construct);
ZEND_METHOD(SwrContext, pointer);
ZEND_METHOD(SwrContext, fromPointer);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(swr_alloc_set_opts2, arginfo_swr_alloc_set_opts2)
	ZEND_FE(swr_init, arginfo_swr_init)
	ZEND_FE(swr_convert, arginfo_swr_convert)
	ZEND_FE(swr_get_delay, arginfo_swr_get_delay)
	ZEND_FE(swr_get_out_samples, arginfo_swr_get_out_samples)
	ZEND_FE(swr_free, arginfo_swr_free)
	ZEND_FE_END
};

static const zend_function_entry class_SwrContext_methods[] = {
	ZEND_ME(SwrContext, __construct, arginfo_class_SwrContext___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(SwrContext, pointer, arginfo_class_SwrContext_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(SwrContext, fromPointer, arginfo_class_SwrContext_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_SwrContext(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "SwrContext", class_SwrContext_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
