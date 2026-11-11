<?php

declare(strict_types=1);

if (! extension_loaded('ffmpeg')) {
    throw new RuntimeException('The ffmpeg extension is not loaded; run ./install-macos.sh or ./install-debian-trixie.sh');
}

/**
 * 1 s of 64 × 48 solid red H.264 (yuv420p, 25 frames) and a 440 Hz stereo AAC sine (fltp, 48 kHz), made with
 * `ffmpeg -f lavfi -i color=c=red:s=64x48:r=25:d=1 -f lavfi -i sine=frequency=440:sample_rate=48000:duration=1
 * -ac 2 -c:v libx264 -pix_fmt yuv420p -preset veryslow -c:a aac -b:a 32k -shortest -movflags +faststart`.
 * The repository keeps it as base64 text; the first call writes the file to the temp directory.
 */
function videoFixture(): string
{
    static $path = null;
    if ($path === null) {
        $bytes = base64_decode((string) file_get_contents(__DIR__.'/Fixtures/red-64x48-1s.mp4.b64'), true);
        hash('sha256', (string) $bytes) === '4b5d2ad592d73b90fcbfe7766e0622e3f14049441c26c9579f9ee1a8ec88109b' || throw new RuntimeException('tests/Fixtures/red-64x48-1s.mp4.b64 does not decode to the fixture');
        $path = sys_get_temp_dir().'/ext-ffmpeg-red-64x48-1s.mp4';
        file_put_contents($path, $bytes);
    }

    return $path;
}

/** The fixture opened, its streams read. */
function openFixture(): AVFormatContext
{
    $context = null;
    $options = null;
    avformat_open_input($context, videoFixture(), null, $options) === 0 || throw new RuntimeException('avformat_open_input failed');
    avformat_find_stream_info($context, null) >= 0 || throw new RuntimeException('avformat_find_stream_info failed');

    return $context;
}

/** An opened decoder for the fixture's best stream of $type, and that stream's index. @return array{AVCodecContext, int} */
function openDecoder(AVFormatContext $context, int $type): array
{
    $codec = null;
    $index = av_find_best_stream($context, $type, -1, -1, $codec, 0);
    $index >= 0 || throw new RuntimeException('av_find_best_stream failed');
    $decoder = avcodec_alloc_context3($codec);
    avcodec_parameters_to_context($decoder, $context->streams()[$index]->codecpar()) >= 0 || throw new RuntimeException('avcodec_parameters_to_context failed');
    $options = null;
    avcodec_open2($decoder, $codec, $options) === 0 || throw new RuntimeException('avcodec_open2 failed');

    return [$decoder, $index];
}

/**
 * Every frame of stream $index, through the send/receive loop and a final flush. $each gets each frame
 * while it is valid. Answers the frame count.
 *
 * @param  Closure(AVFrame): void  $each
 */
function decodeAll(AVFormatContext $context, AVCodecContext $decoder, int $index, Closure $each): int
{
    $packet = av_packet_alloc();
    $frame = av_frame_alloc();
    $count = 0;
    $drain = function () use ($decoder, $frame, $each, &$count): void {
        while (($result = avcodec_receive_frame($decoder, $frame)) >= 0) {
            $each($frame);
            $count++;
            av_frame_unref($frame);
        }
        $result === AVERROR(EAGAIN_CODE) || $result === AVERROR_EOF || throw new RuntimeException('avcodec_receive_frame: '.$result);
    };
    while (av_read_frame($context, $packet) >= 0) {
        if ($packet->stream_index() === $index) {
            avcodec_send_packet($decoder, $packet) >= 0 || throw new RuntimeException('avcodec_send_packet failed');
            $drain();
        }
        av_packet_unref($packet);
    }
    avcodec_send_packet($decoder, null);
    $drain();
    av_packet_free($packet);
    av_frame_free($frame);

    return $count;
}

/** EAGAIN on this platform: 35 on macOS, 11 on Linux. */
define('EAGAIN_CODE', PHP_OS_FAMILY === 'Darwin' ? 35 : 11);

/** macOS signal numbers a kept refusal's probe dies by. */
const PROBE_SIGBUS = 10;
const PROBE_SIGSEGV = 11;

/** Skips a tests/Probes test unless the loaded ext-ffmpeg is a probe build (--enable-ffmpeg-probes). */
function requireProbeBuild(): void
{
    if (! defined('FFMPEG_PROBES')) {
        test()->markTestSkipped('ext-ffmpeg is not a probe build (configure --enable-ffmpeg-probes): FFMPEG_PROBES is undefined');
    }
}

/**
 * Runs $code in a child of this PHP with only ext-ffmpeg loaded (from FFMPEG_SO when set). $siteOff names
 * the kept site FFMPEG_PROBE_OFF turns off, null keeps every site.
 *
 * @return array{signal: ?int, stdout: string}
 */
function ffmpegProbe(string $code, ?string $siteOff): array
{
    $script = "<?php\ndeclare(strict_types=1);\ntry {\n".$code."\n} catch (Throwable \$t) {\n    echo 'REFUSED ', get_class(\$t), ': ', \$t->getMessage(), \"\\n\";\n}\necho \"SURVIVED\\n\";\n";
    $file = tempnam(sys_get_temp_dir(), 'ffmpeg-probe');
    file_put_contents($file, $script);

    $env = getenv();
    unset($env['FFMPEG_PROBE_OFF']);
    $env['XDEBUG_MODE'] = 'off';
    if ($siteOff !== null) {
        $env['FFMPEG_PROBE_OFF'] = $siteOff;
    }

    $so = getenv('FFMPEG_SO') ?: ini_get('extension_dir').'/ffmpeg.so';
    $command = [PHP_BINARY, '-n', '-d', 'memory_limit=128M', '-d', 'extension='.$so, $file];
    $process = proc_open($command, [0 => ['file', '/dev/null', 'r'], 1 => ['pipe', 'w'], 2 => ['file', '/dev/null', 'w']], $pipes, null, $env);
    $stdout = (string) stream_get_contents($pipes[1]);
    fclose($pipes[1]);
    do {
        $status = proc_get_status($process);
        if ($status['running']) {
            usleep(20_000);
        }
    } while ($status['running']);
    proc_close($process);
    @unlink($file);

    return ['signal' => $status['signaled'] ? $status['termsig'] : null, 'stdout' => $stdout];
}
