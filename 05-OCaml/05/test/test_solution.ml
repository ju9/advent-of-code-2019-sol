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
	let expected = Addition {arg1 = 0; arg2 = 3; dest = 2} in
	let operation, ip = parse_instruction input 0 in
	assert_equal expected operation;
	assert_equal 4 ip ~printer:string_of_int
	

let test_perform_instruction _ =
	let tape = [|1;2;3;4;1;1;99|] in
	let expected = [|1;2;3;4;7;1;99|] in
	perform_instruction tape (Addition {arg1 = 2; arg2 = 3; dest = 4});
	assert_equal expected tape ~printer:string_of_array

let test_perform_instruction2 _ =
	let tape = [|2;2;3;4;1;1;99|] in
	let expected = [|2;2;3;4;12;1;99|] in
	perform_instruction tape (Multiplication {arg1 = 2; arg2 = 3; dest = 4});
	assert_equal expected tape ~printer:string_of_array

let test_perform_instruction3 _ =
	let tape = [||] in
	assert_raises End_of_file (fun () -> perform_instruction tape (Halt))

let test_run_tape _ =
	let tape = [|2; 0; 0; 0; 2; 4; 8; 1; 99|] in
	let expected = [|4; 198; 0; 0; 2; 4; 8; 1; 99|] in
	run_tape tape;
	assert_equal expected tape ~printer:string_of_array

let test_setup_tape _ =
	let tape = [|1;0;3;2;99|] in
	let expected = [|1;12;2;2;99|] in
	setup_tape tape 12 2;
	assert_equal expected tape ~printer:string_of_array

let test_mainloop _ =
	let input = "2,0,0,0,2,4,8,1,1,0,1,0,99" in
	let expected = 1190 in
	assert_equal expected (run_program input) ~printer:string_of_int

let suite = 
	"Solution tests" >::: [
		"parse input" >:: test_parse_input;
		"parse input2" >:: test_parse_input2;
		"parse instruction" >:: test_parse_instruction;
		"perform instruction" >:: test_perform_instruction;
		"perform instruction2" >:: test_perform_instruction2;
		"perform instruction3" >:: test_perform_instruction3;
		"run tape" >:: test_run_tape;
		"setup tape" >:: test_setup_tape;
		"mainloop" >:: test_mainloop;
	]

let () =
	run_test_tt_main suite