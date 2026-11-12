---
type: API
title: Bindings
description: The decode path bound 1:1 — demux, decode, scale, resample — with the handle and value classes and their fields.
resource: stubs/libavformat.stub.php
tags: [ffmpeg, api, decode]
status: draft
generated: { by: claude-opus/5.5, at: 2026-11-11T17:00:00Z }
sources:
  - id: stubs
    resource: stubs/libavformat.stub.php
    title: The five stubs
  - id: decode
    resource: tests/DecodeTest.php
    title: Video and audio decode
  - id: demux
    resource: tests/DemuxTest.php
    title: Open, streams, seek, close
---

# Overview

42 of FFmpeg's functions and the macro `AVERROR`, one stub per library. Each function is declared in both FFmpeg 7.1 and 9.0.2.[^stubs]

| Library | Functions |
|---|---|
| libavformat | `avformat_open_input`, `avformat_find_stream_info`, `av_find_best_stream`, `av_read_frame`, `av_seek_frame`, `avformat_close_input` |
| libavcodec | `avcodec_find_decoder`, `avcodec_get_name`, `avcodec_alloc_context3`, `avcodec_parameters_to_context`, `avcodec_open2`, `avcodec_send_packet`, `avcodec_receive_frame`, `avcodec_flush_buffers`, `avcodec_free_context`, `av_packet_alloc`, `av_packet_unref`, `av_packet_free` |
| libavutil | `av_dict_set`, `av_dict_free`, `av_frame_alloc`, `av_frame_unref`, `av_frame_free`, `av_strerror`, `AVERROR` (macro), `av_q2d`, `av_samples_alloc`, `av_get_bytes_per_sample`, `av_sample_fmt_is_planar`, `av_get_sample_fmt_name`, `av_get_pix_fmt_name`, `av_free`, `av_channel_layout_default`, `av_channel_layout_uninit` |
| libswscale | `sws_getContext`, `sws_scale`, `sws_freeContext` |
| libswresample | `swr_alloc_set_opts2`, `swr_init`, `swr_convert`, `swr_get_delay`, `swr_get_out_samples`, `swr_free` |

Constants: `AV_NOPTS_VALUE`, `AV_TIME_BASE`, 5 `AV_DICT_*` (not the two `DONT_STRDUP` flags: the strings are PHP's, so `av_dict_set` never hands FFmpeg ownership and clears those bits), 27 `AVERROR_*`, every member of `AVMediaType`, `AVSampleFormat`, `AVPixelFormat` and `AVChannelOrder` that both versions declare (not the `_NB` counts, whose value changes), 4 `AVSEEK_FLAG_*`, 18 `SWS_*`.

# Handles and fields

| Class | Fields (getters) | Lifetime |
|---|---|---|
| `AVFormatContext` | `nb_streams`, `streams`, `iformat`, `duration`, `start_time`, `bit_rate` | `avformat_close_input` |
| `AVStream` | `index`, `id`, `time_base`, `start_time`, `duration`, `nb_frames`, `avg_frame_rate`, `codecpar` | released with its context |
| `AVCodecParameters` | `codec_type`, `codec_id`, `format`, `width`, `height`, `sample_rate`, `ch_layout` | released with its stream |
| `AVCodec`, `AVInputFormat` | `name`, `long_name`, (`type`, `id`) | static: never freed |
| `AVCodecContext` | `codec_type`, `codec_id`, `width`, `height`, `pix_fmt`, `sample_fmt`, `sample_rate`, `ch_layout`, `time_base` | `avcodec_free_context` |
| `AVPacket` | `stream_index`, `pts`, `dts`, `duration`, `size`, `flags` | `av_packet_free` |
| `AVFrame` | `width`, `height`, `format`, `data` (8 addresses), `linesize` (8), `nb_samples`, `sample_rate`, `ch_layout`, `pts`, `best_effort_timestamp`, `duration`, `time_base` | `av_frame_free` |
| `SwsContext`, `SwrContext`, `SwsFilter`, `AVDictionary` | — | their free functions |

A getter on a released handle throws `ValueError` ("AVFrame has been released"): site `ffmpeg-field-released`. `AVRational` (`num`, `den`) and `AVChannelLayout` (`order`, `nb_channels`, `u`, `opaque`) are value classes.

# Measured

Mac, FFmpeg 9.0.2, PHP 8.4 NTS and ZTS: 26 passed, the probe skipped on a release build; the probe passes on a probe build. A 1 s 64 × 48 H.264 fixture decodes to 25 `yuv420p` frames; `sws_scale` into an ext-fb buffer's `pointer()` gives red RGBA pixels; its 48 kHz stereo AAC (`fltp`) converts to `s16` with as many samples out as in.[^decode] `avformat_open_input` on a missing file answers `AVERROR(2)` ("No such file or directory") and leaves the context null; closing a context releases its streams and their parameters.[^demux]

[^stubs]: The five stubs
[^decode]: Video and audio decode
[^demux]: Open, streams, seek, close
