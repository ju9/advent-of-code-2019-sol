
(*
Requires: Pass.t = int list
*)


type t_fun_next = Pass.t -> Pass.t
type t_fun_valid = Pass.t -> bool



let rec double_valid : t_fun_valid = function
	| [] | [_] -> false
	| p::(x::_ as tail) ->
		if (p = x) then true
		else double_valid tail


let double_next pass =
	let rec closest_double pass =
		match pass with
		| [] | [_] -> [1;1]
		| p::(x::xs as tail) ->
			if (p < x) then
			(* units can catch up to tens *)
				(x::tail)
			
			else if (x <= 9) then
			(* need to increment tens, but is still below 90 *)
				let xinc = x + 1 in
				(xinc::xinc::xs)
			
			else 
			(* need spill to higher orders, as tens are overflowing now *)
				(0::(closest_double tail))
				(* NOTE: this is clearly invalid for the nondecreasing generator *)
			
	in
	let succ = Pass.succ pass in
	if double_valid succ then succ
	else closest_double succ


let rec nondecreasing_valid : t_fun_valid = function
	| [] | [_] -> true
	| p::x::xs ->
		if (p >= x) then nondecreasing_valid (x::xs)
		else false


let nondecreasing_next pass =
	let rec repeats digit size_ref =
		match size_ref with
		| [] -> []
		| _::xs -> digit :: (repeats digit xs)
	in
	let rec nondec_step_rev rpass =
		match rpass with
		| [] | [_] -> rpass
		| f::(s::_ as tail) ->
			if (f > s) then f :: (repeats f tail)
			else f :: (nondec_step_rev tail)
	in
	let fix_nondec x = x |> List.rev |> nondec_step_rev |> List.rev in
	let nondec = fix_nondec pass in
	(* got non-decreasing pass now, but not sure if it is the successor,
		or the same pass *)
	if (Pass.compare pass nondec) = -1 then nondec
	else nondec |> Pass.succ |> fix_nondec
	(* the [Pass.succ] may have scrambled other digits than LSD;
		need to re-process with the inner fixer-function *)


let succ_next pass =
	Pass.succ pass
