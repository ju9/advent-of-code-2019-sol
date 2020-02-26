open Solution
open OUnit2

(*
Depends: module Pass must be OK
*)

let test_arbitration _ =
	
	let y = InAgreement (Pass.of_int 5) in
	let acc = PANone in
	let x = Pass.of_int 5 in
	assert_equal y (pass_arbitration acc x);
	
	let y = InAgreement (Pass.of_int 5) in
	let acc = InAgreement (Pass.of_int 5) in
	let x = Pass.of_int 5 in
	assert_equal y (pass_arbitration acc x);

	let y = Different (Pass.of_int 6) in
	let acc = InAgreement (Pass.of_int 5) in
	let x = Pass.of_int 6 in
	assert_equal y (pass_arbitration acc x);

	let y = Different (Pass.of_int 6) in
	let acc = InAgreement (Pass.of_int 6) in
	let x = Pass.of_int 5 in
	assert_equal y (pass_arbitration acc x);

	let y = Different (Pass.of_int 6) in
	let acc = Different (Pass.of_int 5) in
	let x = Pass.of_int 6 in
	assert_equal y (pass_arbitration acc x);

	let y = Different (Pass.of_int 6) in
	let acc = Different (Pass.of_int 6) in
	let x = Pass.of_int 5 in
	assert_equal y (pass_arbitration acc x)


let assert_all_valid validators num =
	assert_bool (Printf.sprintf "Should be valid: %d" num) (all_rules_valid (Pass.of_int num) validators)

let assert_all_invalid validators num =
	assert_bool (Printf.sprintf "Should be invalid: %d" num) (not (all_rules_valid (Pass.of_int num) validators))


let test_all_valid_1 _ =
	let validators = [ Generators.double_valid ] in
	(
		assert_all_valid validators 11;
		assert_all_valid validators 22;
		assert_all_valid validators 99;
		assert_all_valid validators 122;
		assert_all_valid validators 221;
		assert_all_valid validators 1221;
		assert_all_valid validators 100121;
		assert_all_valid validators 133221;
		assert_all_valid validators 123455;
		assert_all_valid validators 112345;
		assert_all_valid validators 123345;
		assert_all_valid validators 111112;
		assert_all_valid validators 122222;
	
		assert_all_invalid validators 1;
		assert_all_invalid validators 2;
		assert_all_invalid validators 9;
		assert_all_invalid validators 10;
		assert_all_invalid validators 34;
		assert_all_invalid validators 121314;
		assert_all_invalid validators 512131
	)

let test_all_valid_2 _ =
	let validators = [ Generators.nondecreasing_valid ] in
	(
		assert_all_valid validators 6;
		assert_all_valid validators 11;
		assert_all_valid validators 12;
		assert_all_valid validators 39;
		assert_all_valid validators 11111;
		assert_all_valid validators 11112;
		assert_all_valid validators 12222;
		assert_all_valid validators 123456;
		assert_all_valid validators 155555;
		assert_all_valid validators 111115;
		assert_all_valid validators 399999;
		assert_all_valid validators 333339;
	
		assert_all_invalid validators 10;
		assert_all_invalid validators 43;
		assert_all_invalid validators 123454;
		assert_all_invalid validators 123450;
		assert_all_invalid validators 123490;
		assert_all_invalid validators 999990
	)

let test_all_valid_3 _ =
	let validators = [ Generators.double_valid; Generators.nondecreasing_valid ] in
	(
		assert_all_valid validators 11;
		assert_all_valid validators 11111;
		assert_all_valid validators 11112;
		assert_all_valid validators 12222;
		assert_all_valid validators 155555;
		assert_all_valid validators 111115;
		assert_all_valid validators 399999;
		assert_all_valid validators 333339;
	
		assert_all_invalid validators 6;
		assert_all_invalid validators 123456;
		assert_all_invalid validators 100121;
		assert_all_invalid validators 133221;
		assert_all_invalid validators 10;
		assert_all_invalid validators 12;
		assert_all_invalid validators 39;
		assert_all_invalid validators 43;
		assert_all_invalid validators 123454;
		assert_all_invalid validators 123450;
		assert_all_invalid validators 123490;
		assert_all_invalid validators 999990
	)

let assert_next generators after range_max expect =
(* not testing directly [next_generated_pass] *)
	let next = next_all_validated_pass
		(Pass.of_int after)
		(Pass.of_int range_max)
		[] generators in
	assert_equal expect next

let test_next_1 _ =
	let generators = [ Generators.double_next ] in
	(
		assert_next generators 0      300000 (Some (Pass.of_int 11    ));
		assert_next generators 8      300000 (Some (Pass.of_int 11    ));
		assert_next generators 11     300000 (Some (Pass.of_int 22    ));
		assert_next generators 2343   300000 (Some (Pass.of_int 2344  ));
		assert_next generators 2344   300000 (Some (Pass.of_int 2355  ));
		assert_next generators 2348   300000 (Some (Pass.of_int 2355  ));
		assert_next generators 2350   300000 (Some (Pass.of_int 2355  ));
		assert_next generators 2250   300000 (Some (Pass.of_int 2251  ));
		assert_next generators 2251   300000 (Some (Pass.of_int 2252  ));
		assert_next generators 2255   300000 (Some (Pass.of_int 2256  ));
		assert_next generators 42250  300000 (Some (Pass.of_int 42251 ));
		assert_next generators 42251  300000 (Some (Pass.of_int 42252 ));
		assert_next generators 42255  300000 (Some (Pass.of_int 42256 ));
		assert_next generators 12349  300000 (Some (Pass.of_int 12355 ));
		assert_next generators 12349  12355 (Some (Pass.of_int 12355 ));
		assert_next generators 12349  12354 None;
		assert_next generators 12349  12349 None;
		assert_next generators 12349  20 None;
	)
	

