
(*
	failwith "not implemented"
*)

type orbit_hashtbl = (string, string * string list) Hashtbl.t

let com = "COM"
let free = ""

let parse_orbit orbit =
	match (String.split_on_char ')' orbit) with
	| center :: satellite :: [] -> center, satellite
	| _ -> failwith ("input error, expected 'AAA)BBB', got " ^ orbit)


let push_center table center satellite =
(* add a new [center] with this [satellite] as its child,
+	or add this [satellite] to an existing [center] *)
	match Hashtbl.find_opt table center with
	| None ->
		Hashtbl.add table center (free, satellite::[])
	| Some (parent, sats) ->
		Hashtbl.replace table center (parent, satellite::sats)


let push_satellite table center satellite =
(* add a new [satellite] under existing [center],
+	or bind it to the [center] if the sat. already exists *)
	match Hashtbl.find_opt table satellite with
	| None ->
		Hashtbl.add table satellite (center, [])
	| Some (_, sats) ->
		Hashtbl.replace table satellite (center, sats)


let parse_input reader =
	let table = Hashtbl.create 256 in
	let push orbit k =
		let (center, satellite) = parse_orbit orbit in
		push_center table center satellite;
		push_satellite table center satellite;
		k ()
	in
	let rec loop () =
		try push (reader ()) loop with
		| End_of_file -> table
	in
	loop ()


let compute_checksum table =
	let rec counter acc depth node =
		let _, satellites = Hashtbl.find table node in
		depth (* add depth of each item to the sum *)
		+
		List.fold_left
			(fun lacc item -> counter lacc (depth + 1) item)
				(* run over each satellite item, at depth + 1 *)
			acc (* start count from where the last count left of (computing a sum) *)
			satellites (* run over all satellites of this node *)
	in
	counter 0 0 com
	(* note: This recursion probbly won't tail-optimize. Can we rewrite it? *)
	

let get_parent table node =
	let parent, _ = Hashtbl.find table node in
	parent


let trace_path table node =
	let rec tracer acc node =
		let parent = get_parent table node in
		if parent = free then
			acc
		else
			tracer (parent::acc) parent
	in
	tracer [] node

let transfer_count table a b =
	let pa = trace_path table a in
	let pb = trace_path table b in
	let rec short_count pa pb =
		match pa, pb with
		| ha::ta, hb::tb when ha = hb ->
			short_count ta tb
			(* both paths are nonempty, and start at the same node: can shorten *)
		| _, _ ->
			(List.length pa) + (List.length pb)
			(* different paths, or any empty: count up what's left *)
	in
	short_count pa pb
	
