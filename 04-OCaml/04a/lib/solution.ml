
(** The accumulative result of comparing outputs of different generators. *)
type pass_arb_verdict = 
	  InAgreement of Pass.t
	| Different of Pass.t
	| PANone


let pass_arbitration  acc res =
	match (acc, res) with
	| (PANone, r) -> InAgreement r
	| (InAgreement a, r) when (a = r) -> acc
	| (InAgreement a, r) | (Different a, r) -> Different (Pass.max a r)


let rec all_rules_valid pass validators =
	match validators with
	| [] -> true
	| h::t ->
		if (h pass) then all_rules_valid pass t
		else false
	

let rec next_generated_pass  after pass_arbd generators =
	
	match generators with
	| [] -> pass_arbd
	| h::t -> 
		let next_gend = h after in
		let next_arbd = pass_arbitration  pass_arbd next_gend in
		next_generated_pass  after next_arbd t


let rec next_all_validated_pass  after pass_limit_max validators generators =

	if ((Pass.compare after pass_limit_max) >= 0) then None
	else
		let pass_resolution = next_generated_pass  after PANone generators in
		match pass_resolution with
		| PANone -> None
		
		| InAgreement pass | Different pass
			when ((Pass.compare pass pass_limit_max) > 0) -> None
			
		| InAgreement pass -> Some pass
		
		| Different pass ->
			if all_rules_valid  pass validators then
				Some pass
			else
				next_all_validated_pass  pass pass_limit_max validators generators

		
	



let passwords_in_range_count  range_min range_max validators generators =
	
	let rec counter  after until acc = 
		let pass = next_all_validated_pass  after until validators generators  in
		match pass with
		| None -> acc
		| Some pass ->
			(*Printf.printf "pass: %d\n" (Pass.to_int pass);*) (* debug *)
			counter  pass until (acc + 1)
		(* None ~ counting halted; return current count;
		   Some pass ~ found something, note count++,
		   + and count more from here onwards
		 *)
	in
	let after = Pass.of_int (range_min - 1) in
	let pass_limit_max = Pass.of_int range_max in
	counter  after pass_limit_max 0
		

	
