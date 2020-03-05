# Advent of Code 2019

[Advent of Code](https://adventofcode.com/2019) is an Advent calendar of small programming puzzles.

This repo contains solutions to AoC-2019 by _ju9_.
Programming languages used in solutions vary; _C_ will be represented; the intention is to implement solutions also in _OCaml_ and _Python_ and whatever other fun languages.

## Purpose

- practice programming, testing, gitting
- create a portfolio that can be presented alongside CV

## Background

In recent years I have used mainly PHP at my former job. I wish to practice programming in other languages, and to practice other dev-tasks like testing.

## I/O Interface

Each solution is a standalone program called _solution_ or _main.exe_. Input will be read from standard input, output will be passed to standard output.
Usage: solution < input.txt > output.txt

Where keyboard input is needed, the usage may be: main.exe "input.txt" etc. - may be indicated in Makefile under the 'run' target.

For solutions in interpreted languages the usage may vary; wrapper in Bash may be used.

## Dependencies

### C

- [Check](https://libcheck.github.io/check/) (tests)

### OCaml

- [dune](https://dune.build/) (building)
- [ounit2](https://github.com/gildor478/ounit) (tests)