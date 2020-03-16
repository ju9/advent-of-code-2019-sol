
val parse_orbit :
	   string
	-> string * string
(* given one input entry [AAA)BBB], returns names of the two objects:
the local center [AAA] and the satellite [BBB] *)

val parse_input :
	   (unit -> string)
	-> (string, string list) Hashtbl.t
(* given input-entry-generator function,
returns the parsed orbits in a [hash table] as "local center" => "list of satellites" *)

val compute_checksum :
	   (string, string list) Hashtbl.t
	-> int
(* given [hash table] of objects with their satellites, returns a [checksum]
(the number of all direct and indirect orbits) *)