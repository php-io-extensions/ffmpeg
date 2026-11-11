/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 1fd1762755020e116db453d335da51c01b7b35db */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_sws_getContext, 0, 10, SwsContext, 1)
	ZEND_ARG_TYPE_INFO(0, srcW, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, srcH, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, srcFormat, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, dstW, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, dstH, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, dstFormat, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, srcFilter, SwsFilter, 1)
	ZEND_ARG_OBJ_INFO(0, dstFilter, SwsFilter, 1)
	ZEND_ARG_TYPE_INFO(0, param, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_sws_scale, 0, 7, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, c, SwsContext, 0)
	ZEND_ARG_TYPE_INFO(0, srcSlice, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, srcStride, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, srcSliceY, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, srcSliceH, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, dst, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, dstStride, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_sws_freeContext, 0, 1, IS_VOID, 0)
	ZEND_ARG_OBJ_INFO(0, swsContext, SwsContext, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_SwsContext___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SwsContext_pointer, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SwsContext_fromPointer, 0, 1, IS_STATIC, 1)
	ZEND_ARG_TYPE_INFO(0, pointer, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_SwsFilter___construct arginfo_class_SwsContext___construct

#define arginfo_class_SwsFilter_pointer arginfo_class_SwsContext_pointer

#define arginfo_class_SwsFilter_fromPointer arginfo_class_SwsContext_fromPointer

ZEND_FUNCTION(sws_getContext);
ZEND_FUNCTION(sws_scale);
ZEND_FUNCTION(sws_freeContext);
ZEND_METHOD(SwsContext, __construct);
ZEND_METHOD(SwsContext, pointer);
ZEND_METHOD(SwsContext, fromPointer);
ZEND_METHOD(SwsFilter, __construct);
ZEND_METHOD(SwsFilter, pointer);
ZEND_METHOD(SwsFilter, fromPointer);

static const zend_function_entry ext_functions[] = {
	ZEND_FE(sws_getContext, arginfo_sws_getContext)
	ZEND_FE(sws_scale, arginfo_sws_scale)
	ZEND_FE(sws_freeContext, arginfo_sws_freeContext)
	ZEND_FE_END
};

static const zend_function_entry class_SwsContext_methods[] = {
	ZEND_ME(SwsContext, __construct, arginfo_class_SwsContext___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(SwsContext, pointer, arginfo_class_SwsContext_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(SwsContext, fromPointer, arginfo_class_SwsContext_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static const zend_function_entry class_SwsFilter_methods[] = {
	ZEND_ME(SwsFilter, __construct, arginfo_class_SwsFilter___construct, ZEND_ACC_PRIVATE)
	ZEND_ME(SwsFilter, pointer, arginfo_class_SwsFilter_pointer, ZEND_ACC_PUBLIC)
	ZEND_ME(SwsFilter, fromPointer, arginfo_class_SwsFilter_fromPointer, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_FE_END
};

static void register_libswscale_symbols(int module_number)
{
	REGISTER_LONG_CONSTANT("SWS_FAST_BILINEAR", SWS_FAST_BILINEAR, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_BILINEAR", SWS_BILINEAR, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_BICUBIC", SWS_BICUBIC, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_X", SWS_X, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_POINT", SWS_POINT, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_AREA", SWS_AREA, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_BICUBLIN", SWS_BICUBLIN, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_GAUSS", SWS_GAUSS, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_SINC", SWS_SINC, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_LANCZOS", SWS_LANCZOS, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_SPLINE", SWS_SPLINE, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_PRINT_INFO", SWS_PRINT_INFO, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_FULL_CHR_H_INT", SWS_FULL_CHR_H_INT, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_FULL_CHR_H_INP", SWS_FULL_CHR_H_INP, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_DIRECT_BGR", SWS_DIRECT_BGR, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_ACCURATE_RND", SWS_ACCURATE_RND, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_BITEXACT", SWS_BITEXACT, CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SWS_ERROR_DIFFUSION", SWS_ERROR_DIFFUSION, CONST_PERSISTENT);
}

static zend_class_entry *register_class_SwsContext(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "SwsContext", class_SwsContext_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}

static zend_class_entry *register_class_SwsFilter(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "SwsFilter", class_SwsFilter_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, NULL, ZEND_ACC_FINAL|ZEND_ACC_NOT_SERIALIZABLE);

	return class_entry;
}
