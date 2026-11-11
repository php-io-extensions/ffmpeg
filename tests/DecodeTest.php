<?php

declare(strict_types=1);

it('decodes every video frame, flushing the decoder at the end', function (): void {
    $context = openFixture();
    [$decoder, $index] = openDecoder($context, AVMEDIA_TYPE_VIDEO);
    $frames = [];
    $count = decodeAll($context, $decoder, $index, function (AVFrame $frame) use (&$frames): void {
        $frames[] = [$frame->width(), $frame->height(), $frame->format(), $frame->best_effort_timestamp()];
    });

    expect($count)->toBe(25)
        ->and($decoder->width())->toBe(64)
        ->and($decoder->pix_fmt())->toBe(AV_PIX_FMT_YUV420P)
        ->and(array_unique(array_map(fn (array $f): string => "{$f[0]}x{$f[1]} {$f[2]}", $frames)))->toBe(['64x48 '.AV_PIX_FMT_YUV420P])
        ->and(array_column($frames, 3))->toBe(array_values(array_unique(array_column($frames, 3))));

    avcodec_free_context($decoder);
    avformat_close_input($context);
    expect($decoder)->toBeNull();
});

it('converts a decoded frame to RGBA with sws_scale into memory it is given by address', function (): void {
    $context = openFixture();
    [$decoder, $index] = openDecoder($context, AVMEDIA_TYPE_VIDEO);
    $scaler = sws_getContext(64, 48, AV_PIX_FMT_YUV420P, 64, 48, AV_PIX_FMT_RGBA, SWS_POINT, null, null, null);
    // One packed plane of 64 × 48 four-byte samples is a 64 × 48 RGBA image.
    $planes = null;
    $linesize = null;
    expect(av_samples_alloc($planes, $linesize, 1, 64 * 48, AV_SAMPLE_FMT_S32, 1))->toBeGreaterThanOrEqual(64 * 48 * 4);

    $rows = [];
    decodeAll($context, $decoder, $index, function (AVFrame $frame) use ($scaler, $planes, &$rows): void {
        $rows[] = sws_scale($scaler, $frame->data(), $frame->linesize(), 0, $frame->height(), $planes, [64 * 4]);
    });

    expect($scaler)->toBeInstanceOf(SwsContext::class)
        ->and($rows)->toHaveCount(25)
        ->and(array_unique($rows))->toBe([48]);

    av_free($planes[0]);
    sws_freeContext($scaler);
    expect($scaler->pointer())->toBe(0);
    avcodec_free_context($decoder);
    avformat_close_input($context);
});

it('writes red RGBA pixels into an ext-fb buffer', function (): void {
    if (! extension_loaded('fb')) {
        test()->markTestSkipped('ext-fb is not loaded: no buffer whose bytes PHP can read back');
    }
    $context = openFixture();
    [$decoder, $index] = openDecoder($context, AVMEDIA_TYPE_VIDEO);
    $scaler = sws_getContext(64, 48, AV_PIX_FMT_YUV420P, 64, 48, AV_PIX_FMT_RGBA, SWS_POINT | SWS_ACCURATE_RND | SWS_FULL_CHR_H_INT, null, null, null);
    $buffer = new FbBuffer(new FbFormat(FB_LAYOUT_RGBA8888, FB_BIT_ORDER_MSB, FB_BYTE_ORDER_MSB, FB_CHANNELS_RGBA), 64, 48);
    $done = false;
    decodeAll($context, $decoder, $index, function (AVFrame $frame) use ($scaler, $buffer, &$done): void {
        if (! $done) {
            sws_scale($scaler, $frame->data(), $frame->linesize(), 0, 48, [$buffer->pointer()], [64 * 4]);
            $done = true;
        }
    });
    $pixel = array_values(unpack('C4', $buffer->rgba8()));

    expect($pixel[0])->toBeGreaterThan(240)
        ->and($pixel[1])->toBeLessThan(16)
        ->and($pixel[2])->toBeLessThan(16)
        ->and($pixel[3])->toBe(255);

    sws_freeContext($scaler);
    avcodec_free_context($decoder);
    avformat_close_input($context);
});

it('decodes the audio and resamples it to interleaved 16-bit stereo', function (): void {
    $context = openFixture();
    [$decoder, $index] = openDecoder($context, AVMEDIA_TYPE_AUDIO);
    $stereo = new AVChannelLayout();
    av_channel_layout_default($stereo, 2);
    $resampler = null;

    expect($decoder->sample_fmt())->toBe(AV_SAMPLE_FMT_FLTP)
        ->and($decoder->sample_rate())->toBe(48000)
        ->and(swr_alloc_set_opts2($resampler, $stereo, AV_SAMPLE_FMT_S16, 48000, $decoder->ch_layout(), AV_SAMPLE_FMT_FLTP, 48000, 0, 0))->toBe(0)
        ->and(swr_init($resampler))->toBe(0);

    $planes = null;
    $linesize = null;
    av_samples_alloc($planes, $linesize, 2, 4096, AV_SAMPLE_FMT_S16, 0);
    $decoded = 0;
    $converted = 0;
    decodeAll($context, $decoder, $index, function (AVFrame $frame) use ($resampler, $planes, &$decoded, &$converted): void {
        expect($frame->data()[0])->not->toBe(0)->and($frame->data()[1])->not->toBe(0);
        $decoded += $frame->nb_samples();
        $converted += swr_convert($resampler, $planes, 4096, array_slice($frame->data(), 0, 2), $frame->nb_samples());
    });
    $converted += swr_convert($resampler, $planes, 4096, null, 0);

    expect($planes)->toHaveCount(1)
        ->and($linesize)->toBe(4096 * 2 * 2)
        ->and($decoded)->toBeGreaterThanOrEqual(48000)
        ->and($converted)->toBe($decoded)
        ->and(swr_get_delay($resampler, 48000))->toBe(0);

    av_free($planes[0]);
    swr_free($resampler);
    av_channel_layout_uninit($stereo);
    avcodec_free_context($decoder);
    avformat_close_input($context);
    expect($resampler)->toBeNull();
});
