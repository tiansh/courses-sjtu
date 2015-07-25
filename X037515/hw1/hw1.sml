(* 日期相关 *)
(* 将格里历日期转换为儒略日 *)
fun date_to_julian (year: int, month: int, date: int): int = let
  val a = if month > 2 then year else year - 1
  val b = if month > 2 then month - 3 else month + 9
  val c = a div 100
  val d = a - 100 * c
in
  (146097 * c) div 4 + (1461 * d) div 4 + (153 * b + 2) div 5 + date + 1721119
end

(* 将儒略日转换为格里历日期 *)
fun julian_to_date (jd: int): (int * int * int) = let
  val a = jd - 1721119
  val b = (4 * a - 1) div 146097
  val c = 4 * a - 1 - 146097 * b
  val d = c div 4
  val e = (4 * d + 3) div 1461
  val f = 4 * d + 3 - 1461 * e
  val g = (f + 4) div 4
  val h = (5 * g - 3) div 153
  val i = 5 * g - 3 - 153 * h
  val j = 100 * b + e
  val year = if h < 10 then j else j + 1
  val month = if h < 10 then h + 3 else h - 9
  val day = (i + 5) div 5
in
  (year, month, day)
end

(* 检查一个日期是否是合法的 *)
fun valid_date (date: int * int * int): bool =
  (#1 date) > 0 andalso date = julian_to_date(date_to_julian(date))

(* 返回给定日期的年，如果日期非法返回0 *)
fun get_year (date: int * int * int): int =
  if valid_date(date) then (#1 date)
  else 0

(* 返回给定日期的月，如果日期非法返回0 *)
fun get_month (date: int * int * int): int =
  if valid_date(date) then (#2 date)
  else 0

(* 返回给定日期的日，如果日期非法返回0 *)
fun get_day (date: int * int * int): int =
  if valid_date(date) then (#3 date)
  else 0

val month_names = ["January", "February", "March", "April",
  "May", "June", "July", "August",
  "September", "October", "November", "December"]

(* 链表相关 *)
(* map *)
fun map (f: 'a -> 'b, l: 'a list): 'b list =
  if null l then nil
  else f(hd l) :: map(f, tl l)

(* reduce *)
fun reduce (f: 'a * 'a -> 'a, l: 'a list): 'a =
  if null (tl l) then hd l
  else reduce(f, f(hd l, hd (tl l)) :: tl (tl l))

(* 有初值的reduce *)
fun reduce0 (f: 'b * 'a -> 'b, v: 'b, l: 'a list): 'b =
  if null l then v
  else reduce0(f, f(v, hd l), tl l)

(* 按条件过滤一个链表 *)
fun filter (f: 'a -> bool, l: 'a list): 'a list = let
  fun concat_c (l: 'a list, e: 'a): 'a list =
    if f(e) then l @ [e]
    else l
in
  reduce0(concat_c, nil, l)
end

(* 数一个链表中元素的数量 *)
fun len (l: 'a list): int = let
  fun count (n: int, e: 'a): int = n + 1
in
  reduce0(count, 0, l)
end

(* 链表找指定位置元素 *)
fun at (l: 'a list, n: int): 'a = 
  if n = 1 then hd l
  else at(tl l, n - 1)

(* 其他基本函数 *)
(* 整数判断相等 *)
fun int_eq (x: int): (int -> bool) = let
  fun eq (y: int): bool = x = y
in
  eq
end

(* 返回一个范围内的整数 *)
fun range (from: int, to: int, step: int): int list =
  if (from - to) * step >= 0 then nil
  else from :: range(from + step, to, step)

(* 整数大小比较 *)
fun min (x: int, y: int): int = if x > y then y else x
fun max (x: int, y: int): int = if x < y then y else x

(* 去除整数链表中的重复值 *)
fun unique_int (l: int list): int list = let
  fun append (l: int list, e: int): int list =
    if null (filter(int_eq(e), l)) then l @ [e]
    else l
in
  reduce0(append, nil, l)
end

(* 题目涉及的函数 *)
(* 1. 比较日期的大小 *)
fun is_older (date1: (int * int * int), date2: (int * int * int)): bool =
  date_to_julian(date1) < date_to_julian(date2)

(* 4. 返回某个列表中在指定月份内的日期 *)
fun dates_in_month (dates: (int * int * int) list, month: int):
(int * int * int) list = let
  fun month_eq(date: int * int * int): bool = get_month(date) = month
in
  filter(month_eq, dates)
end

(* 5. 返回某个列表中在指定月份列表内的日期 *)
fun dates_in_months (dates: (int * int * int) list, months: int list):
(int * int * int) list = let
  fun month_in(date: int * int * int): bool =
    not (null (filter(int_eq(get_month(date)), months)))
in
  filter(month_in, dates)
end

(* 2. 检查在某个列表中有多少日期在指定的月份内 *)
fun number_in_month (dates: (int * int * int) list, month: int): int =
  len(dates_in_month(dates, month))

(* 3. 检查在某个列表中有多少日期在指定的月份链表内 *)
fun number_in_months (dates: (int * int * int) list, month: int list): int =
  len(dates_in_months(dates, month))

(* 6. 获取一个字符串链表中第n个字符串 *)
fun get_nth (l: string list, n: int): string = at(l, n)

(* 7. 日期转换为字符串 *)
fun date_to_string (year: int, month: int, day: int): string = let
  val valid = valid_date(year, month, day)
  val year_str = Int.toString(year)
  val month_str = at(month_names, month)
  val day_str = Int.toString(day)
in
  if not valid then "N/A"
  else month_str ^ " " ^ day_str ^ ", " ^ year_str
end

(* 8. 列表最前若干个相加超过定值的数字 *)
fun number_before_reaching_sum (sum: int, data: int list): int =
  if sum > hd data then 1 + number_before_reaching_sum(sum - hd data, tl data)
  else 0

(* 9. 输入平年一年中第几天，输出这一天是哪个月份 *)
fun what_month (days: int): int =
  get_month(julian_to_date(date_to_julian(2013, 12, 31) + days))

(* 10. 给定两个日期，返回他们之间每个日期的月份 *)
fun month_range (day1: int, day2: int): int list =
  map(what_month, range(day1, day2 + 1, 1))

(* 11. 返回日期列表中最小的日期 *)
fun oldest (dates: (int * int * int) list): (int * int * int) option =
  if null dates then NONE
  else SOME (julian_to_date(reduce(min, map(date_to_julian, dates))))

(* 12. 去除重复月份 *)
fun dates_in_months_challenge (dates: (int * int * int) list, months: int list):
(int * int * int) list = 
  dates_in_months(dates, unique_int(months)) 

fun number_in_months_chanllenge (dates: (int * int * int) list, month: int list): int =
  len(dates_in_months_challenge(dates, month))

(* 13. 判断一个日期是否合法 *)
val reasonable_date = valid_date
