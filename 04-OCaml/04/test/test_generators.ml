open Generators
open OUnit2

(*
Depends: module Pass must be OK
*)

let assert_valid validator num =
	assert_bool (Printf.sprintf "Should be valid: %d" num) (validator (Pass.of_int num))
	
let assert_invalid validator num =
	assert_bool (Printf.sprintf "Should be invalid: %d" num) (not (validator (Pass.of_int num)))

let assert_next generator num expect =
	assert_equal expect (Pass.to_int (generator (Pass.of_int num))) ~printer:string_of_int

let test_double_valid _ =
	assert_valid double_valid 11;
	assert_valid double_valid 22;
	assert_valid double_valid 99;
	assert_valid double_valid 122;
	assert_valid double_valid 221;
	assert_valid double_valid 1221;
	assert_valid double_valid 100121;
	assert_valid double_valid 133221;
	assert_valid double_valid 123455;
	assert_valid double_valid 112345;
	assert_valid double_valid 123345;
	assert_valid double_valid 111112;
	assert_valid double_valid 122222;

	assert_invalid double_valid 1;
	assert_invalid double_valid 2;
	assert_invalid double_valid 9;
	assert_invalid double_valid 10;
	assert_invalid double_valid 34;
	assert_invalid double_valid 121314;
	assert_invalid double_valid 512131

let assert_consec_repeats len rest_num num =
	let pass = Pass.of_int num in
	let result = consec_repeats (List.hd pass) (List.tl pass) 1 in
	assert_equal len (fst result) ~printer:string_of_int;
	assert_equal rest_num (Pass.to_int (snd result)) ~printer:string_of_int

let test_consec_repeats _ =
	assert_consec_repeats 1 0 1;
	assert_consec_repeats 2 0 11;
	assert_consec_repeats 3 0 111;
	assert_consec_repeats 1 111 1118;
	assert_consec_repeats 3 8 8111

let test_strict_double_valid _ =
	assert_valid strict_double_valid 11;
	assert_valid strict_double_valid 22;
	assert_valid strict_double_valid 99;
	assert_valid strict_double_valid 122;
	assert_valid strict_double_valid 221;
	assert_valid strict_double_valid 1221;
	assert_valid strict_double_valid 100121;
	assert_valid strict_double_valid 133221;
	assert_valid strict_double_valid 123455;
	assert_valid strict_double_valid 112345;
	assert_valid strict_double_valid 123345;
	assert_valid strict_double_valid 111122;
	assert_valid strict_double_valid 112222;
	
	assert_invalid strict_double_valid 111112;
	assert_invalid strict_double_valid 122222;
	assert_invalid strict_double_valid 123444;
	assert_invalid strict_double_valid 588889;
	assert_invalid strict_double_valid 222289;

	assert_invalid strict_double_valid 1;
	assert_invalid strict_double_valid 2;
	assert_invalid strict_double_valid 9;
	assert_invalid strict_double_valid 10;
	assert_invalid strict_double_valid 34;
	assert_invalid strict_double_valid 121314;
	assert_invalid strict_double_valid 512131

let test_nondec_valid _ =
	assert_valid nondecreasing_valid 6;
	assert_valid nondecreasing_valid 11;
	assert_valid nondecreasing_valid 12;
	assert_valid nondecreasing_valid 39;
	assert_valid nondecreasing_valid 11111;
	assert_valid nondecreasing_valid 11112;
	assert_valid nondecreasing_valid 12222;
	assert_valid nondecreasing_valid 123456;
	assert_valid nondecreasing_valid 155555;
	assert_valid nondecreasing_valid 111115;
	assert_valid nondecreasing_valid 399999;
	assert_valid nondecreasing_valid 333339;
	
	assert_invalid nondecreasing_valid 10;
	assert_invalid nondecreasing_valid 43;
	assert_invalid nondecreasing_valid 123454;
	assert_invalid nondecreasing_valid 123450;
	assert_invalid nondecreasing_valid 123490;
	assert_invalid nondecreasing_valid 999990

let test_double_next _ =
	assert_next double_next 0 11;
	assert_next double_next 8 11;
	assert_next double_next 11 22;
	assert_next double_next 198 199;
	assert_next double_next 199 200;
	assert_next double_next 2343 2344;
	assert_next double_next 2344 2355;
	assert_next double_next 2348 2355;
	assert_next double_next 2350 2355;
	assert_next double_next 2250 2251;
	assert_next double_next 2251 2252;
	assert_next double_next 2255 2256;
	assert_next double_next 42250 42251;
	assert_next double_next 42251 42252;
	assert_next double_next 42255 42256;
	assert_next double_next 12349 12355

let test_nondec_next _ =
	assert_next nondecreasing_next 0 1;
	assert_next nondecreasing_next 4 5;
	assert_next nondecreasing_next 9 11;
	assert_next nondecreasing_next 11 12;
	assert_next nondecreasing_next 300 333;
	assert_next nondecreasing_next 311 333;
	assert_next nondecreasing_next 313 333;
	assert_next nondecreasing_next 319 333;
	assert_next nondecreasing_next 329 333;
	assert_next nondecreasing_next 330 333;
	assert_next nondecreasing_next 333 334;
	assert_next nondecreasing_next 334 335;
	assert_next nondecreasing_next 339 344;
	assert_next nondecreasing_next 109109 111111;
	assert_next nondecreasing_next 50982 55555

let suite = 
	"Generators tests" >::: [
		"validity of doubles" >:: test_double_valid;
		"consecutive repeats" >:: test_consec_repeats;
		"validity of strict doubles" >:: test_strict_double_valid;
		"validity of non-decresasing" >:: test_nondec_valid;
		"generation of doubles" >:: test_double_next;
		"generation of non-decreasing" >:: test_nondec_next
	]

let () =
	run_test_tt_main suite