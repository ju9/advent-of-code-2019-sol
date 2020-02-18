type t = int list

val to_int : t -> int
val of_int : int -> t
val compare : t -> t -> int (* like "a - b", bounded to [-1, 0, 1] *)
val max : t -> t -> t
val succ : t -> t
