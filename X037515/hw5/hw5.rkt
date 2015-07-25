;; Programming Languages, Homework 5

#lang racket
(provide (all-defined-out)) ;; so we can put tests in a second file

;; definition of structures for MUPL programs - Do NOT change
(struct var        (string)              #:transparent)  ;; a variable, e.g., (var "foo")
(struct int        (num)                 #:transparent)  ;; a constant number, e.g., (int 17)
(struct add        (e1 e2)               #:transparent)  ;; add two expressions
(struct ifgreater  (e1 e2 e3 e4)         #:transparent) ;; if e1 > e2 then e3 else e4
(struct fun        (nameopt formal body) #:transparent) ;; a recursive(?) 1-argument function
(struct call       (funexp actual)       #:transparent) ;; function call
(struct mlet       (var e body)          #:transparent) ;; a local binding (let var = e in body) 
(struct apair      (e1 e2)               #:transparent) ;; make a new pair
(struct fst        (e)                   #:transparent) ;; get first part of a pair
(struct snd        (e)                   #:transparent) ;; get second part of a pair
(struct aunit      ()                    #:transparent) ;; unit value -- good for ending a list
(struct isaunit    (e)                   #:transparent) ;; evaluate to 1 if e is unit else 0

;; a closure is not in "source" programs; it is what functions evaluate to
(struct closure (env fun) #:transparent) 

;; Problem 1

(define [racketlist->mupllist l] {
  if [null? l] 
     {aunit}
     {apair (car l) (racketlist->mupllist (cdr l))}
})

(define [mupllist->racketlist l] {
  if [aunit? l]
     null
     {cons (apair-e1 l) (mupllist->racketlist (apair-e2 l))}
})

;; Problem 2

;; lookup a variable in an environment
;; Do NOT change this function
(define (envlookup env str)
  (cond [(null? env) (error "unbound variable during evaluation" str)]
        [(equal? (car (car env)) str) (cdr (car env))]
        [#t (envlookup (cdr env) str)]))

;; Do NOT change the two cases given to you.  
;; DO add more cases for other kinds of MUPL expressions.
;; We will test eval-under-env by calling it directly even though
;; "in real life" it would be a helper function of eval-exp.
(define [eval-under-env e env] {cond
    ; (struct var        (string)              #:transparent)  ;; a variable, e.g., (var "foo")
    [(var? e) (envlookup env (var-string e))]
    ; (struct int        (num)                 #:transparent)  ;; a constant number, e.g., (int 17)
    [(int? e) e]
    ; (struct add        (e1 e2)               #:transparent)  ;; add two expressions
    [(add? e) {
      let ([v1 (eval-under-env (add-e1 e) env)]
           [v2 (eval-under-env (add-e2 e) env)])
      (unless (and (int? v1) (int? v2)) (error "MUPL addition applied to non-number"))
      (int (+ (int-num v1) (int-num v2)))
    }]
    ; (struct ifgreater  (e1 e2 e3 e4)         #:transparent) ;; if e1 > e2 then e3 else e4
    [(ifgreater? e) {
      let ([v1 (eval-under-env (ifgreater-e1 e) env)]
           [v2 (eval-under-env (ifgreater-e2 e) env)])
      (unless (and (int? v1) (int? v2)) (error "MUPL compare on non-number"))
      (eval-under-env ((if (> (int-num v1) (int-num v2)) ifgreater-e3 ifgreater-e4) e) env)
    }]
    ;(struct fun        (nameopt formal body) #:transparent) ;; a recursive(?) 1-argument function
    [(fun? e) (closure env e)]
    ;(struct call       (funexp actual)       #:transparent) ;; function call
    [(call? e) {
      let ([v1 (eval-under-env (call-funexp e) env)]
           [v2 (eval-under-env (call-actual e) env)])
      (unless (closure? v1) (error "MUPL applied to uncallable variable"))
      {
        let* ([fun0 (closure-fun v1)]
              [env0 (closure-env v1)]
              [funenv0 (cons (cons (fun-formal fun0) v2) env0)]
              [funname (fun-nameopt fun0)]
              [funenv (if funname (cons (cons funname v1) funenv0) funenv0)])
        (eval-under-env (fun-body (closure-fun v1)) funenv)
      }
    }]
    ;(struct mlet       (var e body)          #:transparent) ;; a local binding (let var = e in body) 
    [(mlet? e) {
      let ([value (eval-under-env (mlet-e e) env)])
      (eval-under-env (mlet-body e) (cons (cons (mlet-var e) value) env))
    }]
    ;(struct apair      (e1 e2)               #:transparent) ;; make a new pair
    [(apair? e) (apair (eval-under-env (apair-e1 e) env) (eval-under-env (apair-e2 e) env))]
    ;(struct fst        (e)                   #:transparent) ;; get first part of a pair
    [(fst? e) {
      let ([v (eval-under-env (fst-e e) env)])
      (unless (and (apair? v)) (error "MUPL argument to fst not apair"))
      (apair-e1 v)
    }]
    ;(struct snd        (e)                   #:transparent) ;; get second part of a pair
    [(snd? e) {
      let ([v (eval-under-env (snd-e e) env)])
      (unless (and (apair? v)) (error "MUPL argument to snd not apair"))
      (apair-e2 v)
    }]
    ;(struct aunit      ()                    #:transparent) ;; unit value -- good for ending a list
    [(aunit? e) e]
    ;(struct isaunit    (e)                   #:transparent) ;; evaluate to 1 if e is unit else 0
    [(isaunit? e) {
      let ([v (eval-under-env (isaunit-e e) env)])
      (int (if (aunit? v) 1 0))
    }]
    ; else...
    [#t {error (format "bad MUPL expression: ~v" e)}]
})

;; Do NOT change
(define (eval-exp e)
  (eval-under-env e null))
        
;; Problem 3

(define (ifaunit e1 e2 e3) 
  (ifgreater (isaunit e1) (int 0) e2 e3)
)

; 这里按照作业要求的，接收类似下面例子的格式
; (mlet* '(["a" . (int 42)]["b" . (int 58)]) (add (var "a") (var "b")))
(define-namespace-anchor a)
(define ns (namespace-anchor->namespace a))
(define (mlet* lstlst e2) 
  (if (null? lstlst) 
    e2
    {mlet (caar lstlst) (eval (cdar lstlst) ns)
      (mlet* (cdr lstlst) e2)
    }
  )
)

; 返回一个0/1再比较可以避免变量名称的污染
(define (ifeq e1 e2 e3 e4) 
  (ifgreater
    {mlet "c" (apair e1 e2)
      (ifgreater (fst (var "c")) (snd (var "c"))
        (int 0)
        (ifgreater (snd (var "c")) (fst (var "c"))
          (int 0)
          (int 1)
        )
      )
    }
    (int 0)
    e3
    e4
  )
)
;; Problem 4

(define mupl-map {fun "map" "fun"
  {fun "mapfun" "list"
    (ifaunit (var "list")
      (aunit)
      (apair
        (call (var "fun") (fst (var "list")))
        (call (var "mapfun") (snd (var "list")))
      )
    )
  }
})


(define mupl-mapAddN {mlet "map" mupl-map
  {fun "mapaddn" "n"
    (call (var "map")
      (fun "addn" "p" (add (var "p") (var "n")))
    )
  }
})

;; Challenge Problem

(struct fun-challenge (nameopt formal body freevars) #:transparent) ;; a recursive(?) 1-argument function

;; We will test this function directly, so it must do
;; as described in the assignment
(define [compute-free-vars-helper e]
  {let* (
    [free-vars-each (λ l (map (λ (f) (compute-free-vars-helper (f e))) l))]
    [free-vars-each-union (λ l (apply set-union (apply free-vars-each l)))]
  ) {cond
    [(var? e) (set (var-string e))]
    [(add? e) (free-vars-each-union add-e1 add-e2)]
    [(ifgreater? e) (free-vars-each-union ifgreater-e1 ifgreater-e2 ifgreater-e3 ifgreater-e4)]
    [(fun-challenge? e) (fun-challenge-freevars e)]
    [(call? e) (free-vars-each-union call-funexp call-actual)]
    [(mlet? e) (set-remove (free-vars-each-union mlet-body) (mlet-var e))]
    [(apair? e) (free-vars-each-union apair-e1 apair-e2)]
    [(fst? e) (free-vars-each-union fst-e)]
    [(snd? e) (free-vars-each-union snd-e)]
    [(isaunit? e) (free-vars-each-union isaunit-e)]
    [(fun? e) (set-subtract (free-vars-each-union fun-body) (set (fun-nameopt e) (fun-formal e)))]
    [#t (set)]
  }}
)

(define [compute-free-vars e]
      {let* (
        [cons-ori (λ l (apply (car l) (map (λ (f) (compute-free-vars (f e))) (cdr l))))]
      ) {cond
        [(var? e) (cons-ori var var-string)]
        [(int? e) (cons-ori int int-num)]
        [(add? e) (cons-ori add add-e1 add-e2)]
        [(ifgreater? e) (cons-ori ifgreater ifgreater-e1 ifgreater-e2 ifgreater-e3 ifgreater-e4)]
        [(call? e) (cons-ori call call-funexp call-actual)]
        [(mlet? e) (cons-ori mlet mlet-var mlet-e mlet-body)]
        [(apair? e) (cons-ori apair apair-e1 apair-e2)]
        [(fst? e) (cons-ori fst fst-e)]
        [(snd? e) (cons-ori snd snd-e)]
        [(aunit? e) (cons-ori aunit)]
        [(isaunit? e) (cons-ori isaunit isaunit-e)]
        [(closure? e) (cons-ori closure-env closure-fun)]
        [(fun? e) (cons-ori fun-challenge fun-nameopt fun-formal fun-body compute-free-vars-helper)]
        [#t e]
      }}
)

;; Do NOT share code with eval-under-env because that will make grading
;; more difficult, so copy most of your interpreter here and make minor changes
(define [eval-under-env-c e env]
  {let* (
    [eval-under-env eval-under-env-c]
    [fun fun-challenge]
    [fun-formal fun-challenge-formal]
    [fun-nameopt fun-challenge-nameopt]
    [fun-body fun-challenge-body]
  ) {cond
    ; (struct var        (string)              #:transparent)  ;; a variable, e.g., (var "foo")
    [(var? e) (envlookup env (var-string e))]
    ; (struct int        (num)                 #:transparent)  ;; a constant number, e.g., (int 17)
    [(int? e) e]
    ; (struct add        (e1 e2)               #:transparent)  ;; add two expressions
    [(add? e) {
      let ([v1 (eval-under-env (add-e1 e) env)]
           [v2 (eval-under-env (add-e2 e) env)])
      (unless (and (int? v1) (int? v2)) (error "MUPL addition applied to non-number"))
      (int (+ (int-num v1) (int-num v2)))
    }]
    ; (struct ifgreater  (e1 e2 e3 e4)         #:transparent) ;; if e1 > e2 then e3 else e4
    [(ifgreater? e) {
      let ([v1 (eval-under-env (ifgreater-e1 e) env)]
           [v2 (eval-under-env (ifgreater-e2 e) env)])
      (unless (and (int? v1) (int? v2)) (error "MUPL compare on non-number"))
      (eval-under-env ((if (> (int-num v1) (int-num v2)) ifgreater-e3 ifgreater-e4) e) env)
    }]
    ;(struct fun        (nameopt formal body) #:transparent) ;; a recursive(?) 1-argument function
    [(fun-challenge? e) (closure 
      (filter (λ (x) (set-member? (fun-challenge-freevars e) x)) env)
      e
    )]
    ;(struct call       (funexp actual)       #:transparent) ;; function call
    [(call? e) {
      let ([v1 (eval-under-env (call-funexp e) env)]
           [v2 (eval-under-env (call-actual e) env)])
      (unless (closure? v1) (error "MUPL applied to uncallable variable"))
      {
        let* ([fun0 (closure-fun v1)]
              [env0 (closure-env v1)]
              [funenv0 (cons (cons (fun-formal fun0) v2) env0)]
              [funname (fun-nameopt fun0)]
              [funenv (if funname (cons (cons funname v1) funenv0) funenv0)])
        (eval-under-env (fun-body (closure-fun v1)) funenv)
      }
    }]
    ;(struct mlet       (var e body)          #:transparent) ;; a local binding (let var = e in body) 
    [(mlet? e) {
      let ([value (eval-under-env (mlet-e e) env)])
      (eval-under-env (mlet-body e) (cons (cons (mlet-var e) value) env))
    }]
    ;(struct apair      (e1 e2)               #:transparent) ;; make a new pair
    [(apair? e) (apair (eval-under-env (apair-e1 e) env) (eval-under-env (apair-e2 e) env))]
    ;(struct fst        (e)                   #:transparent) ;; get first part of a pair
    [(fst? e) {
      let ([v (eval-under-env (fst-e e) env)])
      (unless (and (apair? v)) (error "MUPL argument to fst not apair"))
      (apair-e1 v)
    }]
    ;(struct snd        (e)                   #:transparent) ;; get second part of a pair
    [(snd? e) {
      let ([v (eval-under-env (snd-e e) env)])
      (unless (and (apair? v)) (error "MUPL argument to snd not apair"))
      (apair-e2 v)
    }]
    ;(struct aunit      ()                    #:transparent) ;; unit value -- good for ending a list
    [(aunit? e) e]
    ;(struct isaunit    (e)                   #:transparent) ;; evaluate to 1 if e is unit else 0
    [(isaunit? e) {
      let ([v (eval-under-env (isaunit-e e) env)])
      (int (if (aunit? v) 1 0))
    }]
    ; else...
    [#t {error (format "bad MUPL expression: ~v" e)}]
  }}
)

;; Do NOT change this
(define (eval-exp-c e)
  (eval-under-env-c (compute-free-vars e) null))

