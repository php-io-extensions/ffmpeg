<?php

declare(strict_types=1);

/*
 * The support list: every function FFmpeg's five bound libraries (libavformat, libavcodec,
 * libavutil, libswscale, libswresample) declare in their public headers at both the floor tag
 * (n7.1, the oldest FFmpeg this extension builds against: Debian trixie) and the newest tag it is
 * built against (n9.0.2: Homebrew), marked bound when a stub declares the PHP function of the same name.
 * A function only one of the two declares cannot be bound and still build on both, so it is not counted.
 *
 *   php84 scripts/support.php           writes SUPPORT.md and the README badge
 *   php84 scripts/support.php --check   exits 1 when either differs (the date is not compared)
 *
 * The public headers are the HEADERS list of each library's Makefile at the tag (what
 * `make install-headers` installs), cached under scripts/.support-cache/. A member is a top-level
 * declaration or static inline definition of a function, outside every typedef and struct.
 */

const SUPPORT_LABEL = 'FFmpeg API';
const SUPPORT_FLOOR = 'n7.1';
const SUPPORT_NEWEST = 'n9.0.2';
const SUPPORT_LIBRARIES = ['libavformat', 'libavcodec', 'libavutil', 'libswscale', 'libswresample'];
const SUPPORT_URL = 'https://raw.githubusercontent.com/FFmpeg/FFmpeg/';

/** FFmpeg's attribute macros: never a function's name. */
const SUPPORT_ATTRIBUTES = [
    'attribute_deprecated', 'av_const', 'av_pure', 'av_cold', 'av_noreturn', 'av_unused', 'av_used', 'av_flatten',
    'av_always_inline', 'av_noinline', 'av_malloc_attrib', 'av_alloc_size', 'av_warn_unused_result',
    'av_printf_format', 'av_scanf_format', 'av_builtin_constant_p', 'av_extern_inline',
];

/** The public header names of one library at a tag, from its Makefile's HEADERS list. @return list<string> */
function support_header_names(string $makefile): array
{
    if (! preg_match('/^HEADERS\s*=\s*((?:[^\n]*\\\\\n)*[^\n]*)/m', $makefile, $m)) {
        throw new RuntimeException('A Makefile without a HEADERS list.');
    }
    $names = array_values(array_filter(preg_split('/\s+/', str_replace("\\\n", ' ', $m[1])), static fn (string $w): bool => str_ends_with($w, '.h')));
    sort($names, SORT_STRING);

    return $names;
}

/** C source without comments, preprocessor lines and string literals. */
function support_code(string $source): string
{
    $source = preg_replace('~/\*.*?\*/~s', ' ', $source);
    $source = preg_replace('~//[^\n]*~', ' ', $source);
    $source = preg_replace('/\\\\\n/', ' ', $source);
    $source = preg_replace('/^\s*#[^\n]*/m', ' ', $source);

    return preg_replace('/"(?:[^"\\\\]|\\\\.)*"/', '""', $source);
}

/**
 * The functions one header declares or defines at file scope.
 * @return list<string>
 */
function support_header_functions(string $source): array
{
    $code = support_code($source);
    $names = [];
    $depth = 0;
    $statement = '';
    $length = strlen($code);
    for ($i = 0; $i < $length; $i++) {
        $c = $code[$i];
        if ($c === '{') {
            if ($depth === 0) {
                // A static inline definition: its head is a function; a struct, union or enum body is not.
                $name = support_declared($statement);
                if ($name !== null) {
                    $names[] = $name;
                }
                $statement = $name === null ? $statement . ' {}' : '';
            }
            $depth++;
        } elseif ($c === '}') {
            $depth--;
            if ($depth === 0 && $statement === '') {
                $statement = ';';
            }
        } elseif ($depth === 0) {
            if ($c === ';') {
                $name = support_declared($statement);
                if ($name !== null) {
                    $names[] = $name;
                }
                $statement = '';
            } else {
                $statement .= $c;
            }
        }
    }
    $names = array_values(array_unique($names));
    sort($names, SORT_STRING);

    return $names;
}

