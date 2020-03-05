open Solution
open OUnit2

let string_of_array (arr:int array) : string =
	Array.fold_right (fun item acc -> (string_of_int item) ^ "," ^ acc) arr ""
	
let string_of_some_int (value:int option) : string =
	match value with
	| Some x -> string_of_int x
	| None -> "NONE"

let test_parse_input _ =
	let input = "0,1,234,-3,45,1102" in
	let expected = [|0;1;234;-3;45;1102|] in
	let tape = tape_of_string input in
	assert_equal expected tape

let test_parse_input2 _ =
	let input = "0, 1,234,-3,45,1102\n" in
	let expected = [|0;1;234;-3;45;1102|] in
	let tape = tape_of_string input in
	assert_equal expected tape

let test_parse_instr_add _ =
	let input = [|1;0;3;2;99|] in
	let expected = Addition (
		{arg1 = 0; arg2 = 3; dest = 2},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	) in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int

let test_parse_instr_mult _ =
	let input = [|2;1;3;2;99|] in
	let expected = Multiplication (
		{arg1 = 1; arg2 = 3; dest = 2},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	) in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int

let test_parse_instr_input _ =
	let input = [|3;2;99|] in
	let expected = Input 2 in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 2 ip ~printer:string_of_int

let test_parse_instr_output _ =
	let input = [|4;2;99|] in
	let expected = Output (
		2,
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	) in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 2 ip ~printer:string_of_int

let test_parse_instr_jump_true _ =
	let input = [|5;2;1;99|] in
	let expected = Jump_true (
		{cond = 2; jump = 1},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	) in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 3 ip ~printer:string_of_int

let test_parse_instr_jump_false _ =
	let input = [|6;2;1;99|] in
	let expected = Jump_false (
		{cond = 2; jump = 1},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	) in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 3 ip ~printer:string_of_int

let test_parse_instr_less _ =
	let input = [|7;1;3;2;99|] in
	let expected = Less (
		{arg1 = 1; arg2 = 3; dest = 2},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	) in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int

let test_parse_instr_equal _ =
	let input = [|8;1;3;2;99|] in
	let expected = Equal (
		{arg1 = 1; arg2 = 3; dest = 2},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	) in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int

let test_parse_instr_halt _ =
	let input = [|99;99|] in
	let expected = Halt in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 1 ip ~printer:string_of_int

let test_parse_instr_mult_2 _ =
	let input = [|10101;0;3;2;99|] in
	let expected = Addition (
		{arg1 = 0; arg2 = 3; dest = 2},
		{amarg1 = Immediate; amarg2 = Direct; amdest = Immediate}
	) in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int

let test_parse_instr_mult_3 _ =
	let input = [|1102;1;3;2;99|] in
	let expected = Multiplication (
		{arg1 = 1; arg2 = 3; dest = 2},
		{amarg1 = Immediate; amarg2 = Immediate; amdest = Direct}
	) in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int

let test_perform_instr_add _ =
	let tape = [|1;2;3;4;1;1;99|] in
	let expected = [|1;2;3;4;7;1;99|] in
	let _ = perform_instruction tape (Addition (
		{arg1 = 2; arg2 = 3; dest = 4},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	)) in
	assert_equal expected tape ~printer:string_of_array

let test_perform_instr_mult _ =
	let tape = [|2;2;3;4;1;1;99|] in
	let expected = [|2;2;3;4;12;1;99|] in
	let _ = perform_instruction tape (Multiplication (
		{arg1 = 2; arg2 = 3; dest = 4},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	)) in
	assert_equal expected tape ~printer:string_of_array

(*
let test_perform_instr_input _ =
	let tape = [|3;4;99;0;0|] in
	let expected = [|3;4;99;0;5|] in
	print_endline "Pleez enter '5', sorry.";
	let _ = perform_instruction tape (Input 4) in
	assert_equal expected tape ~printer:string_of_array
*)

let test_perform_instr_output _ =
	let tape = [|4;4;99;0;5|] in
	let _ = perform_instruction tape (Output (
		4,
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	)) in
	print_endline " <- it should've said '5' here."

