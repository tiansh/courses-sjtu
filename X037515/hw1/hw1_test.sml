fun all (l: bool list): bool = let
  fun f (x: bool, y: bool): bool = x andalso y
in
  reduce(f, l)
end

fun any (l: bool list): bool = let
  fun f (x: bool, y: bool): bool = x orelse y
in
  reduce(f, l)
end

fun test_range (): bool = let
  val test = [
  len(range(0, 0, 1)) = 0,
  len(range(0, 1, 1)) = 1,
  len(range(1, 0, 1)) = 0,
  len(range(0, 9, 1)) = 9,
  len(range(0, 0,~1)) = 0,
  len(range(0, 1,~1)) = 0,
  len(range(1, 0,~1)) = 1,
  len(range(9, 0,~1)) = 9,
  true]
in 
  all(test)
end

fun test_convert (): bool = let
  fun f (date: int): bool = date = date_to_julian(julian_to_date(date))
  val dates = range(date_to_julian(1601, 1, 1), date_to_julian(2001, 1, 1), 1);
in
  len(dates) = 146097 andalso all(map(f, dates))
end

fun test_list_funcs (): bool = let
  fun double (x: int): int = 2 * x
  fun positive (x: int): bool = x > 0
  val l = [~1, 0, 1, 10];
  val test = [
    map(double, l) = [~2, 0, 2, 20],
    map(positive, l) = [false, false, true, true],
    map(double, []) = [],
    filter(positive, l) = [1, 10],
    filter(positive, [~1, 0]) = [],
    filter(positive, []) = [],
    at(l, 1) = ~1,
    at(l, 4) = 10,
  true]
in
  all(test)
end

fun test_int_funcs (): bool = let
  val test = [
    int_eq 3 3 = true,
    int_eq 3 4 = false,
    min(1, 2) = 1,
    min(1, ~2) = ~2,
    max(1, 2) = 2,
    max(1, ~2) = 1,
    unique_int([]) = [],
    unique_int([1, 2, 3, 4]) = [1, 2, 3, 4],
    unique_int([1, 1, 1, 1]) = [1],
    unique_int([1, ~1, ~1, 1]) = [1, ~1],
    len([]) = 0,
    len([1]) = 1,
    len([1, 2, 3, 4]) = 4,
  true]
in
  all(test)
end

fun test_is_older (): bool = let
  val test = [
    is_older((1990, 11, 1), (1990, 11, 1)) = false,
    is_older((1990, 11, 1), (2014, 3, 8)) = true,
    is_older((2014, 3, 8), (1990, 11, 1)) = false,
    is_older((1990, 11, 1), (1990, 11, 1)) = false,
    is_older((1990, 11, 1), (1990, 11, 1)) = false,
  true]
in
  all(test)
end

fun test_dates_in_month (): bool = let
  val date1 = (1990, 11, 1);
  val date2 = (1990, 10, 1);
  val date3 = (1990, 11, 11);
  val dates = [date1, date2, date3]
  val test = [
    dates_in_month([], 11) = [],
    dates_in_month(dates, 11) = [date1, date3],
    dates_in_month(dates, 1) = [],
    dates_in_month(date1 :: dates, 11) = [date1, date1, date3],
  true]
in
  all(test)
end

fun test_dates_in_months (): bool = let
  val date1 = (1990, 11, 1);
  val date2 = (1990, 10, 1);
  val date3 = (1990, 11, 11);
  val dates = [date1, date2, date3]
  val test = [
    dates_in_months([], []) = [],
    dates_in_months([], [11, 10]) = [],
    dates_in_months(dates, []) = [],
    dates_in_months(dates, [11]) = [date1, date3],
    dates_in_months(dates, [1, 11]) = [date1, date3],
    dates_in_months(dates, [10, 11]) = [date1, date2, date3],
    dates_in_months(dates, [11, 11]) = [date1, date3],
    dates_in_months(date1 :: dates, [11]) = [date1, date1, date3],
  true]
