val result = let
  fun forEach (nil) = true
    | forEach (test :: tests) = test() andalso forEach(tests)
  fun test_all_except_option() =
    all_except_option("s", ["s"]) = SOME [] andalso
    all_except_option("s", ["a"]) = NONE andalso
    all_except_option("s", ["s", "a"]) = SOME ["a"]
  fun test_get_substitutions(ver) = let
    fun test() =
      ver([], "a") = [] andalso
      ver([[]], "a") = [] andalso
      ver([["x"]], "a") = [] andalso
      ver([["a"]], "a") = [] andalso
      ver([["a", "b"]], "a") = ["b"] andalso
      ver([["b", "a"]], "a") = ["b"] andalso
      ver([["a", "b"], ["b", "a"]], "a") = ["b", "b"] andalso
      ver([["a", "b", "c"], ["b", "a"]], "a") = ["b", "c", "b"]
  in test end
  fun test_similar_names() =
    similar_names([], {first="a", middle="p", last="q"}) =
      [{first="a", middle="p", last="q"}] andalso
    similar_names([["a", "b"], ["x", "y"], ["c", "a", "d"]],
      {first="a", middle="p", last="q"}) = [
        {first="a", middle="p", last="q"},
        {first="b", middle="p", last="q"},
        {first="c", middle="p", last="q"},
        {first="d", middle="p", last="q"}
      ]
  fun test_card_color() =
    card_color(Clubs, Ace) = Black andalso
    card_color(Diamonds, Ace) = Red andalso
    card_color(Hearts, Ace) = Red andalso
    card_color(Spades, Ace) = Black
  fun test_card_value() =
    card_value(Clubs, Ace) = 11 andalso
    card_value(Clubs, Num(5)) = 5 andalso
    card_value(Clubs, King) = 10
  fun test_remove_card() = let exception E in
    (remove_card([], (Hearts, Ace), E) = [] andalso false)
      handle E => true andalso
    (remove_card([(Clubs, Ace)], (Hearts, Ace), E) = [] andalso false)
      handle E => true andalso
    (remove_card([(Hearts, Ace)], (Hearts, Ace), E) = [])
      handle E => false andalso
    (remove_card([(Hearts, Ace), (Hearts, Ace)], (Hearts, Ace), E) =
      [(Hearts, Ace)]) handle E => false andalso
    (remove_card([(Hearts, King), (Hearts, Ace)], (Hearts, King), E) =
      [(Hearts, Ace)]) handle E => false
  end
  fun test_all_same_color() =
    all_same_color([]) = true andalso
    all_same_color([(Clubs, Ace)]) = true andalso
    all_same_color([(Spades, Ace)]) = true andalso
    all_same_color([(Clubs, Ace), (Hearts, Ace)]) = false andalso
    all_same_color([(Clubs, Ace), (Spades, Ace)]) = true andalso
    all_same_color([(Spades, Ace), (Clubs, Ace)]) = true
  fun test_sum_cards() =
    sum_cards([]) = 0 andalso
    sum_cards([(Clubs, Ace)]) = 11 andalso
    sum_cards([(Clubs, King), (Clubs, Ace)]) = 21 andalso
    sum_cards([(Clubs, King), (Clubs, Ace), (Hearts, Num(3))]) = 24
  fun test_score() =
    score([], 3) = 1 andalso
    score([(Clubs, Ace)], 3) = 12 andalso
    score([(Clubs, Ace), (Spades, Ace)], 20) = 3 andalso
    score([(Clubs, Ace), (Hearts, Ace)], 20) = 6 andalso
    score([(Clubs, Ace), (Clubs, Ace), (Clubs, Ace)], 33) = 0
  fun test_officiate() =
    (officiate([], [], 3) = 1) handle IllegalMove => false andalso
    (officiate([], [Draw], 3) = 1) handle IllegalMove => false andalso
    (officiate([(Clubs, Ace)], [Draw], 3) = 21) handle IllegalMove => false andalso
    (officiate([(Clubs, Ace), (Clubs, Num(3))], [Draw, Draw], 3) = 12)
      handle IllegalMove => false andalso
    (officiate([(Clubs, Ace), (Clubs, Num(3))],
      [Draw, Discard(Clubs, Ace), Draw], 3) = 12)
      handle IllegalMove => false andalso
    (officiate([(Clubs, Ace), (Clubs, Num(3))],
      [Discard(Clubs, Ace), Draw, Draw], 3) = 0 andalso false)
      handle IllegalMove => true andalso
    (officiate(
      [(Clubs, Ace), (Clubs, Ace), (Clubs, Ace)],
      [Draw, Draw, Draw], 33) = 0) handle IllegalMove => false
  fun test_score_challenge() =
    score_challenge([], 3) = 1 andalso
    score_challenge([(Clubs, Ace)], 3) = 1 andalso
    score_challenge([(Clubs, Ace), (Spades, Ace)], 20) = 3 andalso
    score_challenge([(Clubs, Ace), (Spades, Ace), (Spades, Num(7))], 20) = 0 andalso
    score_challenge([(Clubs, Ace), (Hearts, Ace)], 20) = 6 andalso
    score_challenge([(Clubs, Ace), (Hearts, Ace), (Hearts, Num(6))], 20) = 2 andalso
    score_challenge([(Clubs, Ace), (Clubs, Ace), (Clubs, Ace)], 33) = 0 andalso
    score_challenge([(Clubs, Ace), (Clubs, Ace), (Clubs, Ace)], 13) = 0
  fun test_officiate_challenge() =
    (officiate_challenge([], [], 3) = 1) handle IllegalMove => false andalso
    (officiate_challenge([], [Draw], 3) = 1) handle IllegalMove => false andalso
    (officiate_challenge([(Clubs, Ace)], [Draw], 3) = 1) handle IllegalMove => false andalso
    (officiate_challenge([(Clubs, Ace), (Clubs, Num(3))], [Draw, Draw], 3) = 1)
      handle IllegalMove => false andalso
    (officiate_challenge([(Clubs, Ace), (Clubs, Num(3))],
      [Draw, Discard(Clubs, Ace), Draw], 3) = 0)
      handle IllegalMove => false andalso
    (officiate_challenge([(Clubs, Ace), (Clubs, Num(3))],
      [Discard(Clubs, Ace), Draw, Draw], 3) = 0 andalso false)
      handle IllegalMove => true andalso
    (officiate_challenge(
      [(Clubs, Ace), (Clubs, Ace), (Clubs, Ace)],
      [Draw, Draw, Draw], 33) = 0) handle IllegalMove => false
  fun test_careful_player() =
    careful_player([(Clubs, Ace)], 3) = [] andalso
    careful_player([(Clubs, Ace)], 11) = [Draw] andalso
    careful_player([(Clubs, Ace), (Clubs, Ace)], 22) = [Draw, Draw] andalso
    careful_player([(Clubs, Num(3)), (Clubs, Ace)], 11) =
      [Draw, Discard(Clubs, Num(3)), Draw] andalso
    careful_player([(Clubs, Ace), (Clubs, Ace)], 11) = [Draw]
  val tests = [
    test_all_except_option,
    test_get_substitutions(get_substitutions1),
    test_get_substitutions(get_substitutions2),
    test_similar_names,
    test_card_color,
    test_card_value,
    test_remove_card,
    test_all_same_color,
    test_sum_cards,
    test_score,
    test_officiate,
    test_score_challenge,
    test_officiate_challenge,
    test_careful_player
  ]
in
  forEach(tests)
end

