
open Solution

(* run as `$0 FILE`, where FILE is path to input file, STDIN is used for keyboard input, STDOUT is outputs. *)


let () =
	let table = parse_input read_line in
	let checksum = compute_checksum table in
	print_endline (string_of_int checksum)
