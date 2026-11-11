PHP_ARG_ENABLE([ffmpeg],
  [whether to enable ffmpeg support],
  [AS_HELP_STRING([--enable-ffmpeg], [Enable the FFmpeg bindings (libavformat, libavcodec, libavutil, libswscale, libswresample)])],
  [no])

PHP_ARG_ENABLE([ffmpeg-probes],
  [whether to build the ffmpeg probe switch],
  [AS_HELP_STRING([--enable-ffmpeg-probes],
    [Build the FFMPEG_PROBE_OFF switch that tests/Probes drives: env FFMPEG_PROBE_OFF=<site id> turns that kept refusal off. Never for a release build])],
  [no],
  [no])

if test "$PHP_FFMPEG" != "no"; then
  if test "$PHP_FFMPEG_PROBES" != "no"; then
    AC_DEFINE([FFMPEG_PROBES], [1], [Kept refusal sites can be turned off through env FFMPEG_PROBE_OFF])
  fi
  dnl The floor is FFmpeg 7.1 (Debian trixie): these are its library versions.
  PKG_CHECK_MODULES([FFMPEG], [libavformat >= 61.7.100 libavcodec >= 61.19.100 libavutil >= 59.39.100 libswscale >= 8.3.100 libswresample >= 5.3.100])
  PHP_EVAL_INCLINE([$FFMPEG_CFLAGS])
  PHP_EVAL_LIBLINE([$FFMPEG_LIBS], [FFMPEG_SHARED_LIBADD])
  PHP_SUBST([FFMPEG_SHARED_LIBADD])

  PHP_NEW_EXTENSION([ffmpeg],
    [src/ffmpeg.c src/runtime.c src/fields.c src/libavutil.c src/libavcodec.c src/libavformat.c src/libswscale.c src/libswresample.c],
    [$ext_shared],, [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1])
  PHP_ADD_BUILD_DIR([$ext_builddir/src])
fi
