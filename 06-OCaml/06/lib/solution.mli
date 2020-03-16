
type orbit_hashtbl = (string, string * string list) Hashtbl.t
(* hash table, with string keys, and values being a tuple of a string for the parent node, and a list of strings for child nodes *)
(* special case: what is COM's parent? Nothing! - could use [option], or empty string, or self*)

val com : string
val free : string

val parse_orbit :
	   string
	-> string * string
(* given one input entry [AAA)BBB], returns names of the two objects:
the local center [AAA] and the satellite [BBB] *)

val parse_input :
	   (unit -> string)
	-> orbit_hashtbl
(* given input-entry-generator function,
returns the parsed orbits in a [hash table] as "local center" => "list of satellites" *)

val compute_checksum :
	   orbit_hashtbl
	-> int
(* given [hash table] of objects with their satellites, returns a [checksum]
(the number of all direct and indirect orbits) *)

val get_parent :
	   orbit_hashtbl
	-> string
	-> string
(* given the [table] to search in, and a [center],
returns the center's [parent] *)

val trace_path :
	   orbit_hashtbl
	-> string
	-> string list
(* given the [table] to search in, and a [center],
returns as list the [path] from COM to the center's parent *)

val transfer_count :
	   orbit_hashtbl
	-> string
	-> string
	-> int
(* given the [table] to search in, and [one center] and [other center],
returns the number of orbital transfers needed for the two centers to orbit the same object *)