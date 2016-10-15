(benchmark tr
:extrafuns ((CLK_1 BitVec[1]))
:extrafuns ((CLK_2 BitVec[1]))
:extrafuns ((CLK_3 BitVec[1]))
:extrafuns ((COUNT_1 BitVec[4]))
:extrafuns ((COUNT_2 BitVec[4]))
:extrafuns ((DIN_1 BitVec[16]))
:extrafuns ((DOUT_1 BitVec[16]))
:extrafuns ((DOUT_0 BitVec[16]))
:extrafuns ((EOC_1 BitVec[1]))
:extrafuns ((EOC_2 BitVec[1]))
:extrafuns ((EOC_3 BitVec[1]))
:extrafuns ((EOC_4 BitVec[1]))
:extrafuns ((FLG_1 BitVec[1]))
:extrafuns ((FLG_2 BitVec[1]))
:extrafuns ((FLG_3 BitVec[1]))
:extrafuns ((RESET_1 BitVec[1]))
:extrafuns ((RESET_2 BitVec[1]))
:extrafuns ((RESET_3 BitVec[1]))
:extrafuns ((RESET_4 BitVec[1]))
:extrafuns ((VLDEOC_1 BitVec[1]))
:extrafuns ((VLDEOC_0 BitVec[1]))

:exists(FLG_1 FLG_2 FLG_3 COUNT_1 COUNT_2)


:formula (and (and (and (if_then_else (or (not (= CLK_1 CLK_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (= DOUT_0 bv0[16] ) (if_then_else (and (not (= CLK_1 CLK_2 )) (= CLK_1 bv1[1] ) ) (if_then_else (and (= COUNT_1 bv5[4] ) (= EOC_1 bv1[1] ) ) (= DOUT_0 DIN_1 ) (= DOUT_0 DOUT_1 )) (= DOUT_0 DOUT_1 ))) (= DOUT_0 DOUT_1 )) (if_then_else (or (or (not (= CLK_2 CLK_3 )) (not (= RESET_2 RESET_3 )) ) (not (= EOC_2 EOC_3 )) ) (if_then_else (or (= RESET_2 bv1[1] ) (= EOC_2 bv0[1] ) ) (= COUNT_1 bv0[4] ) (if_then_else (and (not (= CLK_2 CLK_3 )) (= CLK_2 bv1[1] ) ) (if_then_else (= FLG_2 bv1[1] ) (if_then_else (bvugt COUNT_2 bv7[4] ) (= COUNT_1 COUNT_2 ) (= COUNT_1 (bvadd COUNT_2 bv1[4]  ) )) (= COUNT_1 COUNT_2 )) (= COUNT_1 COUNT_2 ))) (= COUNT_1 COUNT_2 )) ) (if_then_else (or (not (= EOC_3 EOC_4 )) (not (= RESET_3 RESET_4 )) ) (if_then_else (= RESET_3 bv1[1] ) (= FLG_2 bv0[1] ) (if_then_else (and (not (= EOC_3 EOC_4 )) (= EOC_3 bv1[1] ) ) (= FLG_2 bv1[1] ) (= FLG_2 FLG_3 ))) (= FLG_2 FLG_3 )) ) (and (and (if_then_else (or (or (not (= CLK_1 CLK_2 )) (not (= RESET_1 RESET_2 )) ) (not (= EOC_1 EOC_2 )) ) (if_then_else (or (= RESET_1 bv1[1] ) (= EOC_1 bv0[1] ) ) (= VLDEOC_0 bv0[1] ) (if_then_else (and (not (= CLK_1 CLK_2 )) (= CLK_1 bv1[1] ) ) (if_then_else (= FLG_1 bv1[1] ) (if_then_else (bvugt COUNT_1 bv7[4] ) (= VLDEOC_0 bv1[1] ) (= VLDEOC_0 VLDEOC_1 )) (= VLDEOC_0 VLDEOC_1 )) (= VLDEOC_0 VLDEOC_1 ))) (= VLDEOC_0 VLDEOC_1 )) (if_then_else (or (or (not (= CLK_2 CLK_3 )) (not (= RESET_2 RESET_3 )) ) (not (= EOC_2 EOC_3 )) ) (if_then_else (or (= RESET_2 bv1[1] ) (= EOC_2 bv0[1] ) ) (= COUNT_1 bv0[4] ) (if_then_else (and (not (= CLK_2 CLK_3 )) (= CLK_2 bv1[1] ) ) (if_then_else (= FLG_2 bv1[1] ) (if_then_else (bvugt COUNT_2 bv7[4] ) (= COUNT_1 COUNT_2 ) (= COUNT_1 (bvadd COUNT_2 bv1[4]  ) )) (= COUNT_1 COUNT_2 )) (= COUNT_1 COUNT_2 ))) (= COUNT_1 COUNT_2 )) ) (if_then_else (or (not (= EOC_2 EOC_3 )) (not (= RESET_2 RESET_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (= FLG_1 bv0[1] ) (if_then_else (and (not (= EOC_2 EOC_3 )) (= EOC_2 bv1[1] ) ) (= FLG_1 bv1[1] ) (= FLG_1 FLG_2 ))) (= FLG_1 FLG_2 )) ) )

)


