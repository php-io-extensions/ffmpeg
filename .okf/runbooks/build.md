---
type: Runbook
title: Build, install, test
description: The floor and why, the two installers, CI, Pest, the support list.
resource: install-macos.sh
tags: [build, linux, macos, pest]
status: draft
generated: { by: claude-opus/5.5, at: 2026-11-11T00:00:00Z }
sources:
  - id: m4
    resource: config.m4
    title: config.m4
  - id: mac
    resource: install-macos.sh
    title: install-macos.sh
  - id: debian
    resource: install-debian-trixie.sh
    title: install-debian-trixie.sh
  - id: ci
    resource: .github/workflows/ci.yml
    title: CI
  - id: support
    resource: scripts/support.php
    title: Support list
---

# Overview

The floor is FFmpeg 7.1, the version Debian trixie ships. `config.m4` asks pkg-config for its five library versions: libavformat ≥ 61.7.100, libavcodec ≥ 61.19.100, libavutil ≥ 59.39.100, libswscale ≥ 8.3.100, libswresample ≥ 5.3.100.[^m4] Homebrew ships a newer FFmpeg (9.0.2 on 2026-11-10). FFmpeg removes deprecated functions between majors, so a binding builds on both only when 7.1 and the newest version both declare its function.

# Mac

`brew install ffmpeg`, then `./install-macos.sh`. It builds a disposable copy for `/opt/homebrew/opt/php@8.4/bin/php` and `php@8.4-zts`, installs `ffmpeg.so`, ad-hoc signs it, and writes `30-ffmpeg.ini`.[^mac]

```bash
composer install
php84 vendor/bin/pest
zhp vendor/bin/pest
```

`vendor/` and `composer.lock` are not committed.

# Debian and Raspberry Pi OS

`apt install libavformat-dev libavcodec-dev libavutil-dev libswscale-dev libswresample-dev`, then `./install-debian-trixie.sh`. It builds in place, runs `gen_stub.php`, installs into the PHP on `PATH`, enables it for CLI, FPM and Apache, and removes the build products.[^debian]

# CI

macOS 15 with Homebrew FFmpeg on PHP 8.4 and 8.5, NTS and ZTS. Linux runs in a `debian:trixie` container with distro PHP 8.4 NTS: Ubuntu 24.04 ships FFmpeg 6.1, below the floor.[^ci]

# Support list

`php84 scripts/support.php` writes `SUPPORT.md` and the README badge; `--check` exits 1 when either is stale. It reads the `HEADERS` list of each library's Makefile at tags `n7.1` and `n9.0.2`, downloads those headers once into `scripts/.support-cache/` (gitignored), and counts every file-scope function declaration or static inline definition that both tags have: 955 on 2026-11-11. When Homebrew moves past 9.0.2, raise `SUPPORT_NEWEST`.[^support]

[^m4]: config.m4
[^mac]: install-macos.sh
[^debian]: install-debian-trixie.sh
[^ci]: CI
[^support]: Support list
