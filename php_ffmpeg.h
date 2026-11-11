#ifndef PHP_FFMPEG_H
#define PHP_FFMPEG_H

extern zend_module_entry ffmpeg_module_entry;
#define phpext_ffmpeg_ptr &ffmpeg_module_entry

#define PHP_FFMPEG_VERSION "0.11.0"

#if defined(ZTS) && defined(COMPILE_DL_FFMPEG)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif
