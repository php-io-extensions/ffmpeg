<?php

declare(strict_types=1);

it('opens a file, reads its streams and closes it', function (): void {
    $context = null;
    $options = null;

    expect(avformat_open_input($context, videoFixture(), null, $options))->toBe(0)
        ->and($context)->toBeInstanceOf(AVFormatContext::class)
        ->and($options)->toBeNull()
        ->and(avformat_find_stream_info($context, null))->toBeGreaterThanOrEqual(0)
        ->and($context->nb_streams())->toBe(2)
        ->and($context->iformat()->name())->toContain('mp4')
        ->and($context->duration())->toBeGreaterThanOrEqual(1_000_000)->toBeLessThan(1_100_000);

    $streams = $context->streams();
    expect($streams)->toHaveCount(2)
        ->and($context->streams()[0])->toBe($streams[0])
        ->and($streams[0]->index())->toBe(0)
        ->and($streams[1]->index())->toBe(1);

    avformat_close_input($context);
    expect($context)->toBeNull();
});

it('reports a stream\'s codec parameters and finds its decoder', function (): void {
    $context = openFixture();
    $codec = null;
    $video = av_find_best_stream($context, AVMEDIA_TYPE_VIDEO, -1, -1, $codec, 0);
    $parameters = $context->streams()[$video]->codecpar();

    expect($codec)->toBeInstanceOf(AVCodec::class)
        ->and($codec->name())->toBe('h264')
        ->and($codec->type())->toBe(AVMEDIA_TYPE_VIDEO)
        ->and(avcodec_find_decoder($parameters->codec_id()))->toBe($codec)
        ->and(avcodec_get_name($parameters->codec_id()))->toBe('h264')
        ->and($parameters->codec_type())->toBe(AVMEDIA_TYPE_VIDEO)
        ->and($parameters->width())->toBe(64)
        ->and($parameters->height())->toBe(48)
        ->and($parameters->format())->toBe(AV_PIX_FMT_YUV420P)
        ->and(av_q2d($context->streams()[$video]->avg_frame_rate()))->toBe(25.0);

    $audio = av_find_best_stream($context, AVMEDIA_TYPE_AUDIO, -1, -1, $codec, 0);
    $parameters = $context->streams()[$audio]->codecpar();
    expect($codec->name())->toBe('aac')
        ->and($parameters->sample_rate())->toBe(48000)
        ->and($parameters->format())->toBe(AV_SAMPLE_FMT_FLTP)
        ->and($parameters->ch_layout()->nb_channels)->toBe(2);

    avformat_close_input($context);
});

it('answers FFmpeg\'s error for a file that does not exist and leaves the context null', function (): void {
    $context = null;
    $options = null;
    $result = avformat_open_input($context, '/nonexistent/ext-ffmpeg.mp4', null, $options);
    $text = null;
    av_strerror($result, $text, 64);

    expect($result)->toBe(AVERROR(2))
        ->and($text)->toBe('No such file or directory')
        ->and($context)->toBeNull();
});

it('releases the streams and their parameters with the context', function (): void {
    $context = openFixture();
    $stream = $context->streams()[0];
    $parameters = $stream->codecpar();
    avformat_close_input($context);

    expect($stream->pointer())->toBe(0)
        ->and($parameters->pointer())->toBe(0)
        ->and(fn () => $stream->index())->toThrow(ValueError::class, 'AVStream has been released')
        ->and(fn () => $parameters->width())->toThrow(ValueError::class, 'AVCodecParameters has been released');
});

it('seeks back to the start and reads the first packet again', function (): void {
    $context = openFixture();
    $packet = av_packet_alloc();
    av_read_frame($context, $packet);
    $first = [$packet->stream_index(), $packet->pts()];
    av_packet_unref($packet);
    while (av_read_frame($context, $packet) >= 0) {
        av_packet_unref($packet);
    }

    expect(av_seek_frame($context, -1, 0, AVSEEK_FLAG_BACKWARD))->toBeGreaterThanOrEqual(0)
        ->and(av_read_frame($context, $packet))->toBe(0)
        ->and([$packet->stream_index(), $packet->pts()])->toBe($first)
        ->and($packet->size())->toBeGreaterThan(0);

    av_packet_free($packet);
    avformat_close_input($context);
    expect($packet)->toBeNull();
});
