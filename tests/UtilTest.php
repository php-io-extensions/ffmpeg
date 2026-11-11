<?php

declare(strict_types=1);

it('binds the error macro, error text and rationals', function (): void {
    $text = null;

    expect(AVERROR(11))->toBe(-11)
        ->and(av_strerror(AVERROR_EOF, $text, 64))->toBe(0)
        ->and($text)->toBe('End of file')
        ->and(av_strerror(AVERROR_EOF, $text, 4))->toBe(0)
        ->and($text)->toBe('End')
        ->and(av_q2d(new AVRational(1, 25)))->toBe(0.04)
        ->and(new AVRational())->toEqual(new AVRational(0, 0));
});

it('names pixel and sample formats and reports sample sizes', function (): void {
    expect(av_get_pix_fmt_name(AV_PIX_FMT_RGBA))->toBe('rgba')
        ->and(av_get_pix_fmt_name(-1))->toBeNull()
        ->and(av_get_sample_fmt_name(AV_SAMPLE_FMT_FLTP))->toBe('fltp')
        ->and(av_get_bytes_per_sample(AV_SAMPLE_FMT_S16))->toBe(2)
        ->and(av_sample_fmt_is_planar(AV_SAMPLE_FMT_FLTP))->toBe(1)
        ->and(av_sample_fmt_is_planar(AV_SAMPLE_FMT_S16))->toBe(0);
});

it('fills a channel layout in place', function (): void {
    $layout = new AVChannelLayout();
    av_channel_layout_default($layout, 2);

    expect($layout->order)->toBe(AV_CHANNEL_ORDER_NATIVE)
        ->and($layout->nb_channels)->toBe(2)
        ->and($layout->u)->toBe(3);

    av_channel_layout_uninit($layout);
    expect($layout->nb_channels)->toBe(0);
});

it('allocates one plane per channel for a planar format and one for a packed format', function (): void {
    $planar = null;
    $packed = null;
    $linesize = null;

    expect(av_samples_alloc($planar, $linesize, 2, 1024, AV_SAMPLE_FMT_FLTP, 0))->toBeGreaterThan(0)
        ->and($planar)->toHaveCount(2)
        ->and($linesize)->toBe(4096)
        ->and(av_samples_alloc($packed, $linesize, 2, 1024, AV_SAMPLE_FMT_S16, 0))->toBeGreaterThan(0)
        ->and($packed)->toHaveCount(1);

    av_free($planar[0]);
    av_free($packed[0]);
});

it('keeps one object per native pointer and releases it on free', function (): void {
    $frame = av_frame_alloc();
    $copy = $frame;

    expect(AVFrame::fromPointer($frame->pointer()))->toBe($frame)
        ->and(AVFrame::fromPointer(0))->toBeNull()
        ->and($frame->data())->toHaveCount(8)
        ->and($frame->linesize())->toBe(array_fill(0, 8, 0));

    av_frame_free($frame);
    expect($frame)->toBeNull()
        ->and($copy->pointer())->toBe(0)
        ->and(fn () => $copy->width())->toThrow(ValueError::class, 'AVFrame has been released');
});

it('refuses a list element of the wrong type before the native call', function (): void {
    $scaler = sws_getContext(4, 4, AV_PIX_FMT_RGBA, 4, 4, AV_PIX_FMT_RGBA, SWS_POINT, null, null, null);

    expect(fn () => sws_scale($scaler, ['x'], [16], 0, 4, [0], [16]))->toThrow(TypeError::class, 'must be a list of int, string given at index 0')
        ->and(fn () => sws_getContext(4, 4, AV_PIX_FMT_RGBA, 4, 4, AV_PIX_FMT_RGBA, SWS_BICUBIC, null, null, ['a']))->toThrow(TypeError::class, 'must be a list of float');

    sws_freeContext($scaler);
});
