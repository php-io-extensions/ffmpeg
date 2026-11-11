<?php

/** @generate-class-entries */

/**
 * Static: FFmpeg never frees a codec. Fields read live.
 *
 * @not-serializable
 */
final class AVCodec
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): ?static {}

    public function name(): string {}

    public function long_name(): ?string {}

    public function type(): int {}

    public function id(): int {}
}

/**
 * Fields read live. An address other than 0 is trusted.
 *
 * @not-serializable
 */
final class AVCodecContext
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): ?static {}

    public function codec_type(): int {}

    public function codec_id(): int {}

    public function width(): int {}

    public function height(): int {}

    public function pix_fmt(): int {}

    public function sample_fmt(): int {}

    public function sample_rate(): int {}

    public function ch_layout(): AVChannelLayout {}

    public function time_base(): AVRational {}
}

/**
 * Owned by its AVStream: released with the AVFormatContext. Fields read live.
 *
 * @not-serializable
 */
final class AVCodecParameters
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): ?static {}

    public function codec_type(): int {}

    public function codec_id(): int {}

    public function format(): int {}

    public function width(): int {}

    public function height(): int {}

    public function sample_rate(): int {}

    public function ch_layout(): AVChannelLayout {}
}

/**
 * Fields read live. An address other than 0 is trusted.
 *
 * @not-serializable
 */
final class AVPacket
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): ?static {}

    public function stream_index(): int {}

    public function pts(): int {}

    public function dts(): int {}

    public function duration(): int {}

    public function size(): int {}

    public function flags(): int {}
}

/** libavcodec/codec.h */
function avcodec_find_decoder(int $id): ?AVCodec {}

/** libavcodec/codec_id.h */
function avcodec_get_name(int $id): string {}

/** libavcodec/avcodec.h */
function avcodec_alloc_context3(?AVCodec $codec): ?AVCodecContext {}

function avcodec_parameters_to_context(AVCodecContext $codec, AVCodecParameters $par): int {}

/** $options: one AVDictionary in and out; null for none. */
function avcodec_open2(AVCodecContext $avctx, ?AVCodec $codec, ?AVDictionary &$options): int {}

/** A null $avpkt flushes the decoder. */
function avcodec_send_packet(AVCodecContext $avctx, ?AVPacket $avpkt): int {}

function avcodec_receive_frame(AVCodecContext $avctx, AVFrame $frame): int {}

function avcodec_flush_buffers(AVCodecContext $avctx): void {}

/** Releases the handle and sets $avctx to null. */
function avcodec_free_context(?AVCodecContext &$avctx): void {}

/** libavcodec/packet.h */
function av_packet_alloc(): ?AVPacket {}

function av_packet_unref(AVPacket $pkt): void {}

/** Releases the handle and sets $pkt to null. */
function av_packet_free(?AVPacket &$pkt): void {}
