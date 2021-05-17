#lang lazy
;;Leaderboard (*^▽^*)
(require "IOStream.rkt")
(require "Gen.rkt")

;(outstream instream) ; goes forever,  never halt
; 4[(n-1)!+1] = -n (mod n(n+2))

(define Nats
  (Gen
   0
   0
   (λ (in state cont)
     (cont (add1 in) state (list in)))))

(define (Kill k s)
  (filter (λ (x) (if (zero? (remainder x k)) false true)) s))

(define wheel-sieve
 (Gen
  5
  siever
  (λ (in state cont)
    (define small (Kill 5 (take 9 siever)))
    (if (= 5 in) (cont 7 small  (list 2 3 5)) ; (cddddr (cddddr (cddr state))) 
        (cont 7 (map (λ (x) (+ 30 x)) state) state)))))  

(define siever
  (Gen
   0
   1
   (λ (in state cont)
     (cont 0 (add1 state) (list (sub1 (* 6  state)) (add1 (* 6  state)))))))

 (define c-siever (cdr siever))

(define primes
  (Gen
   (list 5)
   c-siever
   (λ (in state cont)
     (define acc (Kill (car in) (second take-result))); (delete (Kill (car in) (cdr state)) ele-taken))
     (define take-result (take-until state empty (car in)))
     (define ele-taken (first take-result))
     (if (= 5 (car in)) (cont (append (cdr in) ele-taken) acc (append (list 2 3 5) ele-taken))
         (cont (append (cdr in) ele-taken) acc ele-taken)))))

(define(delete l1 l2); l2 ∈ l1
  (cond
    [(empty? l2) l1]
    [else (delete (cdr l1) (cdr l2))]))

(define (take-until lst acc num)
  (cond
    [(>= (car lst) (expt num 2)) (list (reverse acc) lst)]
    [else (take-until (cdr lst) (cons (car lst) acc) num)]))
    

(define (prime-helper lst)
  (if (test (car lst)) (cons (car lst) (prime-helper (cdr lst))) (prime-helper (cdr lst))))

(define (test n) ;; miller-rabin
  (define a (add1(random (- n 2))))
  (define b (add1(random (- n 2))))
  (if (= 1 a) (if (= 1 (mod b (sub1 n) n)) (test2 (mod b (* 2 (sub1 n)) n) n) false) (if (= 1 (mod a (sub1 n) n)) (test2 (mod a (* 2 (sub1 n)) n) n) false)))

(define (test2 num n)
  (if (or(= 1 num)(= (sub1 n) num)) true false))

(define (simplify lst)
  (Gen
   0
   lst
   (λ (in state cont)
     (if (zero? in) (cont 1 state (list (sub1 (* 6 (car state))))) (cont 0 (cdr state) (list (add1 (* 6 (car state)))))))))
    
(define (mod d e m)
  (cond
    [(zero? e) 1]
    [(zero? d) 0]
    [(= d 1) 1]
    [(= e 1) (remainder d m)]
    [(= d (sub1 m)) (if (even? e) 1 (sub1 m))]
    [(>= d m) (mod (remainder d m) e m)]
    [else (mod-helper d e m (cons d empty))])) ; d < m
    
(define (mod-helper d e m record)
  (cond
    ;; assert: d < m 
    [(= 1 (length record)) (mod-helper d e m (cons (remainder (* d (car record)) m) record))]
    [(= e (length record)) (car record)]
    [(eq? false (find (remainder (* d (car record)) m) record))
     (mod-helper d e m (cons (remainder (* d (car record)) m) record))]
    [else (calc d e m (add1 (length record)) (remainder (* d (car record)) m))]))

(define (calc d e m index num)
  (remainder (* (mod d (remainder e index) m) (mod num (quotient e index) m)) m))
   
(define twinprimes
  (filter (λ (x) (if (= 2 (- (second x) (first x))) true false)) (comb primes)))

(define (comb s)
  (map list s (cdr s)))

(define (find n lst) ; ini_acc = (length lst)
  (cond
    [(empty? lst) false]
    [(= n (car lst)) true]
    [else (find n (cdr lst))]))
    
           

