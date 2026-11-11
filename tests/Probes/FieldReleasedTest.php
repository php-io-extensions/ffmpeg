<?php

declare(strict_types=1);

// Site ffmpeg-field-released (outcome 1): the handle classes' field getters read the struct themselves, not
// through an FFmpeg call. A freed object's pointer is NULL, which the getter would dereference.

beforeEach(fn () => requireProbeBuild());

it('refuses a field of a freed frame, which the getter reads through NULL (SIGSEGV) without the refusal', function (): void {
    $code = <<<'PHP'
        $frame = av_frame_alloc();
        $copy = $frame;
        av_frame_free($frame);
        $copy->width();
    PHP;

    $kept = ffmpegProbe($code, null);
    $off = ffmpegProbe($code, 'ffmpeg-field-released');

    expect($kept['signal'])->toBeNull()
        ->and($kept['stdout'])->toContain('REFUSED ValueError: AVFrame has been released')
        ->and($off['signal'])->toBeIn([PROBE_SIGSEGV, PROBE_SIGBUS]);
});
