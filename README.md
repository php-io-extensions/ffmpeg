# ffmpeg

[![CI](https://img.shields.io/github/actions/workflow/status/php-io-extensions/ffmpeg/ci.yml?style=flat-square&label=CI)](https://github.com/php-io-extensions/ffmpeg/actions/workflows/ci.yml)
[![FFmpeg API](https://img.shields.io/badge/FFmpeg_API-4.2%25_%2840%2F955%29-blue?style=flat-square)](SUPPORT.md)

1:1 PHP bindings of FFmpeg's libavformat, libavcodec, libavutil, libswscale and libswresample. Version 0.11.0. Linux and macOS.

The extension builds against FFmpeg 7.1 or newer. It binds only functions that FFmpeg 7.1 and the newest FFmpeg it is built against both declare. Function and constant names are the C names.

## Requirements

- FFmpeg 7.1 or newer: libavformat 61.7, libavcodec 61.19, libavutil 59.39, libswscale 8.3, libswresample 5.3
- PHP 8.4, NTS or ZTS
- macOS: `brew install ffmpeg`
- Debian trixie and Raspberry Pi OS: `apt install libavformat-dev libavcodec-dev libavutil-dev libswscale-dev libswresample-dev`

## Install

macOS, into Homebrew `php@8.4` and `php@8.4-zts`:

```bash
./install-macos.sh
```

Debian trixie and Raspberry Pi OS, into the PHP on `PATH`:

```bash
./install-debian-trixie.sh
```

`php --ri ffmpeg` shows the version and license of each loaded FFmpeg library.

## Basic usage

Open a file, find its video stream and its decoder:

```php
$context = null;
$options = null;
avformat_open_input($context, 'clip.mp4', null, $options) === 0 or exit('cannot open');
avformat_find_stream_info($context, null);

$codec = null;
$index = av_find_best_stream($context, AVMEDIA_TYPE_VIDEO, -1, -1, $codec, 0);
$decoder = avcodec_alloc_context3($codec);
avcodec_parameters_to_context($decoder, $context->streams()[$index]->codecpar());
avcodec_open2($decoder, $codec, $options);
```

Read packets, decode frames, and convert each frame to RGBA into memory given by address (here a 64 × 48 buffer):

```php
$packet = av_packet_alloc();
$frame = av_frame_alloc();
$scaler = sws_getContext($decoder->width(), $decoder->height(), $decoder->pix_fmt(), 64, 48, AV_PIX_FMT_RGBA, SWS_BILINEAR, null, null, null);

while (av_read_frame($context, $packet) >= 0) {
    if ($packet->stream_index() === $index && avcodec_send_packet($decoder, $packet) >= 0) {
        while (avcodec_receive_frame($decoder, $frame) >= 0) {
            sws_scale($scaler, $frame->data(), $frame->linesize(), 0, $frame->height(), [$rgbaAddress], [64 * 4]);
            av_frame_unref($frame);
        }
    }
    av_packet_unref($packet);
}

sws_freeContext($scaler);
av_frame_free($frame);
av_packet_free($packet);
avcodec_free_context($decoder);
avformat_close_input($context);
```

A handle is one PHP object per native pointer. Dropping it does not free the native object: the free functions do, and they set the variable to null. Fields are methods named as the C fields (`$frame->width()`, `$stream->codecpar()`). A plane is an address; `0` is NULL.

## Coverage

[SUPPORT.md](SUPPORT.md) lists every counted function, bound or not. `php84 scripts/support.php` writes it and the badge.

## License

The extension is MIT. FFmpeg is LGPL 2.1 or later, or GPL when it is built with GPL parts; `php --ri ffmpeg` shows which.
