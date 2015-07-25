sig Person, Number {}
sig Book {
  // normal：常用；work：工作；family：家庭
  normal, work, family: Person -> lone Number
}{
  // 所有有工作电话或家庭电话的人一定会有常用电话
  no p: one Person | (some p.work + p.family) and (no p.normal)
  // 一个联系人的工作电话和家庭电话号码不同
  no p: one Person | (some p.work) and (p.work in p.family)
  no p: one Person | (some p.family) and (p.family in p.work)
  // 除了一个家庭的联系人不共享电话号码
  all disj p, q: one Person, n: one Number |
     (n in p.normal + p.work + p.family)
       => not (n in q.normal + q.work)
}

// 找到联系人的常用电话号码
fun findNormalByPerson[b: Book, p: Person]: one Number {
  b.normal[p]
}
// 找到联系人的所有电话号码
fun findAllByPerson[b: Book, p: Person]: set Number {
  b.normal[p] + b.work[p] + b.family[p]
}
// 根据电话号码找到联系人
fun findPersonByNumber[b: Book, n: Number]: set Person {
  n.~(b.normal) + n.~(b.work) + n.~(b.family)
}

// 添加一个联系人
pred addPerson[b, b': one Book, p: one Person, nn: one Number, wn, fn: lone Number] {
  no b.normal[p]
  b'.normal = b.normal + (p -> nn)
  b'.work = b.work + (p -> wn)
  b'.family = b.family + (p -> fn)
}
// 修改联系人的号码
pred editPerson[b, b': one Book, p: one Person, nn: one Number, wn, fn: lone Number] {
  some b_temp: one Book {
    rmPerson[b, b_temp, p]
    addPerson[b_temp, b', p, nn, wn, fn]
  }
}
// 删除一个联系人
pred rmPerson[b, b': one Book, p: one Person] {
  one b.normal[p]
  b'.normal = b.normal - (p -> b.normal[p])
  b'.work = b.work - (p -> b.work[p])
  b'.family = b.family - (p -> b.family[p])
}

// 检查每个人的电话号码个数少于或等于3个
assert numberByPersonLessOrEqual3 {
  all b: one Book, p: one Person | #findAllByPerson[b, p] =< 3
}
// 所有在电话簿中的联络人总有常用电话号码
assert allPersonInBookHaveNormal {
  all b: one Book, p: one Person |
    some findAllByPerson[b, p] => some findNormalByPerson[b, p]
}
// 有两个人有相同的电话号码
pred personsHaveSameNumber {
  some b: one Book, n: one Number | #findPersonByNumber[b, n] > 1
}
// 除了家庭号码不存在两个人拥有相同的号码
assert noSameNumberFor2PersonExceptFamily {
  all b: one Book, n: one Number, disj p, q: one Person |
    (p + q in findPersonByNumber[b, n])
      => ((some b.family[p]) and (b.family[p] = b.family[q]))
}
// 联系人的工作号码和家庭号码不同
assert noSameNumberForWorkAndFamily {
  all b: one Book, p: one Person |
    (b.family[p] = b.work[p]) => no b.family[p]
}
// 有联系人的常用电话同时是工作电话或家庭电话
pred somePersonNormalSameAsWorkOrFamily {
  some b: one Book, p: one Person |
    b.normal[p] in (b.work[p] + b.family[p])
}

check numberByPersonLessOrEqual3 for 3 Person, 9 Number, 1 Book
check allPersonInBookHaveNormal for 3 Person, 9 Number, 1 Book
run personsHaveSameNumber for 3 Person, 9 Number, 1 Book
check noSameNumberFor2PersonExceptFamily for 3 Person, 9 Number, 1 Book
run somePersonNormalSameAsWorkOrFamily for 3 Person, 9 Number, 1 Book

run addPerson for 3 Person, 9 Number, 2 Book
run editPerson for 3 Person, 9 Number, 2 Book
run rmPerson for 3 Person, 9 Number, 2 Book

