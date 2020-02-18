type t = int list

let to_int pass =
	(* returns int; accumulates int; magnifies int;
		consumes argument Pass.t *)
	let rec to_int_digit acc mag arg =
		match arg with
		|	[] -> acc
		|	x::xs -> to_int_digit (acc + (mag * x)) (mag * 10) xs
	in
	to_int_digit 0 1 pass

(* Requires: pass_val >= 0 *)
let of_int pass_val =
	(* returns Pass.t; accumulates Pass.t; consumes argument int *)
	(* builds in reverse; result is then reversed back *)
	let rec of_int_digit acc arg =
		if arg < 10 then
			arg :: acc
		else
			let digit = arg mod 10 in
			let arg_shifted = arg / 10 in
			of_int_digit (digit :: acc) arg_shifted
	in
	List.rev (of_int_digit [] pass_val)
	

let compare a b =
	(* returns [-1,0,1]; accumulates [-1,0,1]; consumes two arguments Pass.t *)
	(* -1 is a<b; 0 is a=b; 1 is a>b *)
	let rec compare_digit acc a b =
		match a, b with
		|	[], [] -> acc
		|	[], _ -> -1
		|	_, [] -> 1
		|	xa::xsa, xb::xsb ->
			let digit_comp = compare xa xb in
			let carry_comp = if digit_comp = 0 then acc else digit_comp in
			compare_digit carry_comp xsa xsb
	in
	compare_digit 0 a b

let max a b =
	if compare a b = -1 then b else a

let succ pass =
	(* returns Pass.t; accumulates Pass.t; consumes argument Pass.t *)
	let rec succ_digit acc arg =
		match arg with
		|	[] -> acc @ [1]
			(* after overflowed 9 at MSD as in 9->10 or 99->100 etc. *)
			
		|	x::xs when (x = 9) -> succ_digit (acc @ [0]) xs
			(* overflowed 9 *)
			
		|	x::xs -> acc @ (x + 1) :: xs
			(* incrementing lower digit *)
	in
	succ_digit [] pass