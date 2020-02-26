open Solution


let range_of_argv () =
	if (Array.length Sys.argv) != 3 then
		Error "Missing or wrong number of args: need two numbers [range_min] and [range_max]"
	else try
		let range_a = int_of_string Sys.argv.(1) in
		let range_b = int_of_string Sys.argv.(2) in
		if (range_a < 0 || range_a > 999999 ||range_b < 0 || range_b > 999999) then
			Error "Bad args: out of range; need two positive numbers under a million"
		else
			if (range_a > range_b) then
				Ok (range_b, range_a)
			else
				Ok (range_a, range_b)
	
	with Failure _ ->
		Error "Bad args: need two numbers [range_min] and [range_max]"


let () =
	let range = range_of_argv () in
	match range with
	| Error e -> print_endline e
	| Ok (range_min, range_max) ->
		let validators = [ Generators.double_valid; Generators.nondecreasing_valid ] in
		let generators = [ Generators.double_next; Generators.nondecreasing_next ] in
		let result = passwords_in_range_count  range_min range_max validators generators in
		Printf.printf "found passwords: %d\n" result

