(benchmark tr
:extrafuns ((A_GRT_B_2 BitVec[1]))
:extrafuns ((A_GRT_B_3 BitVec[1]))
:extrafuns ((COMP2DIN_2 BitVec[23]))
:extrafuns ((COMP2DIN_3 BitVec[23]))
:extrafuns ((COMP2DIN_4 BitVec[23]))
:extrafuns ((FDBCK_SUM_2 BitVec[23]))
:extrafuns ((FDBCK_SUM_3 BitVec[23]))
:extrafuns ((FDBCK_SUM_4 BitVec[23]))
:extrafuns ((OPRNR_1 BitVec[1]))
:extrafuns ((SUM128UL_1 BitVec[23]))
:extrafuns ((SUM128UL_0 BitVec[23]))
:extrafuns ((SUM_SUB__ADD_SUB__X_1 BitVec[22]))
:extrafuns ((SUM_SUB__ADD_SUB__Y_1 BitVec[22]))

:exists(A_GRT_B_2 A_GRT_B_3 FDBCK_SUM_2 FDBCK_SUM_3 FDBCK_SUM_4 OPRNR_1 SUM_SUB__ADD_SUB__X_1 SUM_SUB__ADD_SUB__Y_1 COMP2DIN_2 COMP2DIN_3 COMP2DIN_4)


:formula (and (and (and (and (and (if_then_else (= OPRNR_1 bv0[1] ) (= (extract[21 :0 ] SUM128UL_0) (bvadd SUM_SUB__ADD_SUB__X_1 SUM_SUB__ADD_SUB__Y_1 ) ) (= (extract[21 :0 ] SUM128UL_0) (bvadd SUM_SUB__ADD_SUB__X_1 (bvmul bv65535[22] SUM_SUB__ADD_SUB__Y_1 )) )) (if_then_else (= A_GRT_B_2 bv1[1] ) (= SUM_SUB__ADD_SUB__X_1 (extract[21 :0 ] FDBCK_SUM_2) ) (= SUM_SUB__ADD_SUB__X_1 (extract[21 :0 ] COMP2DIN_2) )) ) (if_then_else (= A_GRT_B_2 bv1[1] ) (= SUM_SUB__ADD_SUB__Y_1 (extract[21 :0 ] COMP2DIN_2) ) (= SUM_SUB__ADD_SUB__Y_1 (extract[21 :0 ] FDBCK_SUM_2) )) ) (= (extract[22 :22 ] SUM128UL_0) (extract[22 :22 ] SUM128UL_1) ) ) (if_then_else (or (not (= (extract[21 :0 ] FDBCK_SUM_3) (extract[21 :0 ] FDBCK_SUM_4) )) (not (= (extract[21 :0 ] COMP2DIN_3) (extract[21 :0 ] COMP2DIN_4) )) ) (if_then_else (bvuge (extract[21 :0 ] FDBCK_SUM_3) (extract[21 :0 ] COMP2DIN_3) ) (= A_GRT_B_2 bv1[1] ) (= A_GRT_B_2 bv0[1] )) (= A_GRT_B_2 A_GRT_B_3 )) ) (= OPRNR_1 (bvadd (extract[22 :22 ] FDBCK_SUM_2) (extract[22 :22 ] COMP2DIN_2) ) ) )

)


