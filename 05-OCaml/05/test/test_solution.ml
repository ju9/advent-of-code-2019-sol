open Solution
open OUnit2

let string_of_array (arr:int array) : string =
	Array.fold_right (fun item acc -> (string_of_int item) ^ "," ^ acc) arr ""

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

let test_parse_instruction _ =
	let input = [|1;0;3;2;99|] in
	let expected = Addition {arg1 = 0; arg2 = 3; dest = 2; amarg1 = Direct; amarg2 = Direct; amdest = Direct} in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int

let test_parse_instruction2 _ =
	let input = [|2;1;3;2;99|] in
	let expected = Multiplication {arg1 = 1; arg2 = 3; dest = 2; amarg1 = Direct; amarg2 = Direct; amdest = Direct} in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int

let test_parse_instruction3 _ =
	let input = [|3;2;99|] in
	let expected = Input 2 in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 2 ip ~printer:string_of_int

let test_parse_instruction4 _ =
	let input = [|4;2;99|] in
	let expected = Output {arg = 2; amode = Direct} in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 2 ip ~printer:string_of_int

let test_parse_instruction5 _ =
	let input = [|99;99|] in
	let expected = Halt in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 1 ip ~printer:string_of_int

let test_parse_instruction6 _ =
	let input = [|10101;0;3;2;99|] in
	let expected = Addition ({arg1 = 0; arg2 = 3; dest = 2; amarg1 = Immediate; amarg2 = Direct; amdest = Immediate}) in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int

let test_parse_instruction7 _ =
	let input = [|1102;1;3;2;99|] in
	let expected = Multiplication {arg1 = 1; arg2 = 3; dest = 2; amarg1 = Immediate; amarg2 = Immediate; amdest = Direct} in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int

let test_perform_instruction _ =
	let tape = [|1;2;3;4;1;1;99|] in
	let expected = [|1;2;3;4;7;1;99|] in
	perform_instruction tape (Addition {arg1 = 2; arg2 = 3; dest = 4; amarg1 = Direct; amarg2 = Direct; amdest = Direct});
	assert_equal expected tape ~printer:string_of_array

let test_perform_instruction2 _ =
	let tape = [|2;2;3;4;1;1;99|] in
	let expected = [|2;2;3;4;12;1;99|] in
	perform_instruction tape (Multiplication {arg1 = 2; arg2 = 3; dest = 4; amarg1 = Direct; amarg2 = Direct; amdest = Direct});
	assert_equal expected tape ~printer:string_of_array

(*
let test_perform_instruction3 _ =
	let tape = [|3;4;99;0;0|] in
	let expected = [|3;4;99;0;5|] in
	print_endline "Pleez enter '5', sorry.";
	perform_instruction tape (Input 4);
	assert_equal expected tape ~printer:string_of_array

let test_perform_instruction4 _ =
	let tape = [|4;4;99;0;5|] in
	perform_instruction tape (Output 4);
	print_endline " <- it should've said '5' here."
*)

let test_perform_instruction5 _ =
	let tape = [||] in
	assert_raises End_of_file (fun () -> perform_instruction tape (Halt))

let test_perform_instruction6 _ =
	let tape = [|1002;2;100;4;1;1;99|] in
	let expected = [|1002;2;100;4;10000;1;99|] in
	perform_instruction tape (Multiplication {arg1 = 2; arg2 = 100; dest = 4; amarg1 = Direct; amarg2 = Immediate; amdest = Direct});
	assert_equal expected tape ~printer:string_of_array


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
		"parse instruction" >:: test_parse_instruction;
		"parse instruction2" >:: test_parse_instruction2;
		"parse instruction3" >:: test_parse_instruction3;
		"parse instruction4" >:: test_parse_instruction4;
		"parse instruction5" >:: test_parse_instruction5;
		"perform instruction" >:: test_perform_instruction;
		"perform instruction2" >:: test_perform_instruction2;
(*
		"perform instruction3" >:: test_perform_instruction3;
		"perform instruction4" >:: test_perform_instruction4;
*)
		"perform instruction5" >:: test_perform_instruction5;
		"perform instruction6" >:: test_perform_instruction6;
		"run tape" >:: test_run_tape;
		"mainloop" >:: test_mainloop
	]

let () =
	run_test_tt_main suite