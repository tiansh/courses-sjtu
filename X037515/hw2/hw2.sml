(* Dan Grossman, CSE341 Spring 2013, HW2 Provided Code *)

fun same_string (s1 : string, s2 : string) = s1 = s2

fun all_except_option (s, nil) = NONE
  | all_except_option (s, c :: rst) =
      if same_string(c, s) then SOME rst
      else case all_except_option(s, rst) of 
          NONE => NONE
        | SOME x => SOME (c :: x)

fun get_substitutions1 (nil, s) = nil
  | get_substitutions1 (l :: rst, s) =
      (case all_except_option(s, l) of NONE => nil | SOME l => l)
      @ get_substitutions1(rst, s)
                      
fun get_substitutions2 (lst, s) = let
  fun helper(nil, r, s) = r
    | helper(l :: rst, r, s) =
    helper(rst,
    r @ (case all_except_option(s, l) of NONE => nil | SOME l => l), s)
in
  helper(lst, nil, s)
end

fun similar_names (lst, {first, middle, last}) = let
  fun helper (nil) = nil
    | helper (first :: rst) = {first = first, middle = middle, last = last} :: helper(rst)
in
  helper(first :: get_substitutions1(lst, first))
end

(* you may assume that Num is always used with values 2, 3, ..., 10
   though it will not really come up *)
datatype suit = Clubs | Diamonds | Hearts | Spades
datatype rank = Jack | Queen | King | Ace | Num of int 
type card = suit * rank

datatype color = Red | Black
datatype move = Discard of card | Draw 

exception IllegalMove

fun card_color ((s, r) : card) = case s of (* why not just use if-then-else *)
    Clubs => Black
  | Diamonds => Red
  | Hearts => Red
  | Spades => Black

fun card_value ((s, r) : card) = case r of
    Num(n) => n
  | Ace => 11
  | _ => 10

fun remove_card (nil, c, e) = raise e
  | remove_card (f :: rc, c, e) =
    if f = c then rc else f :: remove_card(rc, c, e)

fun all_same_color (f :: s :: rc) =
      card_color(f) = card_color(s) andalso all_same_color(s :: rc)
  | all_same_color _ = true

fun sum_cards (cs) = let
  fun helper (nil, s) = s
    | helper (f :: rc, s) = helper(rc, card_value(f) + s)
in
  helper(cs, 0)
end

fun score(hc, goal) = let
  val dis = sum_cards(hc) - goal
  val preliminary = if dis > 0 then 3 * dis else ~dis
  val rat = if all_same_color(hc) then 2 else 1
in
  preliminary div rat
end

fun officiate (cl, ml, goal) = let
  fun helper (_, hc, nil) = score(hc, goal)
    | helper (nil, hc, Draw :: rm) = score(hc, goal)
    | helper (c :: rc, hc, Draw :: rm) = let val nhc = c :: hc in
        if sum_cards(nhc) > goal then score(nhc, goal)
        else helper(rc, nhc, rm)
      end
    | helper (cl, hc, Discard(d) :: rm) =
        helper(cl, remove_card(hc, d, IllegalMove), rm)
in
  helper(cl, nil, ml)
end

fun ace_count (hc) = let
  fun helper (count, nil) = count
    | helper (count, c :: rc) = helper(count + 1, rc)
in
  helper (0, hc)
end

fun score_challenge (hc, goal) = let
  val dis = sum_cards(hc) - goal
  val ace = ace_count(hc)
  fun score (dis) = let
    val preliminary = if dis > 0 then 3 * dis else ~dis
    val rat = if all_same_color(hc) then 2 else 1
  in
    preliminary div rat
  end
  fun helper (dis, 0) = score(dis)
    | helper (dis, ace) =
        Int.min(score (dis), helper(dis - 10, ace - 1))
in
  helper(dis, ace)
end

fun officiate_challenge (cl, ml, goal) = let
  fun helper (_, hc, nil) = score_challenge(hc, goal)
    | helper (nil, hc, Draw :: rm) = score_challenge(hc, goal)
    | helper (c :: rc, hc, Draw :: rm) = let val nhc = c :: hc in
        if sum_cards(nhc) - ace_count(nhc) * 10 > goal then
          score_challenge(nhc, goal)
        else helper(rc, nhc, rm)
      end
    | helper (cl, hc, Discard(d) :: rm) =
        helper(cl, remove_card(hc, d, IllegalMove), rm)
in
  helper(cl, nil, ml)
end


(*
 * 鉴于作业说明说的这么不清楚，所以我按照我的理解复述一遍：
 * 1. 玩家手牌的点数永远不会超过目标；
 * 2. 如果当前的点数和比目标少10以上且牌堆不空则抽牌；
 * 3. 如果当前的分数和为0，则停止；
 * 4. 在2、3不满足时如果可以通过弃掉一张再抽一张达到恰好的点数，那么这么做。
 * 5. 其他情况直接结束
 *)
fun careful_player (cl, goal) = let
  fun map (l, f) = let
    fun helper (l, nil, _) =
      l | helper (l, h :: t, f) = helper(l @ [f(h)], t, f)
  in helper(nil, l, f) end
  fun reduce (h :: nil, _) = h
    | reduce (h :: n :: l, f) = reduce(f(h, n) :: l, f)
  fun ohws (cl, hc, nil) = (score(hc, goal), cl, hc, true)
    | ohws (nil, hc, Draw :: rm) = (score(hc, goal), nil, hc, false)
    | ohws (c :: rc, hc, Draw :: rm) = let val nhc = c :: hc in
        if sum_cards(nhc) > goal then (0, rc, nhc, false) else ohws(rc, nhc, rm)
      end
    | ohws (cl, hc, Discard(d) :: rm) =
        ohws(cl, remove_card(hc, d, IllegalMove), rm)
  fun step (ml) = let
    val (ns, nl, nh, nf) = ohws(cl, nil, ml)
    val (ds, dl, dh, df) = ohws(cl, nil, ml @ [Draw])
    val ((ss, sl, sh, sf), sc) = let
      fun h(c) = (ohws(cl, nil, ml @ [Discard(c), Draw]), c)
      fun b (((s1, r1, h1, f1), c1), ((s2, r2, h2, f2), c2)) =
        if not f2 orelse s1 < s2 then ((s1, r1, h1, f1), c1)
        else ((s2, r2, h2, f2), c2)
    in reduce(map(nh, h), b) end
    handle Match => ((0, [], [], false), (Hearts, Ace))
  in
    if sum_cards(nh) + 10 < goal then
      if not df then ml else step(ml @ [Draw])
    else if ns = 0 then ml
    else if sf andalso ss = 0 then step(ml @ [Discard(sc)])
    else ml
  end
in
  step (nil)
end


