open Solution

(* run as `$0 FILE`, where FILE is path to input file, STDIN is used for keyboard input, STDOUT is outputs. *)

let input_of_argv () =
	if (Array.length Sys.argv) != 2 then
		Error "Missing or wrong number of args: need one path to input file."
	else
		let path = Sys.argv.(1) in
		if not (Sys.file_exists path) then
			Error "The given file does not exist."
		else if Sys.is_directory path then
			Error "The given file name refers to a directory; a file is needed."
		else
			Ok path

let load_file path =
	let channel = open_in path in
	let data = input_line channel in
	close_in channel;
	data

let () =
	let input = input_of_argv () in
	match input with
	| Error e -> print_endline e
	| Ok path ->
		let data = load_file path in
		let output = run_program data in
		Printf.printf "Program done, output: %d\n" output