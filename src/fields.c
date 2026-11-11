/*
 * The field getters of the handle classes, read live from the native struct.
 * Every getter method's entry is this one function (each library's file maps
 * zim_<Class>_<field> to ffmpeg_field before it includes its arginfo): it finds
 * the field by the called method's class and name.
 */

#include "runtime.h"

typedef enum {
	FIELD_INT,          /* int, or an enum */
	FIELD_UINT,         /* unsigned int */
	FIELD_INT64,        /* int64_t */
	FIELD_STRING,       /* const char *, NULL gives null */
	FIELD_RATIONAL,     /* AVRational, as a new AVRational */
	FIELD_LAYOUT,       /* AVChannelLayout, as a new AVChannelLayout */
	FIELD_PLANES,       /* uint8_t *[AV_NUM_DATA_POINTERS], as a list of addresses */
	FIELD_LINESIZES,    /* int [AV_NUM_DATA_POINTERS] */
	FIELD_STATIC,       /* a pointer to an object FFmpeg never frees, as a handle */
	FIELD_OWNED,        /* a pointer to an object the struct owns, as a handle the owner keeps */
	FIELD_STREAMS,      /* AVFormatContext's streams and nb_streams, as a list of handles the context keeps */
} ffmpeg_field_kind;

typedef struct {
	zend_class_entry **ce;
	const char *name;
	ffmpeg_field_kind kind;
	size_t offset;
	zend_class_entry **target;  /* the class of a FIELD_STATIC or FIELD_OWNED handle */
} ffmpeg_field_entry;

#define F(cls, field, kind) { &ffmpeg_ce_##cls, #field, kind, offsetof(cls, field), NULL }
#define H(cls, field, kind, target) { &ffmpeg_ce_##cls, #field, kind, offsetof(cls, field), &ffmpeg_ce_##target }

static const ffmpeg_field_entry ffmpeg_fields[] = {
	F(AVFrame, width, FIELD_INT),
	F(AVFrame, height, FIELD_INT),
	F(AVFrame, format, FIELD_INT),
	F(AVFrame, data, FIELD_PLANES),
	F(AVFrame, linesize, FIELD_LINESIZES),
	F(AVFrame, nb_samples, FIELD_INT),
	F(AVFrame, sample_rate, FIELD_INT),
	F(AVFrame, ch_layout, FIELD_LAYOUT),
	F(AVFrame, pts, FIELD_INT64),
	F(AVFrame, best_effort_timestamp, FIELD_INT64),
	F(AVFrame, duration, FIELD_INT64),
	F(AVFrame, time_base, FIELD_RATIONAL),

	F(AVCodec, name, FIELD_STRING),
	F(AVCodec, long_name, FIELD_STRING),
	F(AVCodec, type, FIELD_INT),
	F(AVCodec, id, FIELD_INT),

	F(AVCodecContext, codec_type, FIELD_INT),
	F(AVCodecContext, codec_id, FIELD_INT),
	F(AVCodecContext, width, FIELD_INT),
	F(AVCodecContext, height, FIELD_INT),
	F(AVCodecContext, pix_fmt, FIELD_INT),
	F(AVCodecContext, sample_fmt, FIELD_INT),
	F(AVCodecContext, sample_rate, FIELD_INT),
	F(AVCodecContext, ch_layout, FIELD_LAYOUT),
	F(AVCodecContext, time_base, FIELD_RATIONAL),

	F(AVCodecParameters, codec_type, FIELD_INT),
	F(AVCodecParameters, codec_id, FIELD_INT),
	F(AVCodecParameters, format, FIELD_INT),
	F(AVCodecParameters, width, FIELD_INT),
	F(AVCodecParameters, height, FIELD_INT),
	F(AVCodecParameters, sample_rate, FIELD_INT),
	F(AVCodecParameters, ch_layout, FIELD_LAYOUT),

	F(AVPacket, stream_index, FIELD_INT),
	F(AVPacket, pts, FIELD_INT64),
	F(AVPacket, dts, FIELD_INT64),
	F(AVPacket, duration, FIELD_INT64),
	F(AVPacket, size, FIELD_INT),
	F(AVPacket, flags, FIELD_INT),

	F(AVInputFormat, name, FIELD_STRING),
	F(AVInputFormat, long_name, FIELD_STRING),

	F(AVFormatContext, nb_streams, FIELD_UINT),
	F(AVFormatContext, streams, FIELD_STREAMS),
	H(AVFormatContext, iformat, FIELD_STATIC, AVInputFormat),
	F(AVFormatContext, duration, FIELD_INT64),
	F(AVFormatContext, start_time, FIELD_INT64),
	F(AVFormatContext, bit_rate, FIELD_INT64),

	F(AVStream, index, FIELD_INT),
	F(AVStream, id, FIELD_INT),
	F(AVStream, time_base, FIELD_RATIONAL),
	F(AVStream, start_time, FIELD_INT64),
	F(AVStream, duration, FIELD_INT64),
	F(AVStream, nb_frames, FIELD_INT64),
	F(AVStream, avg_frame_rate, FIELD_RATIONAL),
	H(AVStream, codecpar, FIELD_OWNED, AVCodecParameters),
};

