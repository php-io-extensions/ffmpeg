<?php

declare(strict_types=1);

it('reports its version and each loaded library at or above the 7.1 floor', function (): void {
    ob_start();
    (new ReflectionExtension('ffmpeg'))->info();
    $info = (string) ob_get_clean();

    $floor = ['libavformat' => '61.7.100', 'libavcodec' => '61.19.100', 'libavutil' => '59.39.100', 'libswscale' => '8.3.100', 'libswresample' => '5.3.100'];
    foreach ($floor as $library => $minimum) {
        expect(preg_match('/^' . $library . ' => (\d+\.\d+\.\d+) \((.+)\)$/m', $info, $m))->toBe(1, $library)
            ->and(version_compare($m[1], $minimum, '>='))->toBeTrue("{$library} {$m[1]} is below {$minimum}")
            ->and($m[2])->not->toBe('');
    }
    expect(phpversion('ffmpeg'))->toBe('0.11.0');
});
