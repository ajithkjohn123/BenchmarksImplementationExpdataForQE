(benchmark tr
:extrafuns ((CLK_0 BitVec[1]))
:extrafuns ((CLK_1 BitVec[1]))
:extrafuns ((CRRNT_PWR_0 BitVec[16]))
:extrafuns ((CRRNT_PWR_1 BitVec[16]))
:extrafuns ((CRRNT_RNG_1 BitVec[8]))
:extrafuns ((LST_RNG_FLG_0 BitVec[1]))
:extrafuns ((LST_RNG_FLG_1 BitVec[1]))
:extrafuns ((RESET_0 BitVec[1]))
:extrafuns ((RESET_1 BitVec[1]))
:extrafuns ((RFRNC_PWR_0 BitVec[16]))
:extrafuns ((RFRNC_PWR_1 BitVec[16]))
:extrafuns ((RFRNC_PWR_LUT_0 BitVec[16]))
:extrafuns ((RFRNC_PWR_LUT_1 BitVec[16]))
:extrafuns ((X_1_0 BitVec[16]))

:exists(LST_RNG_FLG_0 LST_RNG_FLG_1 CRRNT_PWR_0 CRRNT_PWR_1)

:formula (and (if_then_else (or (or (not (= LST_RNG_FLG_1 LST_RNG_FLG_0 )) (not (= CRRNT_PWR_1 X_1_0 )) ) (not (= RFRNC_PWR_LUT_1 RFRNC_PWR_LUT_1 )) ) (if_then_else (= LST_RNG_FLG_1 bv1[1] ) (if_then_else (and (bvule bv4704[16] CRRNT_PWR_1 ) (bvule CRRNT_PWR_1 bv9504[16] ) ) (= RFRNC_PWR_1 bv4640[16] ) (= RFRNC_PWR_1 RFRNC_PWR_LUT_1 )) (= RFRNC_PWR_1 RFRNC_PWR_LUT_1 )) (if_then_else (or (or (not (= LST_RNG_FLG_0 LST_RNG_FLG_0 )) (not (= X_1_0 CRRNT_PWR_0 )) ) (not (= RFRNC_PWR_LUT_1 RFRNC_PWR_LUT_0 )) ) (if_then_else (= LST_RNG_FLG_0 bv1[1] ) (if_then_else (and (bvule bv4704[16] X_1_0 ) (bvule X_1_0 bv9504[16] ) ) (= RFRNC_PWR_1 bv4640[16] ) (= RFRNC_PWR_1 RFRNC_PWR_LUT_1 )) (= RFRNC_PWR_1 RFRNC_PWR_LUT_1 )) (= RFRNC_PWR_1 RFRNC_PWR_0 ))) (if_then_else (or (not (= CLK_1 CLK_0 )) (not (= RESET_1 RESET_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= LST_RNG_FLG_1 bv0[1] ) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (= CRRNT_RNG_1 bv15[8] ) (if_then_else (bvuge X_1_0 bv4677[16] ) (= LST_RNG_FLG_1 bv1[1] ) (if_then_else (bvult X_1_0 bv4640[16] ) (if_then_else (= LST_RNG_FLG_0 bv1[1] ) (= LST_RNG_FLG_1 bv0[1] ) (= LST_RNG_FLG_1 LST_RNG_FLG_0 )) (= LST_RNG_FLG_1 LST_RNG_FLG_0 ))) (= LST_RNG_FLG_1 LST_RNG_FLG_0 )) (= LST_RNG_FLG_1 LST_RNG_FLG_0 ))) (= LST_RNG_FLG_1 LST_RNG_FLG_0 )) )

)