let test_next_2 _ =
	let generators = [ Generators.nondecreasing_next ] in
	(
		assert_next generators 0       300000 (Some (Pass.of_int 1      ));
		assert_next generators 4       300000 (Some (Pass.of_int 5      ));
		assert_next generators 9       300000 (Some (Pass.of_int 11     ));
		assert_next generators 11      300000 (Some (Pass.of_int 12     ));
		assert_next generators 300     300000 (Some (Pass.of_int 333    ));
		assert_next generators 311     300000 (Some (Pass.of_int 333    ));
		assert_next generators 313     300000 (Some (Pass.of_int 333    ));
		assert_next generators 319     300000 (Some (Pass.of_int 333    ));
		assert_next generators 329     300000 (Some (Pass.of_int 333    ));
		assert_next generators 330     300000 (Some (Pass.of_int 333    ));
		assert_next generators 333     300000 (Some (Pass.of_int 334    ));
		assert_next generators 334     300000 (Some (Pass.of_int 335    ));
		assert_next generators 339     300000 (Some (Pass.of_int 344    ));
		assert_next generators 109109  300000 (Some (Pass.of_int 111111 ));
		assert_next generators 50982   300000 (Some (Pass.of_int 55555  ));
		assert_next generators 50982   20 None;
	)
	

let test_next_3 _ =
	let generators = [ Generators.double_next; Generators.nondecreasing_next ] in
	(
		assert_next generators 0       300000 (Some (Pass.of_int 11    ));
		assert_next generators 4       300000 (Some (Pass.of_int 11    ));
		assert_next generators 9       300000 (Some (Pass.of_int 11    ));
		assert_next generators 11      300000 (Some (Pass.of_int 22    ));
		assert_next generators 300     300000 (Some (Pass.of_int 333   ));
		assert_next generators 311     300000 (Some (Pass.of_int 333   ));
		assert_next generators 313     300000 (Some (Pass.of_int 333   ));
		assert_next generators 319     300000 (Some (Pass.of_int 333   ));
		assert_next generators 329     300000 (Some (Pass.of_int 333   ));
		assert_next generators 330     300000 (Some (Pass.of_int 333   ));
		assert_next generators 333     300000 (Some (Pass.of_int 334   ));
		assert_next generators 334     300000 (Some (Pass.of_int 335   ));
		assert_next generators 339     300000 (Some (Pass.of_int 344   ));
		assert_next generators 109109  300000 (Some (Pass.of_int 111111));
		assert_next generators 50982   300000 (Some (Pass.of_int 55555 ));

		assert_next generators 2343    300000 (Some (Pass.of_int 2344  ));
		assert_next generators 2344    300000 (Some (Pass.of_int 2355  ));
		assert_next generators 2348    300000 (Some (Pass.of_int 2355  ));
		assert_next generators 2350    300000 (Some (Pass.of_int 2355  ));
		assert_next generators 2250    300000 (Some (Pass.of_int 2255  ));
		assert_next generators 2251    300000 (Some (Pass.of_int 2255  ));
		assert_next generators 2255    300000 (Some (Pass.of_int 2256  ));
		assert_next generators 42250   300000 (Some (Pass.of_int 44444 ));
		assert_next generators 12349   300000 (Some (Pass.of_int 12355 ));
		assert_next generators 12349   20 None;
	)

let test_count _ =
	let validators = [ Generators.double_valid; Generators.nondecreasing_valid ] in
	let generators = [ Generators.double_next; Generators.nondecreasing_next ] in
	let assert_count count range_min range_max =
		let result = passwords_in_range_count range_min range_max validators generators in
		assert_equal count result ~printer:string_of_int
	in (
		assert_count 0 1 8;
		assert_count 1 1 11;
		assert_count 1 11 11;
		assert_count 1 11 12;
		assert_count 2 11 22;
		assert_count 3 1 40;
		assert_count 10 100 122;
	)
	
let suite = 
	"Solution tests" >::: [
		"arbitration" >:: test_arbitration;
		"multiple validators: 1. one" >:: test_all_valid_1;
		"multiple validators: 2. another" >:: test_all_valid_2;
		"multiple validators: 3. two" >:: test_all_valid_3;
		"multiple generators: 1. one" >:: test_next_1;
		"multiple generators: 2. another" >:: test_next_2;
		"multiple generators: 3. two" >:: test_next_3;
		"counting" >:: test_count
	]

let () =
	run_test_tt_main suite