/** The function a file-scope statement declares, or null for a typedef, a variable or a type body. */
function support_declared(string $statement): ?string
{
    $statement = trim(ltrim(trim($statement), ';'));
    if ($statement === '' || str_contains($statement, '{}') || preg_match('/^\s*(?:typedef|extern\s+const|extern\s+"")\b/', $statement)) {
        return null;
    }
    preg_match_all('/\b([A-Za-z_]\w*)\s*\(/', $statement, $m, PREG_OFFSET_CAPTURE);
    foreach ($m[1] as [$name, $offset]) {
        if (in_array($name, SUPPORT_ATTRIBUTES, true) || in_array($name, ['__attribute__', 'sizeof'], true) || ctype_upper(str_replace('_', '', $name))) {
            continue;
        }
        // A return type precedes the name: a bare call-like token at the start is no declaration.
        $head = trim(substr($statement, 0, $offset));

        return $head === '' || str_contains($head, '=') ? null : $name;
    }

    return null;
}

/**
 * @param array<string, string> $floor library/header.h => source at the floor tag
 * @param array<string, string> $newest library/header.h => source at the newest tag
 * @return array<string, list<string>> library/header.h => sorted function names declared at both tags
 */
function support_counted(array $floor, array $newest): array
{
    $newestNames = [];
    foreach ($newest as $source) {
        foreach (support_header_functions($source) as $name) {
            $newestNames[$name] = true;
        }
    }
    $counted = [];
    ksort($floor, SORT_STRING);
    foreach ($floor as $header => $source) {
        $names = array_values(array_filter(support_header_functions($source), static fn (string $n): bool => isset($newestNames[$n])));
        if ($names !== []) {
            $counted[$header] = $names;
        }
    }

    return $counted;
}

/** The free functions the stubs declare. @return array<string, true> */
function support_bound(string $stubDir): array
{
    $bound = [];
    foreach (glob(rtrim($stubDir, '/') . '/*.stub.php') ?: [] as $file) {
        foreach (support_functions(file_get_contents($file)) as $function) {
            $bound[$function] = true;
        }
    }

    return $bound;
}

/**
 * The top-level functions of one stub, without class methods.
 * @return list<string>
 */
function support_functions(string $source): array
{
    $tokens = token_get_all($source);
    $out = [];
    $depth = 0;
    foreach ($tokens as $i => $token) {
        if ($token === '{' || (is_array($token) && in_array($token[0], [T_CURLY_OPEN, T_DOLLAR_OPEN_CURLY_BRACES], true))) {
            $depth++;
        } elseif ($token === '}') {
            $depth--;
        } elseif (is_array($token) && $token[0] === T_FUNCTION && $depth === 0) {
            for ($j = $i + 1; is_array($tokens[$j]) && $tokens[$j][0] === T_WHITESPACE; $j++) {
            }
            if (is_array($tokens[$j]) && $tokens[$j][0] === T_STRING) {
                $out[] = $tokens[$j][1];
            }
        }
    }

    return array_values(array_unique($out));
}

/**
 * @param array<string, list<string>> $counted
 * @param array<string, true> $bound
 */
function support_render(array $counted, array $bound, string $library, string $date): string
{
    $sections = [];
    $hits = 0;
    $total = 0;
    $countedKeys = [];
    foreach ($counted as $owner => $keys) {
        $lines = [];
        $ownerHits = 0;
        foreach ($keys as $key) {
            $countedKeys[$key] = true;
            $isBound = isset($bound[$key]);
            $ownerHits += $isBound ? 1 : 0;
            $lines[] = '- [' . ($isBound ? 'x' : ' ') . "] `{$key}`";
        }
        $hits += $ownerHits;
        $total += count($keys);
        $sections[] = "## {$owner} ({$ownerHits} / " . count($keys) . ")\n\n" . implode("\n", $lines) . "\n";
    }
    $outside = array_map(static fn (string $f): string => "- `{$f}`", array_keys(array_diff_key($bound, $countedKeys)));
    sort($outside, SORT_STRING);

    return '# ' . SUPPORT_LABEL . " support\n\n"
        . support_summary($library, $hits, $total, $date) . "\n\n"
        . 'Generated by `php84 scripts/support.php` from the public headers of FFmpeg ' . SUPPORT_FLOOR . ' and ' . SUPPORT_NEWEST
        . " and `stubs/`. A function counts when both declare it. One line per function: `[x]` bound, `[ ]` not bound.\n\n"
        . implode("\n", $sections)
        . "\n## Bound outside the measured library\n\n"
        . "Functions the stubs declare that the counted headers do not; none of them is counted.\n\n"
        . (count($outside) === 0 ? "None.\n" : implode("\n", $outside) . "\n");
}

function support_summary(string $library, int $hits, int $total, string $date): string
{
    return sprintf('%s — %d / %d members bound (%.1f%%) — %s', $library, $hits, $total, $total === 0 ? 0 : 100 * $hits / $total, $date);
}

