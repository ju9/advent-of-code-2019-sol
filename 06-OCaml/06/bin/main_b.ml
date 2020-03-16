
open Solution

let () =
	let table = parse_input read_line in
	let tcount = transfer_count table "YOU" "SAN" in
	print_endline (string_of_int tcount)
