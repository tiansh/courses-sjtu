use "hw3.sml";

only_capitals ["abc", "A", "Ddd"]; (* ["A", "Ddd"] *)

only_capitals []; (* [] *)

longest_string1 []; (* "" *)

longest_string1 ["abc", "A", "Ddd"]; (* "abc" *)

longest_string2 ["abc", "A", "Ddd"]; (* "Ddd" *)

longest_string3 []; (* "" *)

longest_string4 []; (* "" *)

longest_string3 ["abc", "A", "Ddd"];  (* "abc" *)

longest_string4 ["abc", "A", "Ddd"];  (* "Ddd" *)

longest_capitalized ["abc", "A", "Ddd"];  (* "Ddd" *)

longest_capitalized ["abc", "a", "bbb"]; (* "" *)

longest_capitalized []; (* "" *)

rev_string ""; (* "" *)

rev_string "abcdef"; (* "fedcba" *)

fun f1 x = 
    case x of 
	false => NONE
      | true => SOME 1;

fun f2 x = 
    case x of 
	false => NONE
      | true => SOME [1,2];

(* first_answer f1 []; (* raise NoAnswer *) *)
(* first_answer f1 [false]; (* raise NoAnswer *) *)
first_answer f1 [false, false, true]; (* 1 *)


all_answers f2 []; (* SOME [] *)
all_answers f2 [true, true, false]; (* NONE *)
all_answers f2 [true, true, true]; (* SOME [1,2,1,2,1,2] *)

val p1 = Wildcard;  
val p2 = Variable "var1";
val p3 = UnitP;
val p4 = ConstP  7;
val p5 = ConstructorP ("lala", p1);
val p6 = TupleP [p1,p1,p2,p3,p5]
val ps = [p1,p2,p3,p4,p5,p6]; 
List.map count_wildcards ps; (* [1, 0, 0, 0, 1, 3]*)

List.map count_wild_and_variable_lengths ps; (* [1, 4, 0, 0, 1, 7]*)

List.map (fn x => count_some_var ("var1",x)) ps; (* [0, 1, 0, 0, 0, 1]*)
List.map (fn x => count_some_var ("var2",x)) ps; (* [0, 0, 0, 0, 0, 0]*)

val p7 = TupleP [p6, p2]; 
check_pat p7; (* false *)
check_pat p6; (* true *)

val v1 = Const 7;
val v2 = Unit;
val v3 = Constructor ("lala", v1);
val v4 = Tuple [v1,v2,v3];
val p8 = TupleP [p1,p1]
val p9 = ConstructorP ("lala", p2);
val p10 = TupleP [p2, p3, p9];


match (v4,p8); (* NONE *)
match (v4,p2); (* SOME [("var1", v4)] *)
match (v4,p10); (* SOME [("var1", v1), ("var1", v1)] *)
match (v4,p9); (* NONE *)

first_match v4 [p8,p2,p10,p9]; (* SOME [("var1", v4)] *)
first_match v4 []; (* NONE *)
first_match v4 [p8, p10, p2, p9]; (* SOME [("var1", v1), ("var1", v1)] *)


(* SOME Anything *)
typecheck_patterns ([], []);

(* SOME (TupleT [Anything,Anything]) *)
typecheck_patterns ([], [TupleP[Wildcard,Wildcard]]);

(* SOME IntT *)
typecheck_patterns ([], [Wildcard, ConstP 42]);

(* SOME (Datatype "bar") *)
typecheck_patterns ([("foo", "bar", IntT)], [ConstructorP ("foo", ConstP 42)]);

(* SOME (Datatype "bar") *)
typecheck_patterns ([("foo", "bar", IntT)], [ConstructorP ("foo", Wildcard)]);

(* NONE *)
typecheck_patterns ([("foo", "bar", Anything)], [ConstructorP ("foo", ConstP 42)]);

(* SOME (Datatype "bar") *)
typecheck_patterns (
  [("foo1", "bar", IntT), ("foo2", "bar", IntT)],
  [ConstructorP ("foo1", ConstP 42), ConstructorP ("foo2", ConstP 42)]
);

