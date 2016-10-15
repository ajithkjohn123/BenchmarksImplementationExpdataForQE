(benchmark tr
:extrafuns ((CLK_0 BitVec[1]))
:extrafuns ((CLK_1 BitVec[1]))
:extrafuns ((COUNT_0 BitVec[4]))
:extrafuns ((COUNT_1 BitVec[4]))
:extrafuns ((DIN_1 BitVec[16]))
:extrafuns ((DOUT_0 BitVec[16]))
:extrafuns ((DOUT_1 BitVec[16]))
:extrafuns ((EOC_0 BitVec[1]))
:extrafuns ((EOC_1 BitVec[1]))
:extrafuns ((FLG_0 BitVec[1]))
:extrafuns ((FLG_1 BitVec[1]))
:extrafuns ((RESET_0 BitVec[1]))
:extrafuns ((RESET_1 BitVec[1]))
:extrafuns ((VLDEOC_0 BitVec[1]))
:extrafuns ((VLDEOC_1 BitVec[1]))

:exists (FLG_0 FLG_1 COUNT_0 COUNT_1)

:formula (and (and (and (and (if_then_else (or (not (= EOC_1 EOC_0 )) (not (= RESET_1 RESET_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= FLG_1 bv0[1] ) (if_then_else (and (not (= EOC_1 EOC_0 )) (= EOC_1 bv1[1] ) ) (= FLG_1 bv1[1] ) (= FLG_1 FLG_0 ))) (= FLG_1 FLG_0 )) (if_then_else (or (or (not (= CLK_1 CLK_0 )) (not (= RESET_1 RESET_0 )) ) (not (= EOC_1 EOC_0 )) ) (if_then_else (or (= RESET_1 bv1[1] ) (= EOC_1 bv0[1] ) ) (= COUNT_1 bv0[4] ) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (= FLG_0 bv1[1] ) (if_then_else (bvugt COUNT_0 bv7[4] ) (= COUNT_1 COUNT_0 ) (= COUNT_1 (bvadd COUNT_0 bv1[4]  ) )) (= COUNT_1 COUNT_0 )) (= COUNT_1 COUNT_0 ))) (= COUNT_1 COUNT_0 )) ) (and (if_then_else (or (not (= CLK_1 CLK_0 )) (not (= RESET_1 RESET_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= DOUT_1 bv0[16] ) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (and (= COUNT_0 bv5[4] ) (= EOC_1 bv1[1] ) ) (= DOUT_1 DIN_1 ) (= DOUT_1 DOUT_0 )) (= DOUT_1 DOUT_0 ))) (= DOUT_1 DOUT_0 )) true ) ) (and true true ) ) (and (and (and (if_then_else (or (or (not (= CLK_1 CLK_0 )) (not (= RESET_1 RESET_0 )) ) (not (= EOC_1 EOC_0 )) ) (if_then_else (or (= RESET_1 bv1[1] ) (= EOC_1 bv0[1] ) ) (= COUNT_1 bv0[4] ) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (= FLG_0 bv1[1] ) (if_then_else (bvugt COUNT_0 bv7[4] ) (= COUNT_1 COUNT_0 ) (= COUNT_1 (bvadd COUNT_0 bv1[4]  ) )) (= COUNT_1 COUNT_0 )) (= COUNT_1 COUNT_0 ))) (= COUNT_1 COUNT_0 )) (if_then_else (or (not (= EOC_1 EOC_0 )) (not (= RESET_1 RESET_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= FLG_1 bv0[1] ) (if_then_else (and (not (= EOC_1 EOC_0 )) (= EOC_1 bv1[1] ) ) (= FLG_1 bv1[1] ) (= FLG_1 FLG_0 ))) (= FLG_1 FLG_0 )) ) (if_then_else (or (or (not (= CLK_1 CLK_0 )) (not (= RESET_1 RESET_0 )) ) (not (= EOC_1 EOC_0 )) ) (if_then_else (or (= RESET_1 bv1[1] ) (= EOC_1 bv0[1] ) ) (= VLDEOC_1 bv0[1] ) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (= FLG_0 bv1[1] ) (if_then_else (bvugt COUNT_0 bv7[4] ) (= VLDEOC_1 bv1[1] ) (= VLDEOC_1 VLDEOC_0 )) (= VLDEOC_1 VLDEOC_0 )) (= VLDEOC_1 VLDEOC_0 ))) (= VLDEOC_1 VLDEOC_0 )) ) (and true true ) ) )

)


