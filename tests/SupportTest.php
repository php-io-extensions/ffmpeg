<?php

declare(strict_types=1);

require_once __DIR__ . '/../scripts/support.php';

it('reads the public header names from a Makefile HEADERS list', function (): void {
    $makefile = "NAME = swscale\n\nHEADERS = swscale.h                                                      \\\n          version.h                                                     \\\n          version_major.h                                               \\\n\nOBJS = alphablend.o\n";

    expect(support_header_names($makefile))->toBe(['swscale.h', 'version.h', 'version_major.h']);
});

it('counts declarations with leading and trailing attributes, and static inline definitions, once each', function (): void {
    $header = "#define AV_X 1\n"
        . "/** av_fake(int) in a comment */\n"
        . "attribute_deprecated\nint avcodec_close(AVCodecContext *avctx);\n"
        . "int64_t av_rescale_q(int64_t a, AVRational bq, AVRational cq) av_const;\n"
        . "void av_log(void *avcl, int level, const char *fmt, ...) av_printf_format(3, 4);\n"
        . "void *av_malloc(size_t size) av_malloc_attrib av_alloc_size(1);\n"
        . "static inline double av_q2d(AVRational a){\n    return a.num / (double) a.den;\n}\n"
        . "static av_always_inline av_const int av_clip_c(int a, int amin, int amax)\n{\n    if (a < amin) return amin;\n    return a;\n}\n"
        . "av_warn_unused_result\nint av_frame_get_buffer(AVFrame *frame, int align);\n";

    expect(support_header_functions($header))
        ->toBe(['av_clip_c', 'av_frame_get_buffer', 'av_log', 'av_malloc', 'av_q2d', 'av_rescale_q', 'avcodec_close']);
});

it('does not count typedefs, function pointer fields, data or enum bodies', function (): void {
    $header = "typedef int (*av_callback)(void *opaque);\n"
        . "typedef struct AVIOInterruptCB {\n    int (*callback)(void*);\n    void *opaque;\n} AVIOInterruptCB;\n"
        . "extern const AVClass av_fake_class;\n"
        . "enum AVMediaType {\n    AVMEDIA_TYPE_VIDEO,\n    AVMEDIA_TYPE_AUDIO,\n};\n"
        . "struct SwsContext *sws_getContext(int srcW, int srcH, enum AVPixelFormat srcFormat, int dstW, int dstH, enum AVPixelFormat dstFormat, int flags, SwsFilter *srcFilter, SwsFilter *dstFilter, const double *param);\n";

    expect(support_header_functions($header))->toBe(['sws_getContext']);
});

it('counts a function only when the floor and the newest headers both declare it', function (): void {
    $floor = ['libavcodec/avcodec.h' => "int avcodec_close(AVCodecContext *avctx);\nint avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);\n"];
    $newest = ['libavcodec/avcodec.h' => "int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options);\nint avcodec_new_in_9(void);\n"];

    expect(support_counted($floor, $newest))->toBe(['libavcodec/avcodec.h' => ['avcodec_open2']]);
});

it('reads the top-level functions of a stub, not class methods', function (): void {
    $stub = "<?php\nfunction av_frame_alloc(): ?AVFrame {}\nfinal class AVFrame { public function pointer(): int {} }\nfunction av_frame_free(AVFrame \$frame): void {}\n";

    expect(support_functions($stub))->toBe(['av_frame_alloc', 'av_frame_free']);
});

it('renders one line per function and lists the bindings that match none', function (): void {
    $md = support_render(['libavutil/frame.h' => ['av_frame_alloc', 'av_frame_free']], ['av_frame_alloc' => true, 'ffmpeg_helper' => true], 'FFmpeg n7.1 ∩ n9.0.2', '2026-11-11');

    expect($md)->toContain('- [x] `av_frame_alloc`')
        ->toContain('- [ ] `av_frame_free`')
        ->toContain('## libavutil/frame.h (1 / 2)')
        ->toContain('FFmpeg n7.1 ∩ n9.0.2 — 1 / 2 members bound (50.0%) — 2026-11-11')
        ->toContain('- `ffmpeg_helper`');
});

it('computes the badge from the counts', function (): void {
    expect(support_badge('FFmpeg API', 34, 955))
        ->toBe('[![FFmpeg API](https://img.shields.io/badge/FFmpeg_API-3.6%25_%2834%2F955%29-blue?style=flat-square)](SUPPORT.md)');
});

it('compares a support list without its date', function (): void {
    expect(support_same("FFmpeg — 2 / 4 members bound (50.0%) — 2026-11-11\n", "FFmpeg — 2 / 4 members bound (50.0%) — 2026-11-12\n"))->toBeTrue()
        ->and(support_same("FFmpeg — 2 / 4 members bound (50.0%) — 2026-11-11\n", "FFmpeg — 3 / 4 members bound (75.0%) — 2026-11-11\n"))->toBeFalse();
});
