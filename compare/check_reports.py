#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Checks the code in docs/reports: every ```cpp or ```c block that is followed
by a ```text block is compiled, run, and its output compared with that text.

usage: check_reports.py [--update] [report.md ...]
       (default: every docs/reports/*/*.md)

The C++ blocks build against GLM and Eigen, the C blocks against cglm and
../hypatia.h, all from compare/ext (run fetch.sh first).  Exit status 1 if any
block fails to build or prints something else.  --update writes what each block
prints into its text block instead of comparing.
"""
import glob
import os
import re
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
EXT = os.environ.get('COMPARE_EXT', os.path.join(HERE, 'ext'))  # COMPARE_EXT: other library checkouts
# a code block, then (after any prose lines) the text block with what it prints
BLOCK = re.compile(r'```(cpp|c)\n(.*?)```[ \t]*\n+(?:[^`\n][^\n]*\n+)*?```text\n(.*?)```', re.S)

COMPILE = {
    'cpp': ['c++', '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
            '-isystem', EXT + '/glm', '-isystem', EXT + '/eigen'],
    'c': ['cc', '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-I' + ROOT, '-isystem', EXT + '/cglm/include'],
}


def check(path, update=False):
    text = open(path).read()
    blocks = BLOCK.findall(text)
    failures = 0
    outputs = []
    with tempfile.TemporaryDirectory() as tmp:
        for n, (lang, code, expected) in enumerate(blocks):
            src = os.path.join(tmp, f'b{n}.{lang}')
            exe = os.path.join(tmp, f'b{n}')
            open(src, 'w').write(code)
            build = subprocess.run(COMPILE[lang] + [src, '-o', exe, '-lm'], capture_output=True, text=True)
            if build.returncode:
                print(f'{path}: block {n + 1} ({lang}) does not build:\n{build.stderr}')
                failures += 1
                continue
            run = subprocess.run([exe], capture_output=True, text=True)
            outputs.append(run.stdout)
            if update:
                continue
            if run.stdout != expected:
                print(f'{path}: block {n + 1} ({lang}) prints:\n{run.stdout}expected:\n{expected}')
                failures += 1
    if update and not failures:
        it = iter(outputs)
        text = BLOCK.sub(lambda m: m.group(0)[:m.start(3) - m.start(0)] + next(it) + '```', text)
        open(path, 'w').write(text)
    print(f'{os.path.relpath(path, ROOT)}: {len(blocks)} blocks, {"ok" if not failures else str(failures) + " failed"}')
    return failures, len(blocks)


def main():
    args = sys.argv[1:]
    update = '--update' in args
    args = [a for a in args if a != '--update']
    paths = args or sorted(glob.glob(os.path.join(ROOT, 'docs', 'reports', '*', '*.md')))
    paths = [p for p in paths if not p.endswith('README.md')]
    failed = total = 0
    for p in paths:
        f, n = check(p, update)
        failed += f
        total += n
        if n == 0:
            print(f'{p}: no code blocks')
            failed += 1
    print(f'{len(paths)} reports, {total} blocks, {failed} failed')
    sys.exit(1 if failed else 0)


main()
