#lang racket

(require rackunit)
(require racket)
(require racket/stream)

; Problem 1
(define (sequence x y z)
  (range x (+ y 1) z)
); define

(provide sequence)

(test-case "Problem 1 - 1"
  (check-equal? (sequence 3 11 2) '(3 5 7 9 11)))
(test-case "Problem 1 - 2"
  (check-equal? (sequence 3 8 3) '(3 6)))
(test-case "Problem 1 - 3"
  (check-equal? (sequence 3 2 1) '()))

; Problem 2
(define (string-append-map xs suffix)
  (map (λ (s) (string-append s suffix)) xs)
); define

(provide string-append-map)

(test-case "Problem 2 - 1"
  (check-equal? (string-append-map '() "x") '()))
(test-case "Problem 2 - 2"
  (check-equal?
     (string-append-map '("a" "b" "c") "x")
     '("ax" "bx" "cx")
))

; Problem 3
(define (list-nth-mod xs n)
  (cond
    ((< n 0) (error "list-nth-mod: negative number"))
    ((zero? (length xs)) (error "list-nth-mod: empty list"))
    (#t (list-ref xs (remainder n (length xs))))
  ); cond
); define

(provide list-nth-mod)

(test-case "Problem 3 - 1"
  (check-equal? (list-nth-mod '(0 1 2 3 4 5) 3) 3))
(test-case "Problem 3 - 2"
  (check-equal? (list-nth-mod '(0 1 2 3 4 5) 77) 5))
(test-case "Problem 3 - 3"
  (check-exn exn:fail? (λ () (list-nth-mod '() 42))))
(test-case "Problem 3 - 4"
  (check-exn exn:fail? (λ () (list-nth-mod '(1) -5))))

; Problem 4
;(define (stream-for-n-steps s n)
;  (if (zero? n) null
;    (cons (stream-first s) (stream-for-n-steps (stream-rest s) (- n 1)))
;  )
;)

(define (stream-for-n-steps s n)
  (let ([t (s)])
    (if (zero? n) null
      (cons (car t) (stream-for-n-steps (cdr t) (- n 1)))
    )
  )
)

(provide stream-for-n-steps)

;(test-case "Problem 4 - 1"
;  (check-equal? (stream-for-n-steps (stream 1 2 3 4 5 6) 4) '(1 2 3 4)))
;(test-case "Problem 4 - 2"
;  (check-equal? (stream-for-n-steps (stream 1 2 3 4 5 6) 0) '()))

; Helper function for problems
 
;(define natural-number-stream
;  ((letrec ([f (λ (n) (stream-cons n (f (+ n 1))))]) f) 0)
;)

(define natural-number-stream
  (letrec ([f (λ (x) (cons x (λ () (f (+ x 1)))))])
    (λ () (f 0))))

(define (my-stream-map f s)
  (λ () (let ([c (s)])
    (cons (f (car c)) (my-stream-map f (cdr c)))
  ))
)

(define (my-stream-rest s)
  (cdr (s))
)

; Problem 5
(define funny-number-stream
  (my-stream-map
    (λ (x) (if (zero? (remainder x 5)) (- x) x))
    (my-stream-rest natural-number-stream)
  )
)


(provide funny-number-stream)

(test-case "Problem 5 - 1"
  (check-equal?
     (stream-for-n-steps funny-number-stream 12)
     '(1 2 3 4 -5 6 7 8 9 -10 11 12)))

; Problem 6
(define dan-then-dog
  (my-stream-map
    (λ (x) (if (even? x) "dan.jpg" "dog.jpg"))
    natural-number-stream
  )
)

(test-case "Problem 6 - 1"
  (check-equal?
     (stream-for-n-steps dan-then-dog 5)
     '("dan.jpg" "dog.jpg" "dan.jpg" "dog.jpg" "dan.jpg")))

(provide dan-then-dog)

; Problem 7
(define (stream-add-zero s)
  (my-stream-map (λ (x) (cons 0 x)) s)
)

(test-case "Problem 7 - 1"
  (check-equal?
    (stream-for-n-steps (stream-add-zero dan-then-dog) 3)
    '((0 . "dan.jpg") (0 . "dog.jpg") (0 . "dan.jpg"))))

(provide stream-add-zero)

; Problem 8
(define (cycle-lists xs ys)
  (my-stream-map 
    (λ (i) (cons (list-nth-mod xs i) (list-nth-mod ys i)))
    natural-number-stream
  )
)

(test-case "Problem 8 - 1"
  (check-equal?
    (stream-for-n-steps (cycle-lists '(1 2 3) '("a" "b")) 7)
    '((1 . "a") (2 . "b") (3 . "a") (1 . "b") (2 . "a") (3 . "b") (1 . "a"))))

(provide cycle-lists)

; Problem 9
(define (vector-assoc v vec [is-equal? equal?])
  (assoc v (filter pair? (vector->list vec)) is-equal?)
)

(test-case "Problem 9 - 1"
  (check-equal?
    (vector-assoc 2 (vector '(1 3) '(2 5) '(3 6)))
    '(2 5)))

(test-case "Problem 9 - 1"
  (check-equal?
    (vector-assoc 6 (vector '(1 3) '(2 5) '(3 6)))
    #f))

(provide vector-assoc)


; Problem 10
(define (cached-assoc xs n)
  (let ([cache (make-vector n)]
        [pos 0])
    (λ (x) (cond
      [(vector-assoc x cache)]
      [(let ([v (assoc x xs)])
        (if v (begin
          (vector-set! cache pos v)
          (set! pos (remainder (+ pos 1) n))
        v) #f))]
      [#t #f]
    ))
  )
)

(provide cached-assoc)

; Problem 11
(define-syntax while-less
  (syntax-rules (do)
    [(while-lees e1 do e2)
     (letrec ([r1 e1]
              [f (λ (x) (< r1 x))]
              [s (λ () (stream-cons e2 (s)))])
       (stream-ormap f (s))
     )
    ]
  )
)

(provide while-less)


; Problem 12
(define (cycle-lists-challenge xs ys)
  (letrec ([f (λ (x y)
    (let ([xx (if (empty? x) xs x)] [yy (if (empty? y) ys y)])
      (stream-cons (cons (car xx) (car yy)) (f (cdr xx) (cdr yy)))
    )
  )]) (f xs ys))  
)

; Problem 13
(define (cached-assoc-lru xs n)
  (letrec ([cache (make-vector n)]
           [recent (make-vector n)]
           [min-index (λ () 
             (letrec ([p (apply min (vector->list recent))]) (begin
               (set! t (+ t 1))                                                
               (vector-set! recent p t)
               p))
           )]
           [t 0])
    (λ (x) (cond
      [(vector-assoc x cache)]
      [(let ([v (assoc x xs)])
        (if v (begin
          (vector-set! cache (min-index) v)
        v) #f))]
      [#t #f]
    ))
  )
)