let test_perform_instr_jump_true _ =
	let tape = [|5;4;4;99;0|] in
	let result = perform_instruction tape (Jump_true (
		{cond = 4; jump = 4},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	)) in
	assert_equal None result ~printer:string_of_some_int

let test_perform_instr_jump_true_2 _ =
	let tape = [|5;4;4;99;1|] in
	let result = perform_instruction tape (Jump_true (
		{cond = 4; jump = 4},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	)) in
	assert_equal (Some 1) result ~printer:string_of_some_int

let test_perform_instr_jump_true_3 _ =
	let tape = [|1105;4;4;99;0|] in
	let result = perform_instruction tape (Jump_true (
		{cond = 4; jump = 4},
		{amarg1 = Immediate; amarg2 = Immediate; amdest = Direct}
	)) in
	assert_equal (Some 4) result ~printer:string_of_some_int

let test_perform_instr_less _ =
	let tape = [|7;2;3;5;4;8;99|] in
	let expected = [|7;2;3;5;4;1;99|] in
	let _ = perform_instruction tape (Less (
		{arg1 = 2; arg2 = 3; dest = 5},
		{amarg1 = Direct; amarg2 = Direct; amdest = Direct}
	)) in
	assert_equal expected tape ~printer:string_of_array

let test_perform_instr_halt _ =
	let tape = [||] in
	assert_raises End_of_file (fun () -> ignore (perform_instruction tape (Halt)))

let test_perform_instr_mult_2 _ =
	let tape = [|1002;2;100;4;1;1;99|] in
	let expected = [|1002;2;100;4;10000;1;99|] in
	let result = perform_instruction tape (Multiplication (
		{arg1 = 2; arg2 = 100; dest = 4},
		{amarg1 = Direct; amarg2 = Immediate; amdest = Direct}
	)) in
	assert_equal expected tape ~printer:string_of_array;
	assert_equal None result


let test_run_tape _ =
	let tape = [|2; 0; 0; 0; 2; 4; 8; 1; 99|] in
	let expected = [|4; 198; 0; 0; 2; 4; 8; 1; 99|] in
	run_tape tape;
	assert_equal expected tape ~printer:string_of_array

let test_mainloop _ =
	let input = "2,0,0,0,2,4,8,1,1,0,1,0,99" in
	(* "2,0,0,0,2,4,8,1,1,0,1,0,99" *)
	(* "4,0,0,0,2,4,8,1,1,0,1,0,99" *)
	(* "4,2,0,0,2,4,8,1,1,0,1,0,99" *)
	(* "6,2,0,0,2,4,8,1,1,0,1,0,99" *)
	let expected = 6 in
	assert_equal expected (run_program input) ~printer:string_of_int

let suite = 
	"Solution tests" >::: [
		"parse input" >:: test_parse_input;
		"parse input2" >:: test_parse_input2;
		"parse instr add" >:: test_parse_instr_add;
		"parse instr mult" >:: test_parse_instr_mult;
		"parse instr mult 2" >:: test_parse_instr_mult_2;
		"parse instr mult 3" >:: test_parse_instr_mult_3;
		"parse instr input" >:: test_parse_instr_input;
		"parse instr output" >:: test_parse_instr_output;
		"parse instr halt" >:: test_parse_instr_halt;
		"perform instr add" >:: test_perform_instr_add;
		"perform instr mult" >:: test_perform_instr_mult;
(*
		"perform instr input" >:: test_perform_instr_input;
*)
		"perform instr output" >:: test_perform_instr_output;
		"perform instr jump true" >:: test_perform_instr_jump_true;
		"perform instr jump true 2" >:: test_perform_instr_jump_true_2;
		"perform instr jump true 3" >:: test_perform_instr_jump_true_3;
		"perform instr less" >:: test_perform_instr_less;
		"perform instr halt" >:: test_perform_instr_halt;
		"perform instr mult 2" >:: test_perform_instr_mult_2;
		"run tape" >:: test_run_tape;
		"mainloop" >:: test_mainloop
	]

let () =
	run_test_tt_main suite