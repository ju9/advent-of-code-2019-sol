

type addressing = Direct | Immediate

type three_arg_op = {
	arg1: int;
	arg2: int;
	dest: int;
	amarg1: addressing;
	amarg2: addressing;
	amdest: addressing; (* always Direct addressing, never Immediate *)
}

type one_arg_op = {
	arg: int;
	amode : addressing
}

type instruction =
	  Addition of three_arg_op
	| Multiplication of three_arg_op
	| Input of int
	| Output of one_arg_op
	| Halt
	| Unknown of int

val tape_of_string :
	   string
	-> int array
(* Given a string [input] representing the tape, parse that string into an array [tape] - and return that *)

val parse_instruction :
	   int array
	-> int
	-> instruction * int
(* Read from [tape] from at address given by [instruction pointer], consuming one or more codes. Construct an [instruction with its parameters] and compute a new [instruction pointer] advanced past this instruction; return these two items [instr * next_ip] *)

val read_code :
	   unit
	-> int
(* read one keyboard input, return as integer *)

val perform_instruction :
	   int array
	-> instruction
	-> unit
(* On [tape], given an [instruction], perform that instruction's operation *)

val run_tape :
	   int array
	-> unit
(* Run [tape] from start until Halt or error *)


val run_program :
	   string
	-> int
(* given a string [input], load tape, init it, run it until halt, and return value at [#0] *)