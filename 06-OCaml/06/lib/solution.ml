
(*
	failwith "not implemented"
*)

let parse_orbit orbit =
	match (String.split_on_char ')' orbit) with
	| center :: satellite :: [] -> center, satellite
	| _ -> failwith ("input error, expected 'AAA)BBB', got " ^ orbit)



let push_center table center satellite =
(* add a new center with this satellite,
+	or add this satellite to an existing center *)
	match Hashtbl.find_opt table center with
	| None ->
		Hashtbl.add table center [satellite]
	| Some sats ->
		Hashtbl.replace table center (satellite::sats)

let push_satellite table satellite =
(* only add if not present; don't populate its own satellites *)
	if Hashtbl.find_opt table satellite = None then
		Hashtbl.add table satellite []
	else
		()

let parse_input reader =
	let table = Hashtbl.create 64 in
	let push orbit k =
		let (center, satellite) = parse_orbit orbit in
		push_center table center satellite;
		push_satellite table satellite;
		k ()
	in
	let rec loop () =
		try push (reader ()) loop with
		| End_of_file -> table
	in
	loop ()

let compute_checksum table =
	let rec counter acc depth center =
		let satellites = Hashtbl.find table center in
		depth (* add depth of each item to the sum *)
		+
		List.fold_left
			(fun lacc item -> counter lacc (depth + 1) item)
				(* run over each satellite item, at depth + 1 *)
			acc (* start count from where the last count left of (computing a sum) *)
			satellites (* run over all satellites of this center *)
	in
	counter 0 0 "COM"
	(* note: This recursion probbly won't tail-optimize. Can we rewrite it? *)
	