(define True (λ (x) (λ (y) x)))
(define False (λ (x) (λ (y) y)))
((λ (a b) ((a b) (λ (x) (λ (y) y)))) False True)  ;and
((λ (a b) ((a b) a)) True False)  ; and

((λ (a b) ((a a) b)) False False)  ; or

((λ (a b) ((a ((b (λ (x) (λ (y) y))) a)) b))False True) 
