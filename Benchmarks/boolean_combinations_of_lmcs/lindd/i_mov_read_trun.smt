(benchmark i_mov_read_trun
:extrafuns ((x0 Int))
:extrafuns ((x62 Int))
:extrafuns ((x63 Int))
:extrafuns ((x64 Int))
:extrafuns ((x65 Int))
:extrafuns ((x66 Int))
:extrafuns ((x67 Int))
:extrafuns ((x68 Int))
:extrafuns ((x69 Int))
:extrafuns ((x70 Int))
:extrafuns ((x71 Int))
:extrafuns ((x72 Int))
:extrafuns ((x73 Int))
:extrafuns ((x74 Int))
:extrafuns ((x75 Int))
:extrafuns ((x76 Int))
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
 (x24 Int)
 (x25 Int)
 (x26 Int)
 (x27 Int)
 (x28 Int)
 (x29 Int)
 (x3 Int)
 (x30 Int)
 (x31 Int)
 (x32 Int)
 (x33 Int)
 (x34 Int)
 (x35 Int)
 (x36 Int)
 (x37 Int)
 (x38 Int)
 (x39 Int)
 (x4 Int)
 (x40 Int)
 (x41 Int)
 (x42 Int)
 (x43 Int)
 (x44 Int)
 (x45 Int)
 (x46 Int)
 (x47 Int)
 (x48 Int)
 (x49 Int)
 (x5 Int)
 (x50 Int)
 (x51 Int)
 (x52 Int)
 (x53 Int)
 (x54 Int)
 (x55 Int)
 (x56 Int)
 (x57 Int)
 (x58 Int)
 (x59 Int)
 (x6 Int)
 (x60 Int)
 (x61 Int)
 (x7 Int)
 (x8 Int)
 (x9 Int)

  (and
    (and (<= (+ (* 1 x41) (* (~1) x0)) 0) (not (<= (+ (* 1 x41) (* (~1) x0)) (~ 1))))
    (and (<= (+ (* 1 x53) (* (~1) x52) ) 1 ) (not (<= (+ (* 1 x53) (* (~1) x52) ) 0) ))
    (and (<= (+ (* 1 x6) (* (~1) x2) ) (~ 1) ) (not (<= (+ (* 1 x6) (* (~1) x2) ) (~ 2)) ))
    (and (<= (+ (* 1 x8) (* (~1) x7) ) 1 ) (not (<= (+ (* 1 x8) (* (~1) x7) ) 0) ))
    (and (<= (+ (* 1 x64) (* (~1) x63) ) 1 ) (not (<= (+ (* 1 x64) (* (~1) x63) ) 0) ))
    (and (<= (+ (* 1 x69) (* (~1) x31)) 0) (not (<= (+ (* 1 x69) (* (~1) x31)) (~1))))
    (and (<= (+ (* 1 x70) (* (~1) x41) ) 1 ) (not (<= (+ (* 1 x70) (* (~1) x41) ) 0) ))
    (and (<= (+ (* 1 x71) (* (~1) x0)) 0) (not (<= (+ (* 1 x71) (* (~1) x0)) (~ 1))))
    (and (<= (+ (* 1 x72) (* (~1) x32)) 0) (not (<= (+ (* 1 x72) (* (~1) x32)) (~1))))
    (and (<= (+ (* 1 x73) (* (~1) x14)) 0) (not (<= (+ (* 1 x73) (* (~1) x14)) (~1))))
    (not (<= (+ (* 1 x23) (* (~1) x22)) 0))
    (not (<= (+ (* 1 x25) (* (~1) x21)) 0))
    (not (<= (+ (* 1 x68) (* (~1) x67)) 0))
    (not (<= (+ (* 1 x74) (* (~1) x73)) 0))
    (or (and (<= (+ (* 1 x12) (* (~1) x0)) 0) (and (<= (+ (* 1 x14) (* (~1) x13)) 0) (not (<= (+ (* 1 x14) (* (~1) x13)) (~1))))) (and (not (<= (+ (* 1 x12) (* (~1) x0)) 0)) (and (<= (+ (* 1 x14) (* (~1) x0)) 0) (not (<= (+ (* 1 x14) (* (~1) x0)) (~ 1))))))
    (or (and (<= (+ (* 1 x16) (* (~1) x0)) 0) (and (<= (+ (* 1 x18) (* (~1) x17)) 0) (not (<= (+ (* 1 x18) (* (~1) x17)) (~1))))) (and (not (<= (+ (* 1 x16) (* (~1) x0)) 0)) (and (<= (+ (* 1 x18) (* (~1) x1)) 0) (not (<= (+ (* 1 x18) (* (~1) x1)) (~1))))))
    (or (and (<= (+ (* 1 x34) (* (~1) x0)) 0) (and (<= (+ (* 1 x47) (* (~1) x46)) 0) (not (<= (+ (* 1 x47) (* (~1) x46)) (~1))))) (and (not (<= (+ (* 1 x34) (* (~1) x0)) 0)) (and (<= (+ (* 1 x47) (* (~1) x45)) 0) (not (<= (+ (* 1 x47) (* (~1) x45)) (~1))))))
    (or (and (<= (+ (* 1 x36) (* (~1) x0)) 0) (and (<= (+ (* 1 x49) (* (~1) x48)) 0) (not (<= (+ (* 1 x49) (* (~1) x48)) (~1))))) (and (not (<= (+ (* 1 x36) (* (~1) x0)) 0)) (and (<= (+ (* 1 x49) (* (~1) x40)) 0) (not (<= (+ (* 1 x49) (* (~1) x40)) (~1))))))
    (or (and (<= (+ (* 1 x38) (* (~1) x0)) 0) (and (<= (+ (* 1 x51) (* (~1) x50)) 0) (not (<= (+ (* 1 x51) (* (~1) x50)) (~1))))) (and (not (<= (+ (* 1 x38) (* (~1) x0)) 0)) (and (<= (+ (* 1 x51) (* (~1) x44)) 0) (not (<= (+ (* 1 x51) (* (~1) x44)) (~1))))))
    (or (and (<= (+ (* 1 x41) (* (~1) x26)) 0) (not (<= (+ (* 1 x75) (* (~1) x0)) 0))) (and (not (<= (+ (* 1 x41) (* (~1) x26)) 0)) (<= (+ (* 1 x75) (* (~1) x0)) 0)))
    (or (and (<= (+ (* 1 x42) (* (~1) x0)) 0) (and (<= (+ (* 1 x44) (* (~1) x43)) 0) (not (<= (+ (* 1 x44) (* (~1) x43)) (~1))))) (and (not (<= (+ (* 1 x42) (* (~1) x0)) 0)) (and (<= (+ (* 1 x44) (* (~1) x18)) 0) (not (<= (+ (* 1 x44) (* (~1) x18)) (~1))))))
    (or (and (and (<= (+ (* 1 x11) (* (~1) x0)) 0) (not (<= (+ (* 1 x11) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x12) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x11) (* (~1) x0)) 0) (not (<= (+ (* 1 x11) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x12) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x15) (* (~1) x0)) 0) (not (<= (+ (* 1 x15) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x16) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x15) (* (~1) x0)) 0) (not (<= (+ (* 1 x15) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x16) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x19) (* (~1) x0)) 0) (not (<= (+ (* 1 x19) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x20) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x19) (* (~1) x0)) 0) (not (<= (+ (* 1 x19) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x20) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x2) (* (~1) x0)) 0) (not (<= (+ (* 1 x2) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x3) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x2) (* (~1) x0)) 0) (not (<= (+ (* 1 x2) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x3) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x29) (* (~1) x0)) 0) (not (<= (+ (* 1 x29) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x30) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x29) (* (~1) x0)) 0) (not (<= (+ (* 1 x29) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x30) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x3) (* (~1) x0)) 0) (not (<= (+ (* 1 x5) (* (~1) x0)) 0))) (and (<= (+ (* 1 x76) (* (~1) x0)) (~ 1)) (not (<= (+ (* 1 x76) (* (~1) x0)) (~ 2))))) (and (not (<= (+ (* 1 x3) (* (~1) x0)) 0)) (and (<= (+ (* 1 x76) (* (~1) x0)) (~ 1)) (not (<= (+ (* 1 x76) (* (~1) x0)) (~ 2))))))
    (or (and (and (<= (+ (* 1 x33) (* (~1) x0)) 0) (not (<= (+ (* 1 x33) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x34) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x33) (* (~1) x0)) 0) (not (<= (+ (* 1 x33) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x34) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x35) (* (~1) x0)) 0) (not (<= (+ (* 1 x35) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x36) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x35) (* (~1) x0)) 0) (not (<= (+ (* 1 x35) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x36) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x37) (* (~1) x0)) 0) (not (<= (+ (* 1 x37) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x38) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x37) (* (~1) x0)) 0) (not (<= (+ (* 1 x37) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x38) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x41) (* (~1) x0)) 0) (not (<= (+ (* 1 x41) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x42) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x41) (* (~1) x0)) 0) (not (<= (+ (* 1 x41) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x42) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x51) (* (~1) x0)) 0) (not (<= (+ (* 1 x51) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x56) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x51) (* (~1) x0)) 0) (not (<= (+ (* 1 x51) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x56) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x54) (* (~1) x0)) 1) (not (<= (+ (* 1 x54) (* (~1) x0)) 0))) (not (<= (+ (* 1 x55) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x54) (* (~1) x0)) 1) (not (<= (+ (* 1 x54) (* (~1) x0)) 0)))) (<= (+ (* 1 x55) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x55) (* (~1) x0)) 0) (not (<= (+ (* 1 x57) (* (~1) x0)) 0))) (and (<= (+ (* 1 x60) (* (~1) x0)) 1) (not (<= (+ (* 1 x60) (* (~1) x0)) 0)))) (and (not (<= (+ (* 1 x55) (* (~1) x0)) 0)) (and (<= (+ (* 1 x60) (* (~1) x0)) 1) (not (<= (+ (* 1 x60) (* (~1) x0)) 0)))))
    (or (and (and (<= (+ (* 1 x58) (* (~1) x0)) 0) (not (<= (+ (* 1 x58) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x59) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x58) (* (~1) x0)) 0) (not (<= (+ (* 1 x58) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x59) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x60) (* (~1) x0)) 0) (not (<= (+ (* 1 x60) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x61) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x60) (* (~1) x0)) 0) (not (<= (+ (* 1 x60) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x61) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x8) (* (~1) x9)) 0) (not (<= (+ (* 1 x8) (* (~1) x9)) (~1)))) (not (<= (+ (* 1 x10) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x8) (* (~1) x9)) 0) (not (<= (+ (* 1 x8) (* (~1) x9)) (~1))))) (<= (+ (* 1 x10) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x65) (* (~1) x0)) 0) (not (<= (+ (* 1 x65) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x66) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x65) (* (~1) x0)) 0) (not (<= (+ (* 1 x65) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x66) (* (~1) x0)) 0)))
    (or (and (not (<= (+ (* 1 x2) (* (~1) x4)) (~1))) (not (<= (+ (* 1 x5) (* (~1) x0)) 0))) (and (not (not (<= (+ (* 1 x2) (* (~1) x4)) (~1)))) (<= (+ (* 1 x5) (* (~1) x0)) 0)))
    (or (and (not (<= (+ (* 1 x23) (* (~1) x0)) 536870909)) (not (<= (+ (* 1 x24) (* (~1) x0)) 0))) (and (not (not (<= (+ (* 1 x23) (* (~1) x0)) 536870909))) (<= (+ (* 1 x24) (* (~1) x0)) 0)))
    (or (and (not (<= (+ (* 1 x27) (* (~1) x0)) 2147483630)) (not (<= (+ (* 1 x28) (* (~1) x0)) 0))) (and (not (not (<= (+ (* 1 x27) (* (~1) x0)) 2147483630))) (<= (+ (* 1 x28) (* (~1) x0)) 0)))
    (or (and (not (and (<= (+ (* 1 x15) (* (~1) x0)) 0) (not (<= (+ (* 1 x15) (* (~1) x0)) (~ 1))))) (not (<= (+ (* 1 x39) (* (~1) x0)) 0))) (and (not (not (and (<= (+ (* 1 x15) (* (~1) x0)) 0) (not (<= (+ (* 1 x15) (* (~1) x0)) (~ 1)))))) (<= (+ (* 1 x39) (* (~1) x0)) 0)))
    (or (and (not (and (<= (+ (* 1 x60) (* (~1) x0)) 0) (not (<= (+ (* 1 x60) (* (~1) x0)) (~ 1))))) (not (<= (+ (* 1 x62) (* (~1) x0)) 0))) (and (not (not (and (<= (+ (* 1 x60) (* (~1) x0)) 0) (not (<= (+ (* 1 x60) (* (~1) x0)) (~ 1)))))) (<= (+ (* 1 x62) (* (~1) x0)) 0)))
  ))
)