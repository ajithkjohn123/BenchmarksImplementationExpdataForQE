(benchmark i_gnt_wm_new_window_real
:extrafuns ((x0 Int))
:extrafuns ((x100 Int))
:extrafuns ((x101 Int))
:extrafuns ((x102 Int))
:extrafuns ((x103 Int))
:extrafuns ((x104 Int))
:extrafuns ((x105 Int))
:extrafuns ((x106 Int))
:extrafuns ((x86 Int))
:extrafuns ((x87 Int))
:extrafuns ((x88 Int))
:extrafuns ((x89 Int))
:extrafuns ((x90 Int))
:extrafuns ((x91 Int))
:extrafuns ((x92 Int))
:extrafuns ((x93 Int))
:extrafuns ((x94 Int))
:extrafuns ((x95 Int))
:extrafuns ((x96 Int))
:extrafuns ((x97 Int))
:extrafuns ((x98 Int))
:extrafuns ((x99 Int))
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
 (x62 Int)
 (x63 Int)
 (x64 Int)
 (x65 Int)
 (x66 Int)
 (x67 Int)
 (x68 Int)
 (x69 Int)
 (x7 Int)
 (x70 Int)
 (x71 Int)
 (x72 Int)
 (x73 Int)
 (x74 Int)
 (x75 Int)
 (x76 Int)
 (x77 Int)
 (x78 Int)
 (x79 Int)
 (x8 Int)
 (x80 Int)
 (x81 Int)
 (x82 Int)
 (x83 Int)
 (x84 Int)
 (x85 Int)
 (x9 Int)

  (and
    (<= (+ (* 1 x33) (* (~1) x25)) 0)
    (<= (+ (* 1 x40) (* (~1) x27)) 0)
    (and (<= (+ (* 1 x24) (* (~1) x23) ) 1 ) (not (<= (+ (* 1 x24) (* (~1) x23) ) 0) ))
    (and (<= (+ (* 1 x5) (* (~1) x0)) 0) (and (<= (+ (* 1 x10) (* (~1) x4)) 0) (not (<= (+ (* 1 x10) (* (~1) x4)) (~1)))))
    (and (<= (+ (* 1 x59) (* (~1) x0)) 0) (and (<= (+ (* 1 x67) (* (~1) x58)) 0) (not (<= (+ (* 1 x67) (* (~1) x58)) (~1)))))
    (and (<= (+ (* 1 x59) (* (~1) x0)) 0) (and (<= (+ (* 1 x68) (* (~1) x58)) 0) (not (<= (+ (* 1 x68) (* (~1) x58)) (~1)))))
    (and (<= (+ (* 1 x61) (* (~1) x0)) 0) (and (<= (+ (* 1 x66) (* (~1) x60)) 0) (not (<= (+ (* 1 x66) (* (~1) x60)) (~1)))))
    (and (<= (+ (* 1 x76) (* (~1) x0)) 0) (and (<= (+ (* 1 x84) (* (~1) x67)) 0) (not (<= (+ (* 1 x84) (* (~1) x67)) (~1)))))
    (and (<= (+ (* 1 x78) (* (~1) x0)) 0) (and (<= (+ (* 1 x83) (* (~1) x77)) 0) (not (<= (+ (* 1 x83) (* (~1) x77)) (~1)))))
    (and (and (or (and (<= (+ (* 1 x69) (* (~1) x0)) 0) (not (<= (+ (* 1 x71) (* (~1) x0)) 0))) (not (<= (+ (* 1 x69) (* (~1) x0)) 0))) (not (<= (+ (* 1 x75) (* (~1) x0)) 0))) (and (<= (+ (* 1 x96) (* (~1) x54)) 0) (not (<= (+ (* 1 x96) (* (~1) x54)) (~1)))))
    (and (not (<= (+ (* 1 x57) (* (~1) x0)) 0)) (and (<= (+ (* 1 x97) (* (~1) x54)) 0) (not (<= (+ (* 1 x97) (* (~1) x54)) (~1)))))
    (not (<= (+ (* 1 x18) (* (~1) x17)) 0))
    (not (<= (+ (* 1 x20) (* (~1) x19)) 0))
    (not (<= (+ (* 1 x30) (* (~1) x18)) 0))
    (not (<= (+ (* 1 x37) (* (~1) x20)) 0))
    (or (and (<= (+ (* 1 x15) (* (~1) x0)) 0) (not (<= (+ (* 1 x28) (* (~1) x0)) 0))) (and (not (<= (+ (* 1 x15) (* (~1) x0)) 0)) (<= (+ (* 1 x28) (* (~1) x0)) 0)))
    (or (and (<= (+ (* 1 x16) (* (~1) x0)) 0) (not (<= (+ (* 1 x29) (* (~1) x0)) 0))) (and (not (<= (+ (* 1 x16) (* (~1) x0)) 0)) (<= (+ (* 1 x29) (* (~1) x0)) 0)))
    (or (and (<= (+ (* 1 x22) (* (~1) x0)) 0) (and (<= (+ (* 1 x25) (* (~1) x24)) 0) (not (<= (+ (* 1 x25) (* (~1) x24)) (~1))))) (and (not (<= (+ (* 1 x22) (* (~1) x0)) 0)) (and (<= (+ (* 1 x25) (* (~1) x0)) (~ 1)) (not (<= (+ (* 1 x25) (* (~1) x0)) (~ 2))))))
    (or (and (<= (+ (* 1 x22) (* (~1) x0)) 0) (and (<= (+ (* 1 x27) (* (~1) x26)) 0) (not (<= (+ (* 1 x27) (* (~1) x26)) (~1))))) (and (not (<= (+ (* 1 x22) (* (~1) x0)) 0)) (and (<= (+ (* 1 x27) (* (~1) x0)) (~ 2)) (not (<= (+ (* 1 x27) (* (~1) x0)) (~ 3))))))
    (or (and (<= (+ (* 1 x25) (* (~1) x18)) 0) (not (<= (+ (* 1 x44) (* (~1) x0)) 0))) (and (not (<= (+ (* 1 x25) (* (~1) x18)) 0)) (<= (+ (* 1 x44) (* (~1) x0)) 0)))
    (or (and (<= (+ (* 1 x27) (* (~1) x20)) 0) (not (<= (+ (* 1 x45) (* (~1) x0)) 0))) (and (not (<= (+ (* 1 x27) (* (~1) x20)) 0)) (<= (+ (* 1 x45) (* (~1) x0)) 0)))
    (or (and (<= (+ (* 1 x30) (* (~1) x25)) 0) (not (<= (+ (* 1 x32) (* (~1) x0)) 0))) (and (not (<= (+ (* 1 x30) (* (~1) x25)) 0)) (<= (+ (* 1 x32) (* (~1) x0)) 0)))
    (or (and (<= (+ (* 1 x32) (* (~1) x0)) 0) (and (<= (+ (* 1 x36) (* (~1) x35)) 0) (not (<= (+ (* 1 x36) (* (~1) x35)) (~1))))) (and (not (<= (+ (* 1 x32) (* (~1) x0)) 0)) (and (<= (+ (* 1 x36) (* (~1) x31)) 0) (not (<= (+ (* 1 x36) (* (~1) x31)) (~1))))))
    (or (and (<= (+ (* 1 x33) (* (~1) x0)) 0) (not (<= (+ (* 1 x34) (* (~1) x0)) 0))) (and (not (<= (+ (* 1 x33) (* (~1) x0)) 0)) (<= (+ (* 1 x34) (* (~1) x0)) 0)))
    (or (and (<= (+ (* 1 x37) (* (~1) x27)) 0) (not (<= (+ (* 1 x39) (* (~1) x0)) 0))) (and (not (<= (+ (* 1 x37) (* (~1) x27)) 0)) (<= (+ (* 1 x39) (* (~1) x0)) 0)))
    (or (and (<= (+ (* 1 x39) (* (~1) x0)) 0) (and (<= (+ (* 1 x43) (* (~1) x42)) 0) (not (<= (+ (* 1 x43) (* (~1) x42)) (~1))))) (and (not (<= (+ (* 1 x39) (* (~1) x0)) 0)) (and (<= (+ (* 1 x43) (* (~1) x38)) 0) (not (<= (+ (* 1 x43) (* (~1) x38)) (~1))))))
    (or (and (<= (+ (* 1 x40) (* (~1) x0)) 0) (not (<= (+ (* 1 x41) (* (~1) x0)) 0))) (and (not (<= (+ (* 1 x40) (* (~1) x0)) 0)) (<= (+ (* 1 x41) (* (~1) x0)) 0)))
    (or (and (<= (+ (* 1 x49) (* (~1) x0)) 0) (and (<= (+ (* 1 x51) (* (~1) x50)) 0) (not (<= (+ (* 1 x51) (* (~1) x50)) (~1))))) (and (not (<= (+ (* 1 x49) (* (~1) x0)) 0)) (and (<= (+ (* 1 x51) (* (~1) x0)) (~ 2)) (not (<= (+ (* 1 x51) (* (~1) x0)) (~ 3))))))
    (or (and (<= (+ (* 1 x49) (* (~1) x0)) 0) (and (<= (+ (* 1 x53) (* (~1) x52)) 0) (not (<= (+ (* 1 x53) (* (~1) x52)) (~1))))) (and (not (<= (+ (* 1 x49) (* (~1) x0)) 0)) (and (<= (+ (* 1 x53) (* (~1) x0)) (~ 2)) (not (<= (+ (* 1 x53) (* (~1) x0)) (~ 3))))))
    (or (and (<= (+ (* 1 x63) (* (~1) x0)) 0) (and (<= (+ (* 1 x65) (* (~1) x62)) 0) (not (<= (+ (* 1 x65) (* (~1) x62)) (~1))))) (and (not (<= (+ (* 1 x63) (* (~1) x0)) 0)) (and (<= (+ (* 1 x65) (* (~1) x64)) 0) (not (<= (+ (* 1 x65) (* (~1) x64)) (~1))))))
    (or (and (<= (+ (* 1 x7) (* (~1) x0)) 0) (and (<= (+ (* 1 x9) (* (~1) x6)) 0) (not (<= (+ (* 1 x9) (* (~1) x6)) (~1))))) (and (not (<= (+ (* 1 x7) (* (~1) x0)) 0)) (and (<= (+ (* 1 x9) (* (~1) x8)) 0) (not (<= (+ (* 1 x9) (* (~1) x8)) (~1))))))
    (or (and (<= (+ (* 1 x80) (* (~1) x0)) 0) (and (<= (+ (* 1 x82) (* (~1) x79)) 0) (not (<= (+ (* 1 x82) (* (~1) x79)) (~1))))) (and (not (<= (+ (* 1 x80) (* (~1) x0)) 0)) (and (<= (+ (* 1 x82) (* (~1) x81)) 0) (not (<= (+ (* 1 x82) (* (~1) x81)) (~1))))))
    (or (and (<= (+ (* 1 x92) (* (~1) x0)) 0) (and (<= (+ (* 1 x94) (* (~1) x93)) 0) (not (<= (+ (* 1 x94) (* (~1) x93)) (~1))))) (and (not (<= (+ (* 1 x92) (* (~1) x0)) 0)) (and (<= (+ (* 1 x94) (* (~1) x89)) 0) (not (<= (+ (* 1 x94) (* (~1) x89)) (~1))))))
    (or (and (and (<= (+ (* 1 x1) (* (~1) x0)) 0) (not (<= (+ (* 1 x1) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x2) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x1) (* (~1) x0)) 0) (not (<= (+ (* 1 x1) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x2) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x11) (* (~1) x0)) 0) (not (<= (+ (* 1 x11) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x12) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x11) (* (~1) x0)) 0) (not (<= (+ (* 1 x11) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x12) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x21) (* (~1) x0)) 0) (not (<= (+ (* 1 x21) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x22) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x21) (* (~1) x0)) 0) (not (<= (+ (* 1 x21) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x22) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x3) (* (~1) x0)) 0) (not (<= (+ (* 1 x3) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x69) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x3) (* (~1) x0)) 0) (not (<= (+ (* 1 x3) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x69) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x4) (* (~1) x0)) 0) (not (<= (+ (* 1 x4) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x5) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x4) (* (~1) x0)) 0) (not (<= (+ (* 1 x4) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x5) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x46) (* (~1) x0)) 0) (not (<= (+ (* 1 x46) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x47) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x46) (* (~1) x0)) 0) (not (<= (+ (* 1 x46) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x47) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x48) (* (~1) x0)) 0) (not (<= (+ (* 1 x48) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x49) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x48) (* (~1) x0)) 0) (not (<= (+ (* 1 x48) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x49) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x55) (* (~1) x56)) 0) (not (<= (+ (* 1 x55) (* (~1) x56)) (~1)))) (not (<= (+ (* 1 x57) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x55) (* (~1) x56)) 0) (not (<= (+ (* 1 x55) (* (~1) x56)) (~1))))) (<= (+ (* 1 x57) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x58) (* (~1) x0)) 0) (not (<= (+ (* 1 x58) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x59) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x58) (* (~1) x0)) 0) (not (<= (+ (* 1 x58) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x59) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x6) (* (~1) x0)) 0) (not (<= (+ (* 1 x6) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x7) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x6) (* (~1) x0)) 0) (not (<= (+ (* 1 x6) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x7) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x60) (* (~1) x0)) 0) (not (<= (+ (* 1 x60) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x61) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x60) (* (~1) x0)) 0) (not (<= (+ (* 1 x60) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x61) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x62) (* (~1) x0)) 0) (not (<= (+ (* 1 x62) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x63) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x62) (* (~1) x0)) 0) (not (<= (+ (* 1 x62) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x63) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x67) (* (~1) x0)) 0) (not (<= (+ (* 1 x67) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x76) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x67) (* (~1) x0)) 0) (not (<= (+ (* 1 x67) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x76) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x70) (* (~1) x0)) 0) (not (<= (+ (* 1 x70) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x71) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x70) (* (~1) x0)) 0) (not (<= (+ (* 1 x70) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x71) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x72) (* (~1) x68)) 0) (not (<= (+ (* 1 x72) (* (~1) x68)) (~1)))) (not (<= (+ (* 1 x73) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x72) (* (~1) x68)) 0) (not (<= (+ (* 1 x72) (* (~1) x68)) (~1))))) (<= (+ (* 1 x73) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x74) (* (~1) x0)) 0) (not (<= (+ (* 1 x74) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x75) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x74) (* (~1) x0)) 0) (not (<= (+ (* 1 x74) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x75) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x77) (* (~1) x0)) 0) (not (<= (+ (* 1 x77) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x78) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x77) (* (~1) x0)) 0) (not (<= (+ (* 1 x77) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x78) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x79) (* (~1) x0)) 0) (not (<= (+ (* 1 x79) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x80) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x79) (* (~1) x0)) 0) (not (<= (+ (* 1 x79) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x80) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x85) (* (~1) x0)) 0) (not (<= (+ (* 1 x85) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x86) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x85) (* (~1) x0)) 0) (not (<= (+ (* 1 x85) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x86) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x100) (* (~1) x101)) 0) (not (<= (+ (* 1 x100) (* (~1) x101)) (~1)))) (not (<= (+ (* 1 x102) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x100) (* (~1) x101)) 0) (not (<= (+ (* 1 x100) (* (~1) x101)) (~1))))) (<= (+ (* 1 x102) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x100) (* (~1) x103)) 0) (not (<= (+ (* 1 x100) (* (~1) x103)) (~1)))) (not (<= (+ (* 1 x104) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x100) (* (~1) x103)) 0) (not (<= (+ (* 1 x100) (* (~1) x103)) (~1))))) (<= (+ (* 1 x104) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x105) (* (~1) x97)) 0) (not (<= (+ (* 1 x105) (* (~1) x97)) (~1)))) (not (<= (+ (* 1 x106) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x105) (* (~1) x97)) 0) (not (<= (+ (* 1 x105) (* (~1) x97)) (~1))))) (<= (+ (* 1 x106) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x86) (* (~1) x0)) 0) (not (<= (+ (* 1 x88) (* (~1) x0)) 0))) (and (<= (+ (* 1 x89) (* (~1) x90)) 0) (not (<= (+ (* 1 x89) (* (~1) x90)) (~1))))) (and (not (<= (+ (* 1 x86) (* (~1) x0)) 0)) (and (<= (+ (* 1 x89) (* (~1) x0)) 0) (not (<= (+ (* 1 x89) (* (~1) x0)) (~ 1))))))
    (or (and (and (<= (+ (* 1 x87) (* (~1) x0)) 0) (not (<= (+ (* 1 x87) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x88) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x87) (* (~1) x0)) 0) (not (<= (+ (* 1 x87) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x88) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x91) (* (~1) x0)) 0) (not (<= (+ (* 1 x91) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x92) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x91) (* (~1) x0)) 0) (not (<= (+ (* 1 x91) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x92) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x94) (* (~1) x0)) 0) (not (<= (+ (* 1 x94) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x95) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x94) (* (~1) x0)) 0) (not (<= (+ (* 1 x94) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x95) (* (~1) x0)) 0)))
    (or (and (and (<= (+ (* 1 x98) (* (~1) x0)) 0) (not (<= (+ (* 1 x98) (* (~1) x0)) (~ 1)))) (not (<= (+ (* 1 x99) (* (~1) x0)) 0))) (and (not (and (<= (+ (* 1 x98) (* (~1) x0)) 0) (not (<= (+ (* 1 x98) (* (~1) x0)) (~ 1))))) (<= (+ (* 1 x99) (* (~1) x0)) 0)))
    (or (and (not (and (<= (+ (* 1 x13) (* (~1) x0)) 0) (not (<= (+ (* 1 x13) (* (~1) x0)) (~ 1))))) (not (<= (+ (* 1 x14) (* (~1) x0)) 0))) (and (not (not (and (<= (+ (* 1 x13) (* (~1) x0)) 0) (not (<= (+ (* 1 x13) (* (~1) x0)) (~ 1)))))) (<= (+ (* 1 x14) (* (~1) x0)) 0)))
  ))
)
