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
let string_of_my_tuple tuple =
	(fst tuple) ^ " * " ^ (string_of_list (snd tuple))
	
let load_test_table file =
	let ic = open_in ("../../../../06/test/" ^ file) in
	let reader () = input_line ic in
	let table = parse_input reader in
	close_in ic;
	table

let test_parse_orbit _ =
	let input = "ABC)BCD" in
	let center, satellite = parse_orbit input in
	assert_equal "ABC" center;
	assert_equal "BCD" satellite

let test_parse_input _ =
	(*  COM)B
	    B)C
	    B)D		*)
	let table = load_test_table "test_mini.txt" in
	assert_equal 4 (Hashtbl.length table) ~printer:string_of_int;
	assert_equal (free, ["B"]) (Hashtbl.find table com) ~printer:string_of_my_tuple;
	assert_equal (com, ["D";"C"]) (Hashtbl.find table "B") ~printer:string_of_my_tuple;
	assert_equal ("B", []) (Hashtbl.find table "C") ~printer:string_of_my_tuple;
	assert_equal ("B", []) (Hashtbl.find table "D") ~printer:string_of_my_tuple
	
let test_checksum _ =
	let table = load_test_table "test_42.txt" in
	let checksum = compute_checksum table in
	assert_equal 42 checksum ~printer:string_of_int

let test_trace _ =
	(*      G - H       J - K - L
	       /           /
	COM - B - C - D - E - F
	               \
	                I		*)
	let table = load_test_table "test_42.txt" in
	assert_equal ["COM";"B";"C";"D"] (trace_path table "I") ~printer:string_of_list;
	assert_equal ["COM";"B"] (trace_path table "G") ~printer:string_of_list;
	assert_equal ["COM";"B";"C";"D";"E";"J";"K"] (trace_path table "L") ~printer:string_of_list

let test_transfer_count _ =
	let table = load_test_table "test_42.txt" in
	assert_equal 2 (transfer_count table "L" "F") ~printer:string_of_int;
	assert_equal 3 (transfer_count table "L" "I") ~printer:string_of_int;
	assert_equal 4 (transfer_count table "F" "H") ~printer:string_of_int;
	assert_equal 2 (transfer_count table "F" "D") ~printer:string_of_int

let suite = 
	"Solution tests" >::: [
		"parse orbit" >:: test_parse_orbit;
		"parse input" >:: test_parse_input;
		"test checksum" >:: test_checksum;
		"test trace" >:: test_trace;
		"test transfer count" >:: test_transfer_count
	]

let () =
	run_test_tt_main suite