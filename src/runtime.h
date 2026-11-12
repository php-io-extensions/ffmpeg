#ifndef FFMPEG_RUNTIME_H
#define FFMPEG_RUNTIME_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_exceptions.h"
#include "../php_ffmpeg.h"

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/channel_layout.h>
#include <libavutil/dict.h>
#include <libavutil/error.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <libavutil/pixdesc.h>
#include <libavutil/rational.h>
#include <libavutil/samplefmt.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

ZEND_BEGIN_MODULE_GLOBALS(ffmpeg)
	HashTable boxes;               /* native address => zend_object*, live handles only, not refcounted */
ZEND_END_MODULE_GLOBALS(ffmpeg)

ZEND_EXTERN_MODULE_GLOBALS(ffmpeg)
#define FFMPEG_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(ffmpeg, v)

typedef struct {
	void *ptr;                     /* the native object; NULL once released */
	zval keep;                     /* the handles whose native objects this one owns: released with it */
	zend_object std;
} ffmpeg_handle;

static zend_always_inline ffmpeg_handle *ffmpeg_handle_from(zend_object *obj)
{
	return (ffmpeg_handle *) ((char *) obj - XtOffsetOf(ffmpeg_handle, std));
}

/* The handle's native pointer: NULL once released. */
static zend_always_inline void *ffmpeg_ptr(zend_object *obj)
{
	return ffmpeg_handle_from(obj)->ptr;
}

/* The pointer of an optional handle argument: NULL for null or a released handle. */
static zend_always_inline void *ffmpeg_ptr_or_null(zend_object *obj)
{
	return obj == NULL ? NULL : ffmpeg_handle_from(obj)->ptr;
}

/* The PHP object holding ptr, or a new one of class ce. NULL ptr gives null. */
void ffmpeg_box(zval *rv, void *ptr, zend_class_entry *ce);
/* As ffmpeg_box, and owner keeps the handle: releasing owner releases it. */
void ffmpeg_box_kept(zval *rv, zend_object *owner, void *ptr, zend_class_entry *ce);
/* Marks a handle released: pointer cleared, identity entry dropped, the handles it keeps released too. */
void ffmpeg_release(zend_object *obj);
/* For an in-and-out handle parameter: releases the handle the reference held when the native call replaced or freed it, then stores the result. */
void ffmpeg_ref_result(zval *ref, zend_object *before, void *after, zend_class_entry *ce);

void ffmpeg_handle_setup(zend_class_entry *ce);
void ffmpeg_struct_setup(zend_class_entry *ce);

/* A list of ints as a zeroed C array of at least `minimum` entries. False (and a TypeError) for a non-int element. */
bool ffmpeg_list_longs(HashTable *list, uint32_t arg_num, uint32_t minimum, int64_t **out, uint32_t *count);
/* A list of floats (ints accepted) as a zeroed C array of at least `minimum` entries. False (and a TypeError) for another element. */
bool ffmpeg_list_doubles(HashTable *list, uint32_t arg_num, uint32_t minimum, double **out);
/* A list of AVDictionary handles or nulls as a zeroed C array of at least `minimum` entries. False (and a TypeError) for another element. */
bool ffmpeg_list_dictionaries(HashTable *list, uint32_t arg_num, uint32_t minimum, AVDictionary ***out);

/* AVRational and AVChannelLayout <-> their PHP value classes. */
AVRational ffmpeg_rational_read(zend_object *obj);
void ffmpeg_rational_new(zval *rv, AVRational value);
void ffmpeg_layout_read(zend_object *obj, AVChannelLayout *layout);
void ffmpeg_layout_write(zend_object *obj, const AVChannelLayout *layout);
void ffmpeg_layout_new(zval *rv, const AVChannelLayout *layout);

/*
 * The probe switch. A kept refusal site reads `if (!FFMPEG_PROBE_OFF("<id>") && (<check>))`.
 * A build configured with --enable-ffmpeg-probes lets env FFMPEG_PROBE_OFF=<id> turn that one
 * site off, so tests/Probes can show the crash the site prevents; any other build compiles
 * the switch out.
 */
#ifdef FFMPEG_PROBES
bool ffmpeg_probe_off(const char *id);
# define FFMPEG_PROBE_OFF(id) ffmpeg_probe_off(id)
#else
# define FFMPEG_PROBE_OFF(id) 0
#endif

/* pointer(), fromPointer() and the private constructor of a handle class. The address is trusted; 0 (and a released handle) is NULL. */
#define FFMPEG_HANDLE_METHODS(cls) \
	ZEND_METHOD(cls, __construct) { ZEND_PARSE_PARAMETERS_NONE(); } \
	ZEND_METHOD(cls, pointer) { ZEND_PARSE_PARAMETERS_NONE(); RETURN_LONG((zend_long) (uintptr_t) ffmpeg_ptr(Z_OBJ_P(ZEND_THIS))); } \
	ZEND_METHOD(cls, fromPointer) { zend_long p; ZEND_PARSE_PARAMETERS_START(1, 1) Z_PARAM_LONG(p) ZEND_PARSE_PARAMETERS_END(); \
		ffmpeg_box(return_value, (void *) (uintptr_t) p, zend_get_called_scope(execute_data)); }

/* Every field getter of every handle class is this one function (src/fields.c). */
ZEND_NAMED_FUNCTION(ffmpeg_field);

extern zend_class_entry *ffmpeg_ce_AVRational;
extern zend_class_entry *ffmpeg_ce_AVChannelLayout;
extern zend_class_entry *ffmpeg_ce_AVFrame;
extern zend_class_entry *ffmpeg_ce_AVDictionary;
extern zend_class_entry *ffmpeg_ce_AVCodec;
extern zend_class_entry *ffmpeg_ce_AVCodecContext;
extern zend_class_entry *ffmpeg_ce_AVCodecParameters;
extern zend_class_entry *ffmpeg_ce_AVPacket;
extern zend_class_entry *ffmpeg_ce_AVInputFormat;
extern zend_class_entry *ffmpeg_ce_AVFormatContext;
extern zend_class_entry *ffmpeg_ce_AVStream;
extern zend_class_entry *ffmpeg_ce_SwsContext;
extern zend_class_entry *ffmpeg_ce_SwsFilter;
extern zend_class_entry *ffmpeg_ce_SwrContext;

void ffmpeg_register_libavutil(int module_number);
void ffmpeg_register_libavcodec(int module_number);
void ffmpeg_register_libavformat(int module_number);
void ffmpeg_register_libswscale(int module_number);
void ffmpeg_register_libswresample(int module_number);

#endif
