#lang racket

(require "hw5.rkt")

; a test case that uses problems 1, 2, and 4
; should produce (list (int 10) (int 11) (int 16))
(define test1
  (mupllist->racketlist
   (eval-exp (call (call mupl-mapAddN (int 7))
                   (racketlist->mupllist 
                    (list (int 3) (int 4) (int 9)))))))

(mupllist->racketlist (aunit))
(racketlist->mupllist (list))
(mupllist->racketlist (apair (int 10) (apair (int 20) (apair (int 30) (apair (int 40) (aunit))))))
(racketlist->mupllist (list (int 10) (int 20) (int 30) (int 40)))

(eval-exp (int 100))
(eval-exp (add (int 3) (int 97)))
(eval-exp (ifgreater (int 1) (int 0) (int 100) (int -1)))
(eval-exp (ifgreater (int 1) (int 1) (int -1) (int 100)))
(eval-exp (ifgreater (int 1) (int 2) (int -1) (int 100)))
(eval-exp (call (fun "f" "a" (var "a")) (int 100)))
(eval-exp (mlet "x" (int 100) (var "x")))
(eval-exp (mlet "x" (fun "f" "a" (var "a")) (call (var "x") (int 100))))
(eval-exp (call (fun "f" "a" (add (fst (var "a")) (add (fst (var "a")) (snd (var "a"))))) (apair (int 3) (int 94))))
(define MUPL-sum-list (fun "sum" "l" (ifgreater (isaunit (var "l")) (int 0) (int 0) (add (fst (var "l")) (call (var "sum") (snd (var "l")))))))
(eval-exp (call MUPL-sum-list (apair (int 10) (apair (int 20) (apair (int 30) (apair (int 40) (aunit)))))))

(eval-exp (ifaunit (aunit) (int 100) (int -1)))
(eval-exp (ifaunit (apair (int 1) (aunit)) (int -1) (int 100)))
(mlet* '(["a" . (int 42)]["b" . (int 58)]) (add (var "a") (var "b")))
(eval-exp (ifeq (int 3) (int 3) (int 100) (int -1)))
(eval-exp (ifeq (int 3) (int 2) (int -1) (int 100)))

; 一个大于一的正整数，如果是奇数则乘3加1，如果是偶数则除以2，计算迭代到1所需要的步数
; http://zh.wikipedia.org/wiki/%E8%80%83%E6%8B%89%E5%85%B9%E7%8C%9C%E6%83%B3
(define MUPL-odd? (fun "odd?" "i" (ifgreater (var "i") (int 0) (call (var "odd?") (add (var "i") (int -2))) (ifgreater (var "i") (int -1) (int 0) (ifgreater (var "i") (int -2) (int 1) (call (var "odd?") (add (var "i") (int 2))))))))
(eval-exp (call MUPL-odd? (int 5)))
(eval-exp (call MUPL-odd? (int 8)))
(eval-exp (call MUPL-odd? (int -3)))
(eval-exp (call MUPL-odd? (int -6)))
(eval-exp (call MUPL-odd? (int -1)))
(eval-exp (call MUPL-odd? (int 0)))
(eval-exp (call MUPL-odd? (int 1)))
(define MUPL-m3p1 (fun #f "i" (add (int 1) (add (var "i") (add (var "i") (var "i"))))))
(eval-exp (call MUPL-m3p1 (int 1)))
(eval-exp (call MUPL-m3p1 (int 2)))
(eval-exp (call MUPL-m3p1 (int 3)))
(define MUPL-div2 (fun "div2" "i" (ifgreater (var "i") (int 1) (add (int 1) (call (var "div2") (add (var "i") (int -2)))) (ifgreater (int -1) (var "i") (add (int -1) (call (var "div2") (add (var "i") (int 2)))) (int 0)))))
(eval-exp (call MUPL-div2 (int -103)))
(eval-exp (call MUPL-div2 (int -3)))
(eval-exp (call MUPL-div2 (int -2)))
(eval-exp (call MUPL-div2 (int 0)))
(eval-exp (call MUPL-div2 (int 1)))
(eval-exp (call MUPL-div2 (int 2)))
(eval-exp (call MUPL-div2 (int 3)))
(eval-exp (call MUPL-div2 (int 103)))
(define MUPL-collatz (fun "collatz" "i" (ifeq (var "i") (int 1) (int 0) (add (int 1) (call (var "collatz") (ifgreater (call MUPL-odd? (var "i")) (int 0) (call MUPL-m3p1 (var "i")) (call MUPL-div2 (var "i"))))))))
(eval-exp (call MUPL-collatz (int 6))) ; 8
(eval-exp (call MUPL-collatz (int 11))) ; 14
(eval-exp (call MUPL-collatz (int 27))) ; 111

(compute-free-vars (fun "m" "a" (fun "f" "x" (fun "g" "y" (add (var "x") (var "y"))))))
(compute-free-vars (fun "m" "a" (fun "f" "x" (fun "g" "x" (add (var "x") (var "x"))))))
(compute-free-vars (fun "m" "a" (mlet "x" (int 1) (fun "g" "y" (add (var "x") (var "y"))))))
(compute-free-vars (fun "m" "a" (mlet "x" (int 1) (fun "g" "x" (add (var "x") (var "x"))))))
(compute-free-vars (fun "m" "a" (fun "f" "x" (mlet "y" (int 1) (add (var "x") (var "y"))))))
(compute-free-vars (fun "m" "a" (fun "f" "x" (mlet "x" (int 1) (add (var "x") (var "x"))))))

(eval-exp-c (int 100))
(eval-exp-c (add (int 3) (int 97)))
(eval-exp-c (ifgreater (int 1) (int 0) (int 100) (int -1)))
(eval-exp-c (ifgreater (int 1) (int 1) (int -1) (int 100)))
(eval-exp-c (ifgreater (int 1) (int 2) (int -1) (int 100)))
(eval-exp-c (call (fun "f" "a" (var "a")) (int 100)))
(eval-exp-c (mlet "x" (int 100) (var "x")))
(eval-exp-c (mlet "x" (fun "f" "a" (var "a")) (call (var "x") (int 100))))
(eval-exp-c (call (fun "f" "a" (add (fst (var "a")) (add (fst (var "a")) (snd (var "a"))))) (apair (int 3) (int 94))))
(eval-exp-c (call MUPL-sum-list (apair (int 10) (apair (int 20) (apair (int 30) (apair (int 40) (aunit)))))))
(eval-exp-c (call MUPL-collatz (int 6))) ; 8
(eval-exp-c (call MUPL-collatz (int 11))) ; 14
(eval-exp-c (call MUPL-collatz (int 27))) ; 111

