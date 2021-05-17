#lang lazy

(provide 
  Gen
  accum)

(require "IOStream.rkt")

;; (Gen inp state step) creates a lazy list
;;   inp is the overall input (typically but not
;;                            necessarily a list)
;;   state is the internal state
;;   (step inp state cont) returns a list, either
;;      directly, or by invoking cont
;;      (cont newinp newstate newoutput)
;;          is a "continuation" that appends the
;;          elements of newoutput to the beginning
;;          of the list returned by
;;          (Gen newinp newstate step)

;; Examples.
;; Cumulative sum of a list
;; (define (accum s)
;;  (Gen
;;    s  ;; input list
;;    0  ;; initial accumulator
;;    (lambda(in state cont)
;;      (if (empty? in) empty
;;        (cont (cdr in)
;;              (+ state (car in))
;;              (list (+ state (car in))))))))
;;
;; Example usage:  (outstream (accum '(1 2 3 4)))
;; Output for example: 1 3 6 10

(define (Gen inp state step)
  (define (cont inp state out)
     (append out (Gen inp state step)))
  (step inp state cont))
  
  
  ; Cumulative sum; see comments above
(define (accum s)
  (Gen
   s  ;; input list
   0  ;; cumulative sum
   (lambda(in state cont)
     (if (empty? in) empty
       (cont (cdr in)(+ state (car in))(list (+ state (car in))))))))
       
