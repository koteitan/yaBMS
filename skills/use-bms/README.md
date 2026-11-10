[← Back](../../README.md) | [English](README.md) | [Japanese](README-ja.md)

# use-bms

An agent skill that teaches Claude Code how to build and run the `c/bms`
command of the [yaBMS](https://github.com/koteitan/yaBMS) repository on Linux.

The skill itself lives in [SKILL.md](SKILL.md). This file explains what it is
and how to install it, for humans.

It is kept in two places, byte-identical: `skills/use-bms/` inside yaBMS, and
`skills/use-bms/` in [my-skills](https://github.com/koteitan/my-skills). Edit
one and copy it to the other.

## What it covers

`c/bms` is the C implementation of the Bashicu Matrix System commander. The
skill documents all four of its modes:

| Mode | Flag | What it does |
|------|------|--------------|
| Expander | `-e` (default) | Expands a matrix by its bracket |
| Standard checker | `-s` | Reports whether a matrix is standard |
| Comparator | `-c` | Reports which of two matrices is larger |
| Loop finder | `-l`, `-lr` | Detects expansions that fail to decrease |

It also covers the `-v` version switch (BM4 / BM3.3 / BM2 / BM1.1 / DBMS), the
`-d` detail output, and the `-r` multi-bracket mode.

Beyond the flag reference, the skill records behaviours that are easy to get
wrong and that the upstream `c/README.md` does not mention:

- expanding a matrix with no `[n]` bracket prints nothing and exits `0`
- `-lr` writes its `0` / `1` verdict with no trailing newline
- the fixed 4096-entry / 256-bracket arrays are unchecked, so oversized input
  corrupts the heap and aborts
- a row-length mismatch segfaults after printing its error
- the exit code is `0` on success no matter what the verdict is

The MCP server under `mcp/` is a separate, optional route to the same
functionality. This skill deliberately does not use it — it drives the compiled
binary directly.

## Requirements

- Linux or WSL
- `gcc` (C99); no libraries beyond libc
- optionally `doxygen` for `make doc`

## Install

Symlink the directory into a skills path Claude Code reads. From either
checkout:

```bash
# for every project
ln -s "$PWD/skills/use-bms" ~/.claude/skills/use-bms

# or for one project only
mkdir -p <project>/.claude/skills
ln -s "$PWD/skills/use-bms" <project>/.claude/skills/use-bms
```

The skill locates the yaBMS checkout itself (current directory, then
`~/code/yaBMS`, then a fresh clone), so installing from my-skills works even
when yaBMS is not the project you are in.

## Use

Just describe the task; the skill triggers on Bashicu Matrix work.

```
Expand (0,0,0)(1,1,1)(2,1,0)(1,1,1)[2] with BM3.3.
Is (0,0)(1,1)(2,1) standard?
Search for a loop between these two matrices at depth 5.
```

You can also invoke it by name with `/use-bms`.

## Build by hand

The skill builds the binary itself, but the equivalent manual steps are:

```bash
cd c
make
./bms -h
./bms "(0,0,0)(1,1,1)(2,1,0)(1,1,1)[2]"
```

The compiled `c/bms` is not tracked by git.
