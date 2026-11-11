/*
 * ffmpeg: 1:1 bindings of FFmpeg's libavformat, libavcodec, libavutil,
 * libswscale and libswresample. Function and constant names are the C names.
 * The floor is FFmpeg 7.1; only functions 7.1 and the newest supported FFmpeg
 * both declare are bound. Dropping the last PHP reference to a handle does not
 * free the native object.
 */

#include "runtime.h"
#include "ext/standard/info.h"

ZEND_DECLARE_MODULE_GLOBALS(ffmpeg)

static PHP_GINIT_FUNCTION(ffmpeg)
{
#if defined(COMPILE_DL_FFMPEG) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	zend_hash_init(&ffmpeg_globals->boxes, 64, NULL, NULL, 1);
}

static PHP_GSHUTDOWN_FUNCTION(ffmpeg)
{
	zend_hash_destroy(&ffmpeg_globals->boxes);
}

PHP_MINIT_FUNCTION(ffmpeg)
{
	ffmpeg_register_libavutil(module_number);
	ffmpeg_register_libavcodec(module_number);
	ffmpeg_register_libavformat(module_number);
	ffmpeg_register_libswscale(module_number);
	ffmpeg_register_libswresample(module_number);
#ifdef FFMPEG_PROBES
	REGISTER_BOOL_CONSTANT("FFMPEG_PROBES", true, CONST_PERSISTENT);
#endif

	return SUCCESS;
}

PHP_RINIT_FUNCTION(ffmpeg)
{
#if defined(COMPILE_DL_FFMPEG) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

/* One row per library: the version that loaded and the license it was built under. */
static void ffmpeg_info_row(const char *library, unsigned version, const char *license)
{
	char text[96];

	snprintf(text, sizeof(text), "%u.%u.%u (%s)", AV_VERSION_MAJOR(version), AV_VERSION_MINOR(version), AV_VERSION_MICRO(version), license);
	php_info_print_table_row(2, library, text);
}

PHP_MINFO_FUNCTION(ffmpeg)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "ffmpeg support", "enabled");
	php_info_print_table_row(2, "Version", PHP_FFMPEG_VERSION);
	php_info_print_table_row(2, "FFmpeg", av_version_info());
	ffmpeg_info_row("libavformat", avformat_version(), avformat_license());
	ffmpeg_info_row("libavcodec", avcodec_version(), avcodec_license());
	ffmpeg_info_row("libavutil", avutil_version(), avutil_license());
	ffmpeg_info_row("libswscale", swscale_version(), swscale_license());
	ffmpeg_info_row("libswresample", swresample_version(), swresample_license());
	php_info_print_table_end();
}

zend_module_entry ffmpeg_module_entry = {
	STANDARD_MODULE_HEADER,
	"ffmpeg",
	NULL,
	PHP_MINIT(ffmpeg),
	NULL,
	PHP_RINIT(ffmpeg),
	NULL,
	PHP_MINFO(ffmpeg),
	PHP_FFMPEG_VERSION,
	PHP_MODULE_GLOBALS(ffmpeg),
	PHP_GINIT(ffmpeg),
	PHP_GSHUTDOWN(ffmpeg),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_FFMPEG
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(ffmpeg)
#endif
