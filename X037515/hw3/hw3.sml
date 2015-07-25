(* Dan Grossman, CSE341 Spring 2013, HW3 Provided Code *)

exception NoAnswer

datatype pattern = Wildcard
                 | Variable of string
                 | UnitP
                 | ConstP of int
                 | TupleP of pattern list
                 | ConstructorP of string * pattern

datatype valu = Const of int
              | Unit
              | Tuple of valu list
              | Constructor of string * valu

fun g f1 f2 p =
    let 
        val r = g f1 f2 
    in
        case p of
            Wildcard          => f1 ()
          | Variable x        => f2 x
          | TupleP ps         => List.foldl (fn (p,i) => (r p) + i) 0 ps
          | ConstructorP(_,p) => r p
          | _                 => 0
    end

(**** for the challenge problem only ****)

datatype typ = Anything
             | UnitT
             | IntT
             | TupleT of typ list
             | Datatype of string

(**** you can put all your code here ****)

val only_capitals = List.filter (fn s => Char.isUpper (String.sub(s, 0)));

val longest_string1 =
  foldl (fn (x, y) => if String.size x > String.size y then x else y) ""
val longest_string2 =
  foldl (fn (x, y) => if String.size y > String.size x then y else x) ""

fun longest_string_helper (cmp: int * int -> bool) =
  foldl (fn (x, y) => if cmp (String.size x, String.size y) then x else y) ""

val longest_string3 = longest_string_helper Int.>
val longest_string4 = longest_string_helper Int.>=

val longest_capitalized = longest_string_helper Int.>= o only_capitals
val rev_string = implode o rev o explode

fun first_answer f l = case List.find isSome (map f l) of
  SOME v => valOf v | _ => raise NoAnswer

fun all_answers f l = SOME (foldl List.@ nil (map (valOf o f) l))
  handle _ => NONE

val count_wildcards = g (fn () => 1) (fn _ => 0)
val count_wild_and_variable_lengths = g (fn () => 1) String.size
fun count_some_var (s, p) = g (fn _ => 0) (fn t => if t = s then 1 else 0) p
fun check_pat p =
  g (fn _ => 0) (fn t => if count_some_var (t, p) > 1 then 1 else 0) p = 0

fun match (v, p) = let
  fun m (_, Wildcard) = nil
    | m (v, Variable s) = [(s, v)]
    | m (Unit, UnitP) = nil
    | m (Const x, ConstP y) = if x = y then nil else raise Match
    | m (Tuple vs, TupleP ps) =
      foldl List.@ [] (map m (ListPair.zipEq (vs, ps)))
    | m (Constructor (s1, v), ConstructorP (s2, p)) = 
      if s1 = s2 then m (v, p) else raise Match
    | m _ = raise Match
in
  SOME (m (v, p)) handle _ => NONE
end

fun first_match v p = SOME (first_answer (fn p => match (v, p)) p)
  handle _ => NONE

fun typecheck_patterns (cons_list, patterns) = let
  fun common (Anything: typ, t2: typ) = t2
    | common (t1, Anything) = t1
    | common (TupleT l1, TupleT l2) =
        TupleT (map common (ListPair.zipEq (l1, l2)))
    | common (t1, t2) =
        if t1 = t2 then t1 else raise Match
  fun typed UnitP = UnitT
    | typed (ConstP _) = IntT
    | typed (TupleP l) = TupleT (map typed l)
    | typed (ConstructorP (s, p)) = let
      val (_, y, z) = valOf (List.find (fn (x, _, _) => x = s) cons_list)
      val pt = typed p
    in if common (z, pt) = z then Datatype y else raise Match end
    | typed _ = Anything
in
  SOME (foldl common Anything (map typed patterns)) handle _ => NONE
end
