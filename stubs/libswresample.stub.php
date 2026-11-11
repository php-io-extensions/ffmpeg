<?php

/** @generate-class-entries */

/**
 * An address other than 0 is trusted.
 *
 * @not-serializable
 */
final class SwrContext
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): ?static {}
}

/** libswresample/swresample.h. $ps receives the context; $log_ctx is an address, 0 for none. */
function swr_alloc_set_opts2(?SwrContext &$ps, AVChannelLayout $out_ch_layout, int $out_sample_fmt, int $out_sample_rate, AVChannelLayout $in_ch_layout, int $in_sample_fmt, int $in_sample_rate, int $log_offset, int $log_ctx): int {}

function swr_init(SwrContext $s): int {}

/** $out and $in are plane addresses; missing planes are NULL. $out_count and $in_count are samples per channel. */
function swr_convert(SwrContext $s, ?array $out, int $out_count, ?array $in, int $in_count): int {}

function swr_get_delay(SwrContext $s, int $base): int {}

function swr_get_out_samples(SwrContext $s, int $in_samples): int {}

/** Releases the handle and sets $s to null. */
function swr_free(?SwrContext &$s): void {}
