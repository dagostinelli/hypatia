#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Summarizes the precision results: every count and table in
docs/comparison.md comes from this script's output.

usage: summarize.py RESULTS_DIR > summary.md

RESULTS_DIR holds {master,before,now}.{double,single}[.s7].md, the output of the
precision-only programs (see reproduce.sh).

Ranking rule: for each measurement, a version of hypatia is "best or tied" when
its mean error is at most 2% above the smallest mean of GLM, Eigen and cglm,
"ahead" when it is more than 2% below it, and "behind" when it is more than 2%
above it.  Measurements where the version or no other library has the function
are left out of that version's counts.  Errors of 1e6 ulps or more are counted
separately: fewer than 10 correct digits in double, none in single.
"""
import os
import sys

WRONG = 1e6
TIE = 0.02


def table(path):
    """the precision rows of a result file: [function, inputs, hypatia, GLM, Eigen, cglm]"""
    text = open(path).read()
    text = text[text.index('## Precision against'):]
    rows = []
    for line in text.splitlines():
        if line.startswith('### Oracle check'):
            break
        if line.startswith('| ') and not line.startswith('| function'):
            rows.append([c.strip().replace('**', '') for c in line.strip().strip('|').split('|')])
    return rows


def oracle(path):
    text = open(path).read()
    text = text[text.index('### Oracle check'):]
    return [l for l in text.splitlines() if l.startswith('| ') and not l.startswith('| reference')]


def largest(cell):
    if not cell:
        return None
    return float('inf') if cell == 'inf' else float(cell.split(' / ')[0])


def mean(cell):
    if not cell:
        return None
    return float('inf') if cell == 'inf' else float(cell.split(' / ')[1])


def libs(precision):
    return ['GLM', 'Eigen', 'cglm'] if precision == 'single' else ['GLM', 'Eigen']


def others(row, precision):
    return [(lib, cell) for lib, cell in zip(libs(precision), row[3:3 + len(libs(precision))]) if cell]


def counts(rows, now_rows, precision):
    best = ahead = behind = total = wrong = 0
    for r, n in zip(rows, now_rows):
        o = others(n, precision)
        h = mean(r[2])
        if not o or h is None:
            continue
        if largest(r[2]) >= WRONG:
            wrong += 1
        b = min(mean(c) for _, c in o)
        total += 1
        if h <= b * (1 + TIE):
            best += 1
        if h < b * (1 - TIE):
            ahead += 1
        if h > b * (1 + TIE):
            behind += 1
    return best, ahead, behind, wrong, total


def main():
    d = sys.argv[1]
    seeds = [('', 'default seed')]
    if os.path.exists(os.path.join(d, 'now.double.s7.md')):
        seeds.append(('.s7', 'COMPARE_SEED=7'))
    out = []

    # the same inputs for every version: the other libraries' results must match
    for seed, name in seeds:
        for p in ['double', 'single']:
            now = table(os.path.join(d, f'now.{p}{seed}.md'))
            for v in ['master', 'before']:
                rows = table(os.path.join(d, f'{v}.{p}{seed}.md'))
                for r, n in zip(rows, now):
                    assert r[3:] == n[3:], f'{v}.{p}{seed}: {r[0]} {r[1]}: other libraries differ from now; the inputs differ'
    out.append('The results of GLM, Eigen and cglm are identical, digit for digit, in the runs against master, before and now, for both seeds: every version of hypatia was measured on the same inputs.\n')

    out.append('## Counts\n')
    out.append('Mean error within 2% of the best of the other libraries ("best or tied"), more than 2% below it ("ahead"), more than 2% above it ("behind").  ">= 1e6": largest error 1e6 ulps or more (counted in "behind" too).\n')
    out.append('| seed | precision | version | best or tied | ahead | behind | >= 1e6 | measurements |')
    out.append('|---|---|---|---|---|---|---|---|')
    for seed, name in seeds:
        for p in ['double', 'single']:
            now = table(os.path.join(d, f'now.{p}{seed}.md'))
            for v in ['master', 'before', 'now']:
                rows = table(os.path.join(d, f'{v}.{p}{seed}.md'))
                assert [r[:2] for r in rows] == [r[:2] for r in now], f'{v}.{p}{seed}: different measurements'
                b, a, w, x, t = counts(rows, now, p)
                out.append(f'| {name} | {p} | {v} | {b} | {a} | {w} | {x} | {t} |')

    for p in ['double', 'single']:
        now = table(os.path.join(d, f'now.{p}.md'))
        now7 = table(os.path.join(d, f'now.{p}.s7.md')) if len(seeds) > 1 else now

        def ratio(r):
            """hypatia's mean over the best other mean, and the best other"""
            o = others(r, p)
            h = mean(r[2])
            if not o or h is None:
                return None, None
            lib, cell = min(o, key=lambda t: mean(t[1]))
            return (h / mean(cell) if mean(cell) > 0 else float('inf')), (lib, cell)
        ahead, behind = [], []
        for r, r7 in zip(now, now7):
            x, best = ratio(r)
            if x is None:
                continue
            x7, _ = ratio(r7)
            line = f'| `{r[0]}` | {r[1]} | {r[2]} | {best[1]} ({best[0]}) |'
            if x < 1 - TIE:
                ahead.append((1 / x, line, 1 / x7))
            elif x > 1 + TIE:
                behind.append((x, line, x7))
        for title, rows, col in [('ahead', ahead, 'best other / hypatia'), ('behind', behind, 'hypatia / best other')]:
            out.append(f'\n## Where hypatia now is {title} ({p}, default seed)\n')
            out.append('Ratio of the mean errors, with the default seed and with COMPARE_SEED=7.\n')
            out.append(f'| function | inputs | hypatia now (largest / mean) | best other (largest / mean) | {col} | same, seed 7 |')
            out.append('|---|---|---|---|---|---|')
            for x, line, x7 in sorted(rows, key=lambda t: -t[0]):
                out.append(f'{line} {x:.3g} | {x7:.3g} |')

    out.append('\n## Where now is less precise than master (default seed)\n')
    out.append('Mean error more than 2% above master\'s.\n')
    out.append('| precision | function | inputs | master (largest / mean) | now (largest / mean) |')
    out.append('|---|---|---|---|---|')
    for p in ['double', 'single']:
        m, n = table(os.path.join(d, f'master.{p}.md')), table(os.path.join(d, f'now.{p}.md'))
        for rm, rn in zip(m, n):
            if mean(rm[2]) is not None and mean(rn[2]) is not None and mean(rn[2]) > mean(rm[2]) * (1 + TIE):
                out.append(f'| {p} | `{rn[0]}` | {rn[1]} | {rm[2]} | {rn[2]} |')

    if len(seeds) > 1:
        out.append('\n## Noise: the same measurement with two seeds\n')
        out.append('Relative change of each mean error between the default seed and COMPARE_SEED=7, over every library and measurement (rows with "inf" or a mean of 0 left out).  Differences between libraries smaller than this are not meaningful.\n')
        out.append('| precision | measurements | median change | 90th percentile | largest |')
        out.append('|---|---|---|---|---|')
        big = []
        for p in ['double', 'single']:
            ch = []
            for v in ['master', 'before', 'now']:
                a, b = table(os.path.join(d, f'{v}.{p}.md')), table(os.path.join(d, f'{v}.{p}.s7.md'))
                names = ['hypatia ' + v] + (libs(p) if v == 'now' else [])
                for ra, rb in zip(a, b):
                    cells = list(zip(ra[2:3 + len(names) - 1], rb[2:3 + len(names) - 1]))
                    for name, (x, y) in zip(names, cells):
                        mx, my = mean(x), mean(y)
                        if mx and my and mx != float('inf') and my != float('inf') and largest(x) < WRONG:
                            c = abs(mx - my) / max(mx, my)
                            ch.append(c)
                            if c > 0.05:
                                big.append(f'| {p} | {name} | `{ra[0]}` | {ra[1]} | {x} | {y} |')
            ch.sort()
            out.append(f'| {p} | {len(ch)} | {100 * ch[len(ch) // 2]:.2g}% | {100 * ch[int(len(ch) * 0.9)]:.2g}% | {100 * ch[-1]:.2g}% |')
        if big:
            out.append('\nThe measurements whose mean changes by more than 5%:\n')
            out.append('| precision | library | function | inputs | default seed | COMPARE_SEED=7 |')
            out.append('|---|---|---|---|---|---|')
            out.extend(big)

    for p in ['double', 'single']:
        m = table(os.path.join(d, f'master.{p}.md'))
        b = table(os.path.join(d, f'before.{p}.md'))
        n = table(os.path.join(d, f'now.{p}.md'))
        names = libs(p)
        out.append(f'\n## All measurements ({p}, default seed)\n')
        out.append('Largest / mean error in ulps.  Bold: the smallest mean among now and the other libraries, and the means within 2% of it.  Empty: no such function (or, for master, no function of that meaning).\n')
        out.append('| function | inputs | master | before | now | ' + ' | '.join(names) + ' |')
        out.append('|' + '---|' * (5 + len(names)))
        for rm, rb, rn in zip(m, b, n):
            cand = [mean(c) for c in [rn[2]] + rn[3:3 + len(names)] if c]
            best = min(cand) if cand else None

            def cell(c, bold):
                if not c:
                    return ''
                return f'**{c}**' if bold and best is not None and mean(c) <= best * (1 + TIE) else c
            out.append('| ' + ' | '.join([rn[0], rn[1], cell(rm[2], False), cell(rb[2], False), cell(rn[2], True)] +
                                         [cell(c, True) for c in rn[3:3 + len(names)]]) + ' |')

    out.append('\n## Oracle check (now, default seed)\n')
    for p in ['double', 'single']:
        out.append(f'{p}:\n')
        out.append('| reference | largest disagreement |')
        out.append('|---|---|')
        out.extend(oracle(os.path.join(d, f'now.{p}.md')))
        out.append('')
    print('\n'.join(out))


main()
