<?php

/** @generate-class-entries */

/**
 * @var int
 * @cvalue AVSEEK_FLAG_BACKWARD
 */
const AVSEEK_FLAG_BACKWARD = UNKNOWN;

/**
 * @var int
 * @cvalue AVSEEK_FLAG_BYTE
 */
const AVSEEK_FLAG_BYTE = UNKNOWN;

/**
 * @var int
 * @cvalue AVSEEK_FLAG_ANY
 */
const AVSEEK_FLAG_ANY = UNKNOWN;

/**
 * @var int
 * @cvalue AVSEEK_FLAG_FRAME
 */
const AVSEEK_FLAG_FRAME = UNKNOWN;

/**
 * Static: FFmpeg never frees a demuxer. Fields read live.
 *
 * @not-serializable
 */
final class AVInputFormat
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): ?static {}

    public function name(): string {}

    public function long_name(): ?string {}
}

/**
 * Fields read live. streams() are the nb_streams AVStream handles; they are released with the context.
 *
 * @not-serializable
 */
final class AVFormatContext
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): ?static {}

    public function nb_streams(): int {}

    public function streams(): array {}

    public function iformat(): ?AVInputFormat {}

    public function duration(): int {}

    public function start_time(): int {}

    public function bit_rate(): int {}
}

/**
 * Owned by its AVFormatContext and released with it. Fields read live.
 *
 * @not-serializable
 */
final class AVStream
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): ?static {}

    public function index(): int {}

    public function id(): int {}

    public function time_base(): AVRational {}

    public function start_time(): int {}

    public function duration(): int {}

    public function nb_frames(): int {}

    public function avg_frame_rate(): AVRational {}

    public function codecpar(): AVCodecParameters {}
}

/** libavformat/avformat.h. $ps receives the opened context; $options: one AVDictionary in and out, null for none. */
function avformat_open_input(?AVFormatContext &$ps, string $url, ?AVInputFormat $fmt, ?AVDictionary &$options): int {}

/** $options: null, or one AVDictionary per stream. */
function avformat_find_stream_info(AVFormatContext $ic, ?array $options): int {}

/** $decoder_ret receives the stream's decoder. */
function av_find_best_stream(AVFormatContext $ic, int $type, int $wanted_stream_nb, int $related_stream, ?AVCodec &$decoder_ret, int $flags): int {}

function av_read_frame(AVFormatContext $s, AVPacket $pkt): int {}

function av_seek_frame(AVFormatContext $s, int $stream_index, int $timestamp, int $flags): int {}

/** Releases the context, its streams and their parameters, and sets $s to null. */
function avformat_close_input(?AVFormatContext &$s): void {}
