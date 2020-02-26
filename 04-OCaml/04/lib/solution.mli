(** Solution 04A to AOC2019 *)


type pass_arb_verdict = 
	  InAgreement of Pass.t
	| Different of Pass.t
	| PANone


val pass_arbitration :
	   pass_arb_verdict
	-> Pass.t
	-> pass_arb_verdict

val all_rules_valid :
	   Pass.t
	-> Generators.t_fun_valid list
	-> bool

val next_generated_pass :
	   Pass.t
	-> pass_arb_verdict
	-> Generators.t_fun_next list
	-> pass_arb_verdict

val next_all_validated_pass :
	   Pass.t
	-> Pass.t
	-> Generators.t_fun_valid list
	-> Generators.t_fun_next list
	-> Pass.t option

val passwords_in_range_count :
	   int
	-> int
	-> Generators.t_fun_valid list
	-> Generators.t_fun_next list
	-> int

