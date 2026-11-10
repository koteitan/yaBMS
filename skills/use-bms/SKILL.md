---
name: use-bms
description: Build and drive the yaBMS `c/bms` CLI on Linux — expand a Bashicu Matrix by a bracket, check whether a matrix is standard, compare two matrices, and search for expansion loops, across BMS versions 4 / 3.3 / 2 / 1.1 / DBMS. Use whenever a task involves expanding, comparing, standard-checking, or loop-hunting Bashicu Matrices, or asks to run yaBMS.
---

[← Back](../../README.md) | [English](README.md) | [Japanese](README-ja.md)

# use-bms

Drive the `bms` C command in `c/` of the yaBMS repository. Target: Linux (also
WSL). The MCP server in `mcp/` is a separate, optional path — do not use it for
these tasks.

## Locate the repository

All paths below are relative to a yaBMS checkout. Find it in this order:

1. the current working directory, if it contains `c/bms.c`;
2. `~/code/yaBMS`;
3. otherwise clone it — `git clone https://github.com/koteitan/yaBMS.git`.

This skill is mirrored in `~/my-skills`, so it may be active outside the yaBMS
tree. Never assume the skill directory sits inside the checkout.

## Build first

`bms` is not committed; it must be compiled before any use.

```bash
cd <repo>/c
make            # gcc -O0 -g -std=c99 -o bms bms.c
```

Requires `gcc` only — no libraries beyond libc. Re-run `make` after editing
`bms.c` / `bms.h`. `make clean` removes the binary.

Check that the binary exists before running anything; if `./bms` is missing,
run `make` rather than reporting a failure.

## Input syntax

A Bashicu Matrix is written as consecutive parenthesised columns, optionally
followed by brackets:

```
(0,0,0)(1,1,1)(2,1,0)(1,1,1)[2]
 \_____ columns _____________/ \_ bracket
```

- Each `(...)` is one **column**; the numbers inside are the rows.
- **Every column must have the same number of rows.** A mismatch prints
  `error:ys mismatch` and then segfaults.
- `[n]` is the expansion count. Multiple brackets `[2][1]` are allowed and are
  consumed left to right (see `-r`).
- Always quote the argument in the shell — parentheses are shell metacharacters.

## Commands

All forms take the matrix as the last positional argument(s).

| Task | Command |
|------|---------|
| Expand | `./bms [-e] [-d] [-r] [-v ver] "<bm>"` |
| Standard check | `./bms -s [-d] [-v ver] "<bm>"` |
| Compare | `./bms -c [-d] "<bm0>" "<bm1>"` |
| Loop check (single) | `./bms -l [-d] [-v ver] "<bm>"` |
| Loop search (range) | `./bms -lr [-d] [-v ver] "<bm0>" "<bm1>" [<depth>]` |
| Help | `./bms -h` (add `-d` for copyright) |

Options combine into one token (`-sd`, `-lrd`, `-ed`, `-lrdv 1.1`).

### Expand — `-e` (default)

Expansion is the default when no command flag is given, so `-e` is optional.

```bash
$ ./bms "(0,0,0)(1,1,1)(2,1,0)(1,1,1)[2]"
(0,0,0)(1,1,1)(2,1,0)(1,1,0)(2,2,1)(3,2,0)(2,2,0)(3,3,1)(4,3,0)
```

`-r` keeps expanding while brackets remain, printing every intermediate step:

```bash
$ ./bms -r "(0)(1)(2)(3)[1][1][1][1]"
(0)(1)(2)(2)[1][1][1]
(0)(1)(2)(1)(2)[1][1]
(0)(1)(2)(1)(1)[1]
(0)(1)(2)(1)(0)(1)(2)(1)
```

`-d` prints the internal decomposition (parent index matrix, good/bad part, bad
root, `lnz`, delta, ascension matrix) before the result.

### Standard check — `-s`

Prints `1` if the matrix is standard, `0` otherwise. With `-d`, prints the
comparison trace and `standard.` / `non-standard.` instead.

```bash
$ ./bms -s "(0)(1)"
1
$ ./bms -s "(1)"
0
```

### Compare — `-c`

