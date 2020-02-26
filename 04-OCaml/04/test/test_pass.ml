open OUnit2


let test_int_conv _ =
	let test_int x =
		assert_equal x (x |> Pass.of_int |> Pass.to_int) ~printer:string_of_int
	in (
		test_int 0;
		test_int 1;
		test_int 2;
		test_int 10;
		test_int 15;
		test_int 239;
		test_int 700;
		test_int 296409;
		test_int 329034;
	)

let test_compare _ =
	let test_eq a b =
		assert_equal 0 (Pass.compare (Pass.of_int a) (Pass.of_int b)) ~printer:string_of_int
	in
	let test_lt a b =
		assert_equal (-1) (Pass.compare (Pass.of_int a) (Pass.of_int b)) ~printer:string_of_int
	in
	let test_gt a b =
		assert_equal 1 (Pass.compare (Pass.of_int a) (Pass.of_int b)) ~printer:string_of_int
	in (
		test_eq 0 0;
		test_eq 1 1;
		test_eq 2 2;
		test_eq 20 20;
		test_eq 678923 678923;
		test_lt 0 1;
		test_lt 0 2;
		test_lt 0 20;
		test_lt 0 678923;
		test_lt 1 2;
		test_lt 1 20;
		test_lt 1 678923;
		test_lt 2 20;
		test_lt 2 678923;
		test_lt 43566 678923;
		test_lt 678922 678923;
		test_lt 668923 678923;
		test_lt 578923 678923;
		test_gt 1 0;
		test_gt 2 0;
		test_gt 20 0;
		test_gt 678923 0;
		test_gt 2 1;
		test_gt 20 1;
		test_gt 678923 1;
		test_gt 20 2;
		test_gt 678923 2;
		test_gt 678923 43566;
		test_gt 678923 668923;
		test_gt 678923 578923;
		
		test_gt 222 213;
		test_lt 222 231;
	)

let test_max _ =
	let test_lt a b =
		let pb = (Pass.of_int b) in
		assert_equal pb (Pass.max (Pass.of_int a) pb)
	in
	let test_ge a b =
		let pa = (Pass.of_int a) in
		assert_equal pa (Pass.max pa (Pass.of_int b))
	in (
		test_ge 0 0;
		test_ge 1 1;
		test_ge 2 2;
		test_ge 20 20;
		test_ge 678923 678923;
		test_lt 0 1;
		test_lt 0 2;
		test_lt 0 20;
		test_lt 0 678923;
		test_lt 1 2;
		test_lt 1 20;
		test_lt 1 678923;
		test_lt 2 20;
		test_lt 2 678923;
		test_lt 43566 678923;
		test_lt 678922 678923;
		test_lt 668923 678923;
		test_lt 578923 678923;
		test_ge 1 0;
		test_ge 2 0;
		test_ge 20 0;
		test_ge 678923 0;
		test_ge 2 1;
		test_ge 20 1;
		test_ge 678923 1;
		test_ge 20 2;
		test_ge 678923 2;
		test_ge 678923 43566;
		test_ge 678923 668923;
		test_ge 678923 578923;
	)

let test_fun_next _ =
	let test_int x =
		assert_equal (x + 1) (x |> Pass.of_int |> Pass.succ |> Pass.to_int) ~printer:string_of_int
	in (
		test_int 0;
		test_int 1;
		test_int 2;
		test_int 3;
		test_int 8;
		test_int 1199;
		test_int 9;
		test_int 10;
		test_int 20;
		test_int 21;
		test_int 4359;
		test_int 4360;
		test_int 4909;
		test_int 4099;
		test_int 4999;
		test_int 5000;
		test_int 37865;
		test_int 8340537;
	)

let suite = 
	"Password tests" >::: [
		"integer conversions" >:: test_int_conv;
		"comparisons" >:: test_compare;
		"max - bigger of two" >:: test_max;
		"successor" >:: test_fun_next
	]

let () =
	run_test_tt_main suite