ZEND_NAMED_FUNCTION(ffmpeg_field)
{
	zend_object *self = Z_OBJ_P(ZEND_THIS);
	zend_string *method = EX(func)->common.function_name;
	const ffmpeg_field_entry *entry = NULL;
	const char *base;
	size_t i;

	ZEND_PARSE_PARAMETERS_NONE();

	for (i = 0; i < sizeof(ffmpeg_fields) / sizeof(ffmpeg_fields[0]); i++) {
		if (*ffmpeg_fields[i].ce == self->ce && zend_string_equals_cstr(method, ffmpeg_fields[i].name, strlen(ffmpeg_fields[i].name))) {
			entry = &ffmpeg_fields[i];
			break;
		}
	}
	ZEND_ASSERT(entry != NULL);

	base = ffmpeg_ptr(self);
	/* The getter reads the struct itself: a released handle is NULL. */
	if (!FFMPEG_PROBE_OFF("ffmpeg-field-released") && (base == NULL)) {
		zend_value_error("%s has been released", ZSTR_VAL(self->ce->name)); /* binding:ffmpeg-field-released */
		RETURN_THROWS();
	}
	base += entry->offset;

	switch (entry->kind) {
		case FIELD_INT:
			RETURN_LONG(*(const int *) base);
		case FIELD_UINT:
			RETURN_LONG((zend_long) *(const unsigned int *) base);
		case FIELD_INT64:
			RETURN_LONG((zend_long) *(const int64_t *) base);
		case FIELD_STRING: {
			const char *text = *(const char *const *) base;

			if (text == NULL) {
				RETURN_NULL();
			}
			RETURN_STRING(text);
		}
		case FIELD_RATIONAL:
			ffmpeg_rational_new(return_value, *(const AVRational *) base);
			return;
		case FIELD_LAYOUT:
			ffmpeg_layout_new(return_value, (const AVChannelLayout *) base);
			return;
		case FIELD_PLANES:
			array_init_size(return_value, AV_NUM_DATA_POINTERS);
			for (i = 0; i < AV_NUM_DATA_POINTERS; i++) {
				add_next_index_long(return_value, (zend_long) (uintptr_t) ((uint8_t *const *) base)[i]);
			}
			return;
		case FIELD_LINESIZES:
			array_init_size(return_value, AV_NUM_DATA_POINTERS);
			for (i = 0; i < AV_NUM_DATA_POINTERS; i++) {
				add_next_index_long(return_value, ((const int *) base)[i]);
			}
			return;
		case FIELD_STATIC:
			ffmpeg_box(return_value, *(void *const *) base, *entry->target);
			return;
		case FIELD_OWNED:
			ffmpeg_box_kept(return_value, self, *(void *const *) base, *entry->target);
			return;
		case FIELD_STREAMS: {
			const AVFormatContext *context = (const AVFormatContext *) ffmpeg_ptr(self);
			zval stream;

			array_init_size(return_value, context->nb_streams);
			for (i = 0; i < context->nb_streams; i++) {
				ffmpeg_box_kept(&stream, self, context->streams[i], ffmpeg_ce_AVStream);
				add_next_index_zval(return_value, &stream);
			}
			return;
		}
	}
}