Prints `1` / `0` / `-1` for `bm0 > bm1` / `==` / `<`. Brackets are ignored, and
so is `-v` (comparison is version-independent).

```bash
$ ./bms -c "(0,0)(1,1)(2,0)" "(0,0)(1,1)(1,1)"
1
$ ./bms -cd "(0,0)(1,1)(2,0)" "(0,0)(1,1)(1,1)"
(0,0)(1,1)(2,0) > (0,0)(1,1)(1,1)
```

### Loop finding — `-l`, `-lr`

`-l <bm>` first expands the matrix until no brackets remain, then reports `1` if
the result loops (gets no smaller under a further expansion), `0` otherwise.

`-lr <bm0> <bm1> [depth]` searches for a loop from `bm1` down to `bm0` within
`depth` expansions (default `3`). Detection is heuristic and may miss loops.
Runtime grows sharply with `depth` — start at 3–5 and always wrap deep searches
in `timeout`.

```bash
$ ./bms -lr "(0,0,0)(1,1,1)(2,1,0)" "(0,0,0)(1,1,1)(2,1,0)(1,1,1)" 3
0
```

### Versions — `-v`

`-v {4 | 3.3 | 2 | 1.1 | DBMS}`, default `4`. Aliases are accepted: `4.0`,
`2.3`, `BM4`, `BMS4`, `BM2.0`, `DBM`, `DBMS4.0`, and so on. An unrecognised
value is **silently ignored** and version 4 is used — verify with `-d`, which
prints `version = BM4`.

Versions differ only on some matrices; expect identical output on simple ones.

```bash
$ ./bms -v3.3 "(0,0,0)(1,1,1)(2,1,0)(1,1,1)[2]"
(0,0,0)(1,1,1)(2,1,0)(1,1,0)(2,2,1)(3,1,0)(2,2,0)(3,3,1)(4,1,0)
```

## Gotchas

These are real behaviours of the current binary — check for them rather than
assuming clean CLI semantics.

- **No bracket, no output.** `./bms "(0,0)(1,1)"` prints nothing and exits `0`.
  The expand loop runs only while brackets remain. Do not read an empty result
  as a crash; add a `[n]`.
- **`-lr` prints without a trailing newline** (plain `0` / `1`). `-l`, `-s`,
  `-c` and expansion all do end with a newline. Use `$(...)` capture, not
  line-oriented reads, for `-lr`.
- **Errors report through stdout and exit code, not stderr.** Missing operands
  print `error: input is not enough.` and exit `1`. The `ys mismatch` message
  is the one that goes to stderr, and it is followed by a segfault (exit `139`).
- **No bounds checks.** `bms.h` fixes `BMS_ELEMS_MAX 4096` (total row×column
  entries) and `BMS_BRACKETS_MAX 256`. Exceeding either corrupts the heap and
  aborts (`munmap_chunk(): invalid pointer`, `malloc(): corrupted top size`).
  For a 3-row matrix that is 1365 columns. Keep inputs well under the limits,
  and treat a crash on a large matrix as an overflow, not a bug in the input.
- **`./bms` with no arguments prints help and exits `0`** — same as `-h`.
- Exit code is `0` for every successful run regardless of the answer; the
  verdict is on stdout. Do not branch on `$?` to read a result.

## Iterating expansions

To walk a fundamental sequence, feed the output back with the next bracket:

```bash
bm="(0,0)(1,1)(2,2)"
for n in 1 2 3 4; do
  bm=$(./bms "$bm[$n]")
  echo "n=$n $bm"
done
```

Matrices grow fast; guard long runs with `timeout` and check the size against
the 4096-entry limit before the next step. `c/example.sh` is a worked example of
batching a version sweep across depths.

## Reference

- `c/README.md` — upstream feature documentation with examples.
- `c/bms.h` — array limits, version enum, function contracts.
- `mcp/doc/what-is-bms.md` — what a Bashicu Matrix is, the matrix↔ordinal
  correspondence table, and what expansion means. Read this when the task needs
  the mathematics rather than the CLI.
- `make doc` in `c/` builds Doxygen output (needs `doxygen`).
