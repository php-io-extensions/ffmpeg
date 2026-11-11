#!/bin/bash

# Builds ext-ffmpeg in a disposable copy and installs it into each PHP given.
#
#   ./install-macos.sh                      # Homebrew php@8.4 (NTS) and php@8.4-zts
#   ./install-macos.sh /path/to/bin/php ... # specific PHP binaries
#
# For each PHP: phpize/configure/make against that PHP's php-config, copy the
# .so into its extension_dir, re-sign it ad hoc so amfid accepts it on first
# load, and enable it with 30-ffmpeg.ini in its conf.d scan dir.
#
#   FFMPEG_CONFIGURE="--enable-ffmpeg-probes" ./install-macos.sh   # a probe build (tests/Probes); never ship one

set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SOURCES=(config.m4 php_ffmpeg.h src stubs)
INI_NAME="30-ffmpeg.ini"
# The floor: FFmpeg 7.1's library versions.
FFMPEG_MODULES='libavformat >= 61.7.100 libavcodec >= 61.19.100 libavutil >= 59.39.100 libswscale >= 8.3.100 libswresample >= 5.3.100'

die() { printf '✖ %s\n' "$*" >&2; exit 1; }
step() { printf '▶ %s\n' "$*"; }

[[ "$(uname -s)" == "Darwin" ]] || die "This installer targets macOS; on Debian or Raspberry Pi OS use install-debian-trixie.sh."
pkg-config --exists "$FFMPEG_MODULES" || die "FFmpeg 7.1 or newer not found — install: brew install ffmpeg"

if [[ $# -gt 0 ]]; then
    PHP_BINS=("$@")
else
    PHP_BINS=(/opt/homebrew/opt/php@8.4/bin/php /opt/homebrew/opt/php@8.4-zts/bin/php)
fi

BUILD_DIR=""
cleanup() { if [[ -n "$BUILD_DIR" ]]; then rm -rf "$BUILD_DIR"; fi; }
trap cleanup EXIT

for PHP_BIN in "${PHP_BINS[@]}"; do
    BIN_DIR="$(dirname "$PHP_BIN")"
    PHPIZE="${BIN_DIR}/phpize"
    PHP_CONFIG="${BIN_DIR}/php-config"
    for tool in "$PHP_BIN" "$PHPIZE" "$PHP_CONFIG"; do
        [[ -x "$tool" ]] || die "$tool not found or not executable"
    done

    step "Building for $("$PHP_BIN" -r 'echo PHP_VERSION, PHP_ZTS ? " ZTS" : " NTS";') (${PHP_BIN})"
    BUILD_DIR="$(mktemp -d "${TMPDIR:-/tmp}/ffmpeg-build.XXXXXX")"
    for f in "${SOURCES[@]}"; do
        cp -R "${SCRIPT_DIR}/${f}" "${BUILD_DIR}/"
    done
    if ! (cd "$BUILD_DIR" \
        && "$PHPIZE" \
        && ./configure --enable-ffmpeg ${FFMPEG_CONFIGURE:-} --with-php-config="$PHP_CONFIG" \
        && make -j"$(sysctl -n hw.ncpu)") >"${BUILD_DIR}/build.log" 2>&1; then
        tail -40 "${BUILD_DIR}/build.log" >&2
        die "Build failed for ${PHP_BIN}"
    fi

    EXT_DIR="$("$PHP_BIN" -r 'echo ini_get("extension_dir");')"
    SCAN_DIR="$("$PHP_BIN" -r 'echo PHP_CONFIG_FILE_SCAN_DIR;')"
    [[ -d "$EXT_DIR" ]] || die "extension_dir ${EXT_DIR} does not exist"
    [[ -n "$SCAN_DIR" && -d "$SCAN_DIR" ]] || die "conf.d scan dir '${SCAN_DIR}' does not exist"

    install -m 0755 "${BUILD_DIR}/modules/ffmpeg.so" "${EXT_DIR}/ffmpeg.so"
    codesign --force --sign - "${EXT_DIR}/ffmpeg.so" >/dev/null 2>&1
    printf 'extension=ffmpeg\n' > "${SCAN_DIR}/${INI_NAME}"

    "$PHP_BIN" -r 'exit(extension_loaded("ffmpeg") ? 0 : 1);' || die "ffmpeg did not load in ${PHP_BIN}"
    step "Installed ${EXT_DIR}/ffmpeg.so, enabled by ${SCAN_DIR}/${INI_NAME}"

    rm -rf "$BUILD_DIR"
    BUILD_DIR=""
done
