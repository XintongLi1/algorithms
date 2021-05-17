#lang lazy

(provide
 True
 False
 If
 Or
 And
 Not
 Cons
 Car
 Cdr
 Empty
 Empty?
 Y               ;; fixed point combinator
 Z               ;; Zero as binary Natural
 Z?              ;; Test for Z
 ADD1            ;; Add 1 to binary Natural
 ADD             ;; Adds binary Naturals
 ;;;; impure functions.  only for input and output
 True? ;; convert Boolean to 'yes or 'no
 TAN   ;; convert binary Natural number to Racket number
 NAT   ;; convert Racket number to binary Natural

)

(define True (lambda (yes) (lambda (no) yes)))
(define False (lambda (yes) (lambda (no) no)))
(define (True? b) ((b 'yes) 'no))

(define (If test thenpart elsepart)
  ((test thenpart) elsepart))

(define (Or a b) (If a True b))
(define (And a b) (If a b False))
(define (Not a) (If a False True))

(define (Cons car cdr)
  (lambda (selector) ((selector car) cdr)))

(define (Car pair) (pair True))
(define (Cdr pair) (pair False))

(define Empty (lambda (x) True))
(define (Empty? lst)
  (lst (lambda (yes) (lambda(no) False))))

(define Y
  (lambda(f)
    ((lambda(self)(f(self self)))(lambda(self)(f(self self))))))

(define Z Empty)
(define Z? Empty?)
(define (ADD1 x)
   (If (Z? x)
       (Cons True Z)
       (If (Car x) (Cons False (ADD1 (Cdr x)))
           (Cons True (Cdr x)))))
(define (TAN x)
  (If (Z? x) 0
      (If (Car x) (add1 (* 2 (TAN (Cdr x)))) (* 2 (TAN (Cdr x))))))

(define (ADD a b)
  (If (Z? a) b
   (If (Z? b) a
    (If (Not (Car a)) (Cons (Car b) (ADD (Cdr a)(Cdr b)))
     (If (Not (Car b)) (Cons (Car a) (ADD (Cdr a)(Cdr b)))
       (Cons False (ADD1 (ADD (Cdr a)(Cdr b)))))))))

(define (NAT n)
  (if (zero? n) Z (ADD1 (NAT (sub1 n)))))

(define (x1 n)
  (define (x2 m)
    (if (< m 0) (void)
     (begin
       (printf "~a ~a ~a\n" n m (TAN (ADD (NAT n)(NAT m))))
       (x2 (sub1 m)))))
  (if (< n 0) (void)
      (begin (x2 n) (x1 (sub1 n)))))

;; Addition table to test ADD

;(x1 6)
