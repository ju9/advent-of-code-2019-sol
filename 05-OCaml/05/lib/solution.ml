

(*
<channel> = open_in <file name>
<line> = input_line <channel>
close_in <channel>

	failwith "not implemented"
*)


type three_arg_op = {
	arg1: int;
	arg2: int;
	dest: int
}

type instruction =
	  Addition of three_arg_op
	| Multiplication of three_arg_op
	| Halt
	| Unknown of int


let tape_of_string input =
	let string_list = String.split_on_char ',' input in
	let int_of_dirty_string item =
		int_of_string (String.trim item)
	in
	let int_list = List.map int_of_dirty_string string_list in
(*
	ignore input; 
	let int_list = [1;0;3;2;99;99;99;99;99;99;99;99;99;99;99;99] in
*)
	Array.of_list int_list

	

let parse_instruction tape ip =
	let next_three_args () =
		{
			arg1 = (tape.(ip + 1));
			arg2 = (tape.(ip + 2));
			dest = (tape.(ip + 3))
		}
	in
	let opcode = tape.(ip) in
	match opcode with
	| 1 -> Addition (next_three_args ()), ip + 4
	| 2 -> Multiplication (next_three_args ()), ip + 4
	| 99 -> Halt, ip + 1
	| _ -> Unknown opcode, ip + 1


let perform_instruction tape instruction = 
	match instruction with
	| Addition args ->
		tape.(args.dest) <- tape.(args.arg1) + tape.(args.arg2)
	| Multiplication args ->
		tape.(args.dest) <- tape.(args.arg1) * tape.(args.arg2)
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
	

let setup_tape tape word1 word2 =
	tape.(1) <- word1;
	tape.(2) <- word2;
	()

let run_program input =
	let tape = tape_of_string input in
	setup_tape tape 12 2;
	run_tape tape;
	tape.(0)
	