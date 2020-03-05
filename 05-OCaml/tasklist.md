# TODOs

## E2P1

- read input (file) into a line (string)
- ~~parse input line (string) with codes into an array (tape)~~
- ~~scan the tape from start to end, processing them instructions~~
- ~~read by indirect addressing~~
- ~~write by indirect addressing~~
- ~~parse current instruction (what, params, length, ...)~~
- ~~add instruction "1" (@1 + @2 = @3), length 4~~
- ~~mult instruction "2" (@1 * @2 = @3), length 4~~
- ~~halt instruction "99", length 1~~
- ~~perform instructions~~
- ~~initial set-up (#1 = "12", #2 = "2")~~
- ~~mainloop: load, setup, run tape, after halt return #0~~

E2P2 seems not not to be a dependency of E5P1.

## E5P1

- input instruction "3" (@1 = in), length 2
- output instruction "4" (out = @1), length 2
- parse opcodes in "parameter mode", extend opcode typedefs

