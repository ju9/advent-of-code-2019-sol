
type t_fun_next = Pass.t -> Pass.t
type t_fun_valid = Pass.t -> bool

val double_valid : t_fun_valid

val consec_repeats : int -> Pass.t -> int -> (int * Pass.t)

val strict_double_valid : t_fun_valid

val strict_double_next : t_fun_next

val double_next : t_fun_next

val nondecreasing_valid : t_fun_valid

val nondecreasing_next : t_fun_next

val succ_next : t_fun_next