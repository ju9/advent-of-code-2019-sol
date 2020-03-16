open Solution
open OUnit2

(*
let string_of_array (arr:int array) : string =
	Array.fold_right (fun item acc -> (string_of_int item) ^ "," ^ acc) arr ""
	
let string_of_some_int (value:int option) : string =
	match value with
	| Some x -> string_of_int x
	| None -> "NONE"
*)

let string_of_list list =
	List.fold_left (fun acc item -> item ^ "::" ^ acc) "" list

let test_parse_orbit _ =
	let input = "ABC)BCD" in
	let center, satellite = parse_orbit input in
	assert_equal "ABC" center;
	assert_equal "BCD" satellite

let test_parse_input _ =
	let ic = open_in "../../../../06/test/test_mini.txt" in
	let reader () =
		input_line ic
	in
	let table = parse_input reader in
	assert_equal 4 (Hashtbl.length table) ~printer:string_of_int;
	assert_equal ["B"] (Hashtbl.find table "COM") ~printer:string_of_list;
	assert_equal ["D";"C"] (Hashtbl.find table "B") ~printer:string_of_list;
	assert_equal [] (Hashtbl.find table "C") ~printer:string_of_list;
	assert_equal [] (Hashtbl.find table "D") ~printer:string_of_list;
	close_in ic
	
let test_checksum _ =
	let ic = open_in "../../../../06/test/test_42.txt" in
	let reader () =
		input_line ic
	in
	let table = parse_input reader in
	let checksum = compute_checksum table in
	assert_equal 42 checksum ~printer:string_of_int

let suite = 
	"Solution tests" >::: [
		"parse orbit" >:: test_parse_orbit;
		"parse input" >:: test_parse_input;
		"test checksum" >:: test_checksum
	]

let () =
	run_test_tt_main suite