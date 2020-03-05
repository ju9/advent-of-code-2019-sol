
(*
	failwith "not implemented"
*)

type addressing = Direct | Immediate

type three_arg_op = {
	arg1: int;
	arg2: int;
	dest: int;
	amarg1: addressing;
	amarg2: addressing;
	amdest: addressing (* always Direct addressing, never Immediate *)
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


let tape_of_string input =
	let string_list = String.split_on_char ',' input in
	let int_of_dirty_string item =
		int_of_string (String.trim item)
	in
	let int_list = List.map int_of_dirty_string string_list in
	Array.of_list int_list

	

let parse_instruction tape ip =
	let mode_of_code code = if code = 0 then Direct else Immediate in
	let parse_addressing opcode =
		let modes = opcode / 100 in
		let m1 = modes mod 10 in
		let m2 = (modes mod 100) - m1 in
		let m3 = modes / 100 in
		(mode_of_code m1, mode_of_code m2, mode_of_code m3)
	in
	let next_three_args opcode =
		let amarg1, amarg2, amdest = parse_addressing opcode in
		{
			arg1 = (tape.(ip + 1));
			arg2 = (tape.(ip + 2));
			dest = (tape.(ip + 3));
			amarg1 = amarg1;
			amarg2 = amarg2;
			amdest = amdest;
		}
	in
	let next_one_arg () =
		tape.(ip + 1)
	in
	let opcode = tape.(ip) in
	let instruction = opcode mod 100 in
	
	match instruction with
	| 1 -> Addition (next_three_args opcode), ip + 4
	| 2 -> Multiplication (next_three_args opcode), ip + 4
	| 3 -> Input (next_one_arg ()), ip + 2
	| 4 -> Output ({ arg = next_one_arg (); amode = mode_of_code (opcode / 100)}), ip + 2
	| 99 -> Halt, ip + 1
	| _ -> Unknown opcode, ip + 1


let read_code () =
	print_string "Enter code: ";
	read_int ()

let perform_instruction tape instruction = 
	let eval_arg tape arg amode =
		match amode with
		| Direct -> tape.(arg)
		| Immediate -> arg
	in
	let do_operation tape op args =
		let arg1 = eval_arg tape args.arg1 args.amarg1 in
		let arg2 = eval_arg tape args.arg2 args.amarg2 in
		tape.(args.dest) <- op arg1 arg2
	in
	match instruction with
	| Addition args -> do_operation tape ( + ) args
	| Multiplication args -> do_operation tape ( * ) args
	| Input dest ->
		tape.(dest) <- read_code ()
	| Output args ->
		print_endline (string_of_int (eval_arg tape args.arg args.amode))
	| Halt -> raise End_of_file
	| Unknown opcode -> failwith ("unknown opcode: " ^ (string_of_int opcode))


let run_tape tape =
	let rec runner ip = 
		let instruction, next_ip = parse_instruction tape ip in
		perform_instruction tape instruction;
		runner next_ip
	in
	try runner 0 with
	| End_of_file -> ()
	

let run_program input =
	let tape = tape_of_string input in
	run_tape tape;
	tape.(0)
	