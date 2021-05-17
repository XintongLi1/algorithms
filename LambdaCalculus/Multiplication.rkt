(require "Lambda.rkt")

(define MUL
  (Y (λ (f)
       (λ (a b)
         (If (Empty? a) Empty
             (If (Empty? b) Empty
                 (If (And (Car a) (Car b)) ; 1 1
                     (If (And (split? a) (split? b))
                         (If (Length a b)(mul a b)(mul b a))
                     (If (Length a b)(simple-mul a b a)
                         (simple-mul b a b)))
                     (If (Car a) (Cons False (f a (Cdr b)))
                         (If (Car b) (Cons False (f (Cdr a) b)) (Cons False (Cons False (f(Cdr a)(Cdr b)))))))))))))

(define (mul a b)
  (ADD (Cons False (Base (Cdr (find-length b Empty)) (MUL (x1 a (Cdr (find-length b Empty))) (x1 b (Cdr (find-length b Empty))))))
              (ADD (Base (Cdr (find-length b Empty)) (ADD (MUL (x1 a (Cdr (find-length b Empty))) (Reverse(x0 b (Cdr (find-length b Empty)) Empty) Empty)) (MUL (Reverse(x0 a (Cdr (find-length b))Empty)Empty)(x1 b (Cdr (find-length b))))))
                   (MUL (Reverse (x0 a (Cdr (find-length b Empty)) Empty) Empty) (Reverse(x0 b (Cdr (find-length b Empty)) Empty) Empty)))))

(define Base
  (Y (λ (f)
       (λ (num n)
         (If (Empty? num) n
             (f (Sub1 num) (Cons False n)))))))


(define (split? n)
 (If (Not (Empty? (Cdr (Cdr (Cdr (Cdr (Cdr (Cdr (Cdr (Cdr (Cdr (Cdr n))))))))))))
     True
     False))

(define Length
  (Y (λ (f)
       (λ (a b)
         (If (Empty? a) False (If (Empty? b) True (f (Cdr a) (Cdr b))))))))

(define find-length
  (Y (λ (f)
       (λ (n count)
         (If (Empty? n)
             count
             (f (Cdr n) (ADD1 count)))))))
(define Sub1
  (Y (λ (f)
       (λ (n)
       (If (Car n) (Cons False (Cdr n))
           (Cons True (f (Cdr n))))))))


(define x1
  (Y (λ (f)
       (λ (n length)
       (If (Empty? length) n
           (f (Cdr n) (Sub1 length)))))))

(define x0
  (Y (λ (f)
       (λ (n length acc)
         (If (Empty? length) acc
             (f (Cdr n) (Sub1 length) (Cons (Car n) acc)))))))

(define Reverse
  (Y (λ (f)
       (λ (n acc)
         (If (Empty? n) acc
             (f (Cdr n) (Cons (Car n) acc)))))))

(define simple-mul
  (Y (λ (f)
       (λ (l s g) ; large-num small-num acc  (acc-ini = large-num)
         (If (Empty? s) Empty
             (ADD (If (Car s) g Empty) (f l (Cdr s) (Cons False g))))))))
