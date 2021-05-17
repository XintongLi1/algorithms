;λn.λf.λx.f (n f x)
;m.λn.m SUCC n

; 1 (True (λ (x) True))

;; Weirdly, I don't even know where is the recursion

;; I applied (λ (g) ..... (g (cdr x)) before, and it turns out to be non-applicable.




;((f-clone f-clone) 10)

;(((λ (g) (λ (n) (if (= n 1) 0 (add1 ((g g) (quotient n 2))))))(λ (g) (λ (n) (if (= n 1) 0 (add1 ((g g) (quotient n 2)))))))10)

;(((λ (x) (x x)) (λ (g) (λ (n) (if (= n 1) 0 (add1 ((g g) (quotient n 2)))))))10)
           ; which computes the same result as
; (((λ (f) ((λ (x) (f (x x)))
          ;(λ (x) (f (x x)))))
  ;(λ (g) (λ (n) (if (= n 1) 0 (add1 (g (quotient n 2)))))))10)


((λ (f) (f f)) (λ (g) (λ (x) (((x (lambda (yes) (lambda(no) (lambda (y) (lambda (n) n))))) ; Empty? x
        ((λ (a b) (λ (s) ((s a)b))) (λ (y) (λ (n) y)) (λ (x) (λ (y) (λ (n) y))))) ; (Cons True Empty)
        (((x (lambda (yes) (lambda (no) yes)))  ;(If (Car x)
         ((λ (a b) (λ (s) ((s a) b)))(λ(y) (λ (n) n)) ((g g) (x (λ (z) (λ (n) n))))))                                                    
         ((λ (car cdr) (lambda (selector) ((selector car) cdr))) (lambda (yes) (lambda (no) yes)) (x (lambda (yes) (lambda (no) no)))))))))