in
  all(test)
end

fun test_date_to_string (): bool = let
  val test = [
    date_to_string(2013, 1, 20) = "January 20, 2013",
    date_to_string(2014, 1, 1) = "January 1, 2014",
    date_to_string(2014, 2, 11) = "February 11, 2014",
    date_to_string(2014, 3, 21) = "March 21, 2014",
    date_to_string(2014, 4, 1) = "April 1, 2014",
    date_to_string(2014, 5, 11) = "May 11, 2014",
    date_to_string(2014, 6, 21) = "June 21, 2014",
    date_to_string(2014, 7, 31) = "July 31, 2014",
    date_to_string(2014, 8, 1) = "August 1, 2014",
    date_to_string(2014, 9, 11) = "September 11, 2014",
    date_to_string(2014, 10, 21) = "October 21, 2014",
    date_to_string(2014, 11, 1) = "November 1, 2014",
    date_to_string(2014, 12, 11) = "December 11, 2014",
  true]
in
  all(test)
end

fun test_number_before_reaching_sum (): bool = let
  val test = [
    number_before_reaching_sum(10, [100]) = 0,
    number_before_reaching_sum(10, [1, 2, 3, 4, 5]) = 3,
    number_before_reaching_sum(11, [1, 2, 3, 4, 5]) = 4,
    number_before_reaching_sum(9, [1, 2, 3, 4, 5]) = 3,
  true]
in
  all(test)
end

fun test_what_month (): bool = let
  val test = [
    what_month(1) = 1,
    what_month(31) = 1,
    what_month(31 + 1) = 2,
    what_month(31 + 28) = 2,
    what_month(31 + 28 + 1) = 3,
    what_month(31 + 28 + 31) = 3,
    what_month(31 + 28 + 31 + 1) = 4,
    what_month(31 + 28 + 31 + 30) = 4,
    what_month(31 + 28 + 31 + 30 + 1) = 5,
    what_month(31 + 28 + 31 + 30 + 31) = 5,
    what_month(31 + 28 + 31 + 30 + 31 + 1) = 6,
    what_month(31 + 28 + 31 + 30 + 31 + 30) = 6,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 1) = 7,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31) = 7,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31 + 1) = 8,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31) = 8,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 1) = 9,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30) = 9,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 1) = 10,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) = 10,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 1) = 11,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) = 11,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 1) = 12,
    what_month(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31) = 12,
  true]
in
  all(test)
end

fun test_month_range (): bool = let
  val test = [
    month_range(31, 31) = [1],
    month_range(31, 30) = [],
    month_range(31, 32) = [1, 2],
    month_range(30, 31) = [1, 1],
    month_range(30, 32) = [1, 1, 2],
    month_range(32, 33) = [2, 2],
  true]
in
  all(test)
end

fun test_oldest (): bool = let
  val date1 = (1990, 11, 1);
  val date2 = (1990, 10, 1);
  val date3 = (1990, 11, 11);
  val test = [
    oldest([]) = NONE,
    oldest([date1]) = SOME date1,
    oldest([date1, date2]) = SOME date2,
    oldest([date1, date2, date3]) = SOME date2,
    oldest([date1, date3]) = SOME date1,
  true]
in
  all(test)
end

fun tests (funcs: (unit -> bool) list): (unit -> bool) = let
  fun call (f: unit -> 'a): 'a = f()
  fun f (): bool = all(map(call, funcs))
in f end

fun const (x: 'a): (unit -> 'a) = let
  fun f(): 'a = x
in f end

val result = tests ([
  test_range,
  test_convert,
  test_list_funcs,
  test_int_funcs,
  test_is_older,
  test_dates_in_month,
  test_dates_in_months,
  test_date_to_string,
  test_number_before_reaching_sum,
  test_what_month,
  test_month_range,
const(true)])();
