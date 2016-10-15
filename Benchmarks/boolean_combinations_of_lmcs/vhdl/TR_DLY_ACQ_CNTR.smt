(benchmark tr
:extrafuns ((ACQ_RFRNC_0 BitVec[1]))
:extrafuns ((ACQ_RFRNC_1 BitVec[1]))
:extrafuns ((CLK_0 BitVec[1]))
:extrafuns ((CLK_1 BitVec[1]))
:extrafuns ((COUNT_0 BitVec[16]))
:extrafuns ((COUNT_1 BitVec[16]))
:extrafuns ((COUNT_GRT_130_0 BitVec[1]))
:extrafuns ((COUNT_GRT_130_1 BitVec[1]))
:extrafuns ((RESET_0 BitVec[1]))
:extrafuns ((RESET_1 BitVec[1]))
:extrafuns ((RNG_CHNG_FLG_0 BitVec[1]))
:extrafuns ((RNG_CHNG_FLG_1 BitVec[1]))

:exists(COUNT_0 COUNT_1)

:formula 
(and 
(and 
(if_then_else (or (or (not (= RNG_CHNG_FLG_1 RNG_CHNG_FLG_0 )) (not (= RESET_1 RESET_0 )) ) (not (= CLK_1 CLK_0 )) ) 
(if_then_else (or (= RESET_1 bv1[1] ) (= RNG_CHNG_FLG_1 bv1[1] ) ) (= COUNT_1 COUNT_0) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (bvugt COUNT_0 bv255[16] ) (= COUNT_1 COUNT_0) (= COUNT_1 (bvadd COUNT_0 bv1[16]  )) ) (= COUNT_1 COUNT_0))) ) (= COUNT_1 COUNT_0 )) 
(if_then_else (or (or (not (= RNG_CHNG_FLG_1 RNG_CHNG_FLG_0 )) (not (= RESET_1 RESET_0 )) ) (not (= CLK_1 CLK_0 )) ) (if_then_else (or (= RESET_1 bv1[1] ) (= RNG_CHNG_FLG_1 bv1[1] ) ) (= ACQ_RFRNC_1 bv0[1] ) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (bvugt COUNT_0 bv255[16] ) (= ACQ_RFRNC_1 bv1[1] ) (if_then_else (bvugt COUNT_0 bv4095[16] ) (= ACQ_RFRNC_1 ACQ_RFRNC_0 ) (= ACQ_RFRNC_1 bv0[1] ))) (= ACQ_RFRNC_1 ACQ_RFRNC_0 ))) (= ACQ_RFRNC_1 ACQ_RFRNC_0 )) )

 (and (if_then_else (or (or (not (= RNG_CHNG_FLG_1 RNG_CHNG_FLG_0 )) (not (= RESET_1 RESET_0 )) ) (not (= CLK_1 CLK_0 )) ) (if_then_else (or (= RESET_1 bv1[1] ) (= RNG_CHNG_FLG_1 bv1[1] ) ) (= COUNT_1 COUNT_0) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (bvugt COUNT_0 bv255[16] ) (= COUNT_1 COUNT_0) (= COUNT_1 (bvadd COUNT_0 bv1[16]  ))) (= COUNT_1 COUNT_0))) ) (= COUNT_1 COUNT_0 )) 

(if_then_else (or (or (not (= RNG_CHNG_FLG_1 RNG_CHNG_FLG_0 )) (not (= RESET_1 RESET_0 )) ) (not (= CLK_1 CLK_0 )) ) (if_then_else (or (= RESET_1 bv1[1] ) (= RNG_CHNG_FLG_1 bv1[1] ) ) (= COUNT_GRT_130_1 bv0[1] ) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (bvugt COUNT_0 bv255[16] ) (= COUNT_GRT_130_1 COUNT_GRT_130_0 ) (if_then_else (bvugt COUNT_0 bv4095[16] ) (= COUNT_GRT_130_1 bv1[1] ) (= COUNT_GRT_130_1 COUNT_GRT_130_0 ))) (= COUNT_GRT_130_1 COUNT_GRT_130_0 ))) (= COUNT_GRT_130_1 COUNT_GRT_130_0 )) ) )

)


