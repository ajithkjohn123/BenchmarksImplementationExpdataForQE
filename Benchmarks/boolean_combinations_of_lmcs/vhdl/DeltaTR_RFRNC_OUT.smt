(benchmark tr
:extrafuns ((CLK_2 BitVec[1]))
:extrafuns ((CLK_3 BitVec[1]))
:extrafuns ((CRRNT_PWR_1 BitVec[16]))
:extrafuns ((CRRNT_PWR_2 BitVec[16]))
:extrafuns ((CRRNT_RNG_2 BitVec[8]))
:extrafuns ((LST_RNG_FLG_1 BitVec[1]))
:extrafuns ((LST_RNG_FLG_2 BitVec[1]))
:extrafuns ((RESET_2 BitVec[1]))
:extrafuns ((RESET_3 BitVec[1]))
:extrafuns ((RFRNC_PWR_1 BitVec[16]))
:extrafuns ((RFRNC_PWR_0 BitVec[16]))
:extrafuns ((RFRNC_PWR_LUT_1 BitVec[16]))
:extrafuns ((RFRNC_PWR_LUT_2 BitVec[16]))

:exists(CRRNT_PWR_1 CRRNT_PWR_2 CRRNT_PWR_1 CRRNT_PWR_2)


:formula (and (if_then_else (or (or (not (= LST_RNG_FLG_1 LST_RNG_FLG_2 )) (not (= CRRNT_PWR_1 CRRNT_PWR_2 )) ) (not (= RFRNC_PWR_LUT_1 RFRNC_PWR_LUT_2 )) ) (if_then_else (= LST_RNG_FLG_1 bv1[1] ) (if_then_else (and (bvule bv4704[16] CRRNT_PWR_1 ) (bvule CRRNT_PWR_1 bv9504[16] ) ) (= RFRNC_PWR_0 bv4640[16] ) (= RFRNC_PWR_0 RFRNC_PWR_LUT_1 )) (= RFRNC_PWR_0 RFRNC_PWR_LUT_1 )) (= RFRNC_PWR_0 RFRNC_PWR_1 )) (if_then_else (or (not (= CLK_2 CLK_3 )) (not (= RESET_2 RESET_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (= LST_RNG_FLG_1 bv0[1] ) (if_then_else (and (not (= CLK_2 CLK_3 )) (= CLK_2 bv1[1] ) ) (if_then_else (= CRRNT_RNG_2 bv15[8] ) (if_then_else (bvuge CRRNT_PWR_2 bv4677[16] ) (= LST_RNG_FLG_1 bv1[1] ) (if_then_else (bvult CRRNT_PWR_2 bv4640[16] ) (if_then_else (= LST_RNG_FLG_2 bv1[1] ) (= LST_RNG_FLG_1 bv0[1] ) (= LST_RNG_FLG_1 LST_RNG_FLG_2 )) (= LST_RNG_FLG_1 LST_RNG_FLG_2 ))) (= LST_RNG_FLG_1 LST_RNG_FLG_2 )) (= LST_RNG_FLG_1 LST_RNG_FLG_2 ))) (= LST_RNG_FLG_1 LST_RNG_FLG_2 )) )

)


