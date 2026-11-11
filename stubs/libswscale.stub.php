<?php

/** @generate-class-entries */

/**
 * @var int
 * @cvalue SWS_FAST_BILINEAR
 */
const SWS_FAST_BILINEAR = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_BILINEAR
 */
const SWS_BILINEAR = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_BICUBIC
 */
const SWS_BICUBIC = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_X
 */
const SWS_X = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_POINT
 */
const SWS_POINT = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_AREA
 */
const SWS_AREA = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_BICUBLIN
 */
const SWS_BICUBLIN = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_GAUSS
 */
const SWS_GAUSS = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_SINC
 */
const SWS_SINC = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_LANCZOS
 */
const SWS_LANCZOS = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_SPLINE
 */
const SWS_SPLINE = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_PRINT_INFO
 */
const SWS_PRINT_INFO = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_FULL_CHR_H_INT
 */
const SWS_FULL_CHR_H_INT = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_FULL_CHR_H_INP
 */
const SWS_FULL_CHR_H_INP = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_DIRECT_BGR
 */
const SWS_DIRECT_BGR = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_ACCURATE_RND
 */
const SWS_ACCURATE_RND = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_BITEXACT
 */
const SWS_BITEXACT = UNKNOWN;

/**
 * @var int
 * @cvalue SWS_ERROR_DIFFUSION
 */
const SWS_ERROR_DIFFUSION = UNKNOWN;

/**
 * An address other than 0 is trusted.
 *
 * @not-serializable
 */
final class SwsContext
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): ?static {}
}

/**
 * An address other than 0 is trusted.
 *
 * @not-serializable
 */
final class SwsFilter
{
    private function __construct() {}

    public function pointer(): int {}

    public static function fromPointer(int $pointer): ?static {}
}

/** libswscale/swscale.h. $param: null, or the scaler's parameters. */
function sws_getContext(int $srcW, int $srcH, int $srcFormat, int $dstW, int $dstH, int $dstFormat, int $flags, ?SwsFilter $srcFilter, ?SwsFilter $dstFilter, ?array $param): ?SwsContext {}

/** $srcSlice and $dst are plane addresses, $srcStride and $dstStride line sizes; missing planes are NULL. */
function sws_scale(SwsContext $c, array $srcSlice, array $srcStride, int $srcSliceY, int $srcSliceH, array $dst, array $dstStride): int {}

/** Releases the handle. */
function sws_freeContext(?SwsContext $swsContext): void {}
