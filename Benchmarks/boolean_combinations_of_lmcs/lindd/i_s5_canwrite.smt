(benchmark i_s5_canwrite
:extrafuns ((x0 Int))
:extrafuns ((x24 Int))
:extrafuns ((x25 Int))
:extrafuns ((x26 Int))
:extrafuns ((x27 Int))
:extrafuns ((x28 Int))
:extrafuns ((x29 Int))
:formula
  (exists (x1 Int)
 (x10 Int)
 (x11 Int)
 (x12 Int)
 (x13 Int)
 (x14 Int)
 (x15 Int)
 (x16 Int)
 (x17 Int)
 (x18 Int)
 (x19 Int)
 (x2 Int)
 (x20 Int)
 (x21 Int)
 (x22 Int)
 (x23 Int)
 (x3 Int)
 (x4 Int)
 (x5 Int)
 (x6 Int)
 (x7 Int)
 (x8 Int)
 (x9 Int)

  (and
    (and (<= (+ (* 1 x14) (* (~1) x0)) 0) (and (<= (+ (* 1 x18) (* (~1) x17)) 0) (not (<= (+ (* 1 x18) (* (~1) x17)) (~1)))))
    (and (or (and (<= (+ (* 1 x20) (* (~1) x0)) 0) (not (<= (+ (* 1 x22) (* (~1) x0)) 0))) (not (<= (+ (* 1 x20) (* (~1) x0)) 0))) (and (<= (+ (* 1 x26) (* (~1) x0)) 3) (not (<= (+ (* 1 x26) (* (~1) x0)) 2))))
    (or (and (<= (+ (* 1 x10) (* (~1) x0)) 0) (and (<= (+ (* 1 x12) (* (~1) x11)) 0) (not (<= (+ (* 1 x12) (* (~1) x11)) (~1))))) (and (not (<= (+ (* 1 x10) (* (~1) x0)) 0)) (and (<= (+ (* 1 x12) (* (~1) x13)) 0) (not (<= (+ (* 1 x12) (* (~1) x13)) (~1))))))
    (or (and (<= (+ (* 1 x4) (* (~1) x0)) 0) (and (<= (+ (* 1 x5) (* (~1) x3)) 0) (not (<= (+ (* 1 x5) (* (~1) x3)) (~1))))) (and (not (<= (+ (* 1 x4) (* (~1) x0)) 0)) (and (<= (+ (* 1 x5) (* (~1) x0)) 0) (not (<= (+ (* 1 x5) (* (~1) x0)) (~ 1))))))
    (or (and (<= (+ (* 1 x7) (* (~1) x0)) 0) (and (<= (+ (* 1 x8) (* (~1) x6)) 0) (not (<= (+ (* 1 x8) (* (~1) x6)) (~1))))) (and (not (<= (+ (* 1 x7) (* (~1) x0)) 0)) (and (<= (+ (* 1 x8) (* (~1) x0)) 0) (not (<= (+ (* 1 x8) (* (~1) x0)) (~ 1))))))
    (or (and (<= (+ (* 1 x28) (* (~1) x0)) 0) (and (<= (+ (* 1 x29) (* (~1) x27)) 0) (not (<= (+ (* 1 x29) (* (~1) x27)) (~1))))) (and (not (<= (+ (* 1 x28) (* (~1) x0)) 0)) (and (<= (+ (* 1 x29) (* (~1) x0)) 0) (not (<= (+ (* 1 x29) (* (~1) x0)) (~ 1))))))
    (or (and (and (<= (+ (* 1 x1) (* (~1) x0)) 0) (not (<= (+ (* 1 x1) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x2) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x1) (* (~1) x0)) 0) (not (<= (+ (* 1 x1) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x2) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x12) (* (~1) x0)) 0) (not (<= (+ (* 1 x12) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x14) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x12) (* (~1) x0)) 0) (not (<= (+ (* 1 x12) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x14) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x15) (* (~1) x0)) 0) (not (<= (+ (* 1 x15) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x16) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x15) (* (~1) x0)) 0) (not (<= (+ (* 1 x15) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x16) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x19) (* (~1) x0)) 0) (not (<= (+ (* 1 x19) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x20) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x19) (* (~1) x0)) 0) (not (<= (+ (* 1 x19) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x20) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x20) (* (~1) x0)) 0) (not (<= (+ (* 1 x22) (* (~1) x0)) 0))) (and (<= (+ (* 1 x25) (* (~1) x23)) 0) (not (<= (+ (* 1 x25) (* (~1) x23)) (~1))))) (and (not (<= (+ (* 1 x20) (* (~1) x0)) 0)) (and (<= (+ (* 1 x25) (* (~1) x24)) 0) (not (<= (+ (* 1 x25) (* (~1) x24)) (~1))))))
    (or (and (and (<= (+ (* 1 x21) (* (~1) x0)) 0) (not (<= (+ (* 1 x21) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x22) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x21) (* (~1) x0)) 0) (not (<= (+ (* 1 x21) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x22) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x3) (* (~1) x0)) 0) (not (<= (+ (* 1 x3) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x4) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x3) (* (~1) x0)) 0) (not (<= (+ (* 1 x3) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x4) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x6) (* (~1) x0)) 0) (not (<= (+ (* 1 x6) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x7) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x6) (* (~1) x0)) 0) (not (<= (+ (* 1 x6) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x7) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x9) (* (~1) x0)) 0) (not (<= (+ (* 1 x9) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x10) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x9) (* (~1) x0)) 0) (not (<= (+ (* 1 x9) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x10) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x27) (* (~1) x0)) 0) (not (<= (+ (* 1 x27) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x28) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x27) (* (~1) x0)) 0) (not (<= (+ (* 1 x27) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x28) (* (~1) x0)) 0)))
  ))
)