function support_badge(string $label, int $bound, int $total): string
{
    $percent = sprintf('%.1f', $total === 0 ? 0 : 100 * $bound / $total);
    $message = str_replace('%20', '_', rawurlencode("{$percent}% ({$bound}/{$total})"));

    return "[![{$label}](https://img.shields.io/badge/" . str_replace(' ', '_', $label) . "-{$message}-blue?style=flat-square)](SUPPORT.md)";
}

/** Two support lists alike but for the date of their summary line. */
function support_same(string $a, string $b): bool
{
    $undated = static fn (string $s): string => preg_replace('/ — \d{4}-\d{2}-\d{2}$/m', '', $s);

    return $undated($a) === $undated($b);
}

/** The README with the badge line set: replaced where it is, else after the CI badge. */
function support_readme(string $readme, string $badge): string
{
    $lines = explode("\n", $readme);
    foreach ($lines as $i => $line) {
        if (str_starts_with($line, '[![' . SUPPORT_LABEL . ']')) {
            $lines[$i] = $badge;

            return implode("\n", $lines);
        }
    }
    foreach ($lines as $i => $line) {
        if (str_starts_with($line, '[![CI]')) {
            array_splice($lines, $i + 1, 0, [$badge]);

            return implode("\n", $lines);
        }
    }
    throw new RuntimeException('The README has no CI badge line to put the API badge after.');
}

/** @return array{int, int} bound and total, read back from a support list's summary line */
function support_totals(string $md): array
{
    preg_match('/ (\d+) \/ (\d+) members bound/', $md, $m);

    return [(int) $m[1], (int) $m[2]];
}

/** A pinned upstream file, downloaded once into the cache. */
function support_fetch(string $url, string $cache): string
{
    if (! is_file($cache)) {
        $context = stream_context_create(['http' => ['header' => "User-Agent: php-io-extensions-support\r\n"]]);
        $body = @file_get_contents($url, false, $context);
        if ($body === false) {
            throw new RuntimeException("Cannot download {$url}.");
        }
        @mkdir(dirname($cache), 0777, true);
        file_put_contents($cache, $body);
    }

    return file_get_contents($cache);
}

/** @return array<string, string> library/header.h => source, the public headers of the bound libraries at $tag */
function support_headers(string $tag, string $cache): array
{
    $headers = [];
    foreach (SUPPORT_LIBRARIES as $library) {
        $makefile = support_fetch(SUPPORT_URL . "{$tag}/{$library}/Makefile", "{$cache}/{$tag}/{$library}/Makefile");
        foreach (support_header_names($makefile) as $name) {
            $headers["{$library}/{$name}"] = support_fetch(SUPPORT_URL . "{$tag}/{$library}/{$name}", "{$cache}/{$tag}/{$library}/{$name}");
        }
    }

    return $headers;
}

function support_main(array $argv): int
{
    $root = dirname(__DIR__);
    $cache = "{$root}/scripts/.support-cache";
    try {
        $counted = support_counted(support_headers(SUPPORT_FLOOR, $cache), support_headers(SUPPORT_NEWEST, $cache));
    } catch (RuntimeException $e) {
        fwrite(STDERR, $e->getMessage() . "\n");

        return 2;
    }
    $md = support_render($counted, support_bound("{$root}/stubs"), 'FFmpeg ' . SUPPORT_FLOOR . ' ∩ ' . SUPPORT_NEWEST, date('Y-m-d'));
    [$hits, $total] = support_totals($md);
    $readme = support_readme(file_get_contents("{$root}/README.md"), support_badge(SUPPORT_LABEL, $hits, $total));

    if (in_array('--check', $argv, true)) {
        $current = is_file("{$root}/SUPPORT.md") ? file_get_contents("{$root}/SUPPORT.md") : '';
        $same = support_same($current, $md) && $readme === file_get_contents("{$root}/README.md");
        fwrite(STDOUT, $same ? "SUPPORT.md and the badge are current.\n" : "SUPPORT.md or the badge differs: run php84 scripts/support.php.\n");

        return $same ? 0 : 1;
    }
    file_put_contents("{$root}/SUPPORT.md", $md);
    file_put_contents("{$root}/README.md", $readme);
    fwrite(STDOUT, "{$hits} / {$total}\n");

    return 0;
}

if (PHP_SAPI === 'cli' && isset($argv[0]) && realpath($argv[0]) === __FILE__) {
    exit(support_main($argv));
}
