
(*
	failwith "not implemented"
*)

type addressing = Direct | Immediate

type three_arg_op = {
	arg1: int;
	arg2: int;
	dest: int
}

type jump_arg_op = {
	cond: int;
	jump: int
}

type addr_params = {
	amarg1: addressing;
	amarg2: addressing;
	amdest: addressing (* always Direct addressing, never Immediate *)
}

type instruction =
	  Addition of (three_arg_op * addr_params)
	| Multiplication of (three_arg_op * addr_params)
	| Input of int
	| Output of (int * addr_params)
	| Jump_true of (jump_arg_op * addr_params)
	| Jump_false of (jump_arg_op * addr_params)
	| Less of (three_arg_op * addr_params)
	| Equal of (three_arg_op * addr_params)
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
	let mode_of_code code =
		if code = 0 then Direct else Immediate
	in
	let parse_addressing opcode =
		let modes = opcode / 100 in
		let m1 = modes mod 10 in
		let m2 = (modes mod 100) - m1 in
		let m3 = modes / 100 in
		{
			amarg1 = mode_of_code m1;
			amarg2 = mode_of_code m2;
			amdest = mode_of_code m3
		}
	in
	let next_three_args () =
		{
			arg1 = (tape.(ip + 1));
			arg2 = (tape.(ip + 2));
			dest = (tape.(ip + 3))
		}
	in
	let next_jump_args () =
		{
			cond = (tape.(ip + 1));
			jump = (tape.(ip + 2))
		}
	in
	let next_one_arg () =
		tape.(ip + 1)
	in
	let opcode = tape.(ip) in
	let instruction = opcode mod 100 in
	
	match instruction with
	| 1 -> Addition       (next_three_args (), parse_addressing opcode), ip + 4
	| 2 -> Multiplication (next_three_args (), parse_addressing opcode), ip + 4
	| 3 -> Input          (next_one_arg ()), ip + 2
	| 4 -> Output         (next_one_arg (),    parse_addressing opcode), ip + 2
	| 5 -> Jump_true      (next_jump_args (),  parse_addressing opcode), ip + 3
	| 6 -> Jump_false     (next_jump_args (),  parse_addressing opcode), ip + 3
	| 7 -> Less           (next_three_args (), parse_addressing opcode), ip + 4
	| 8 -> Equal          (next_three_args (), parse_addressing opcode), ip + 4
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
	let do_3arg_op tape op args addr =
		let arg1 = eval_arg tape args.arg1 addr.amarg1 in
		let arg2 = eval_arg tape args.arg2 addr.amarg2 in
		tape.(args.dest) <- op arg1 arg2
	in
	let eval_jump_op tape op args addr =
		let cond = eval_arg tape args.cond addr.amarg1 in
		let jump = eval_arg tape args.jump addr.amarg2 in
		if op cond then Some jump else None
	in
	match instruction with
	| Addition (args, addr) ->
		do_3arg_op tape ( + ) args addr;
		None
	| Multiplication (args, addr) ->
		do_3arg_op tape ( * ) args addr;
		None
	| Input dest ->
		tape.(dest) <- read_code ();
		None
	| Output (src, addr) ->
		let value = eval_arg tape src addr.amarg1 in
		print_endline (string_of_int value);
		None
	| Jump_true (args, addr) ->
		let op x = x <> 0 in
		eval_jump_op tape (op) args addr
	| Jump_false (args, addr) ->
		let op x = x = 0 in
		eval_jump_op tape (op) args addr
	| Less (args, addr) ->
		let op a b = if a < b then 1 else 0 in
		do_3arg_op tape (op) args addr;
		None
	| Equal (args, addr) ->
		let op a b = if a = b then 1 else 0 in
		do_3arg_op tape (op) args addr;
		None
	| Halt ->
		raise End_of_file
	| Unknown opcode ->
		failwith ("unknown opcode: " ^ (string_of_int opcode))


let run_tape tape =
	let rec runner ip = 
		let instruction, next_ip = parse_instruction tape ip in
		let result = perform_instruction tape instruction in
		let jump_to = match result with
			| Some redirect -> redirect
			| None -> next_ip
		in
		runner jump_to
	in
	try runner 0 with
	| End_of_file -> ()
	

let run_program input =
	let tape = tape_of_string input in
	run_tape tape;
	tape.(0)
	