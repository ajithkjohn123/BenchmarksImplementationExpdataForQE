(benchmark tr
:extrafuns ((A_GRT_B_0 BitVec[1]))
:extrafuns ((A_GRT_B_1 BitVec[1]))
:extrafuns ((COMP2DIN_0 BitVec[23]))
:extrafuns ((COMP2DIN_1 BitVec[23]))
:extrafuns ((FDBCK_SUM_0 BitVec[23]))
:extrafuns ((FDBCK_SUM_1 BitVec[23]))
:extrafuns ((OPRNR_0 BitVec[1]))
:extrafuns ((OPRNR_1 BitVec[1]))
:extrafuns ((SUM128UL_0 BitVec[23]))
:extrafuns ((SUM128UL_1 BitVec[23]))
:extrafuns ((SUM_SUB__ADD_SUB__X_0 BitVec[22]))
:extrafuns ((SUM_SUB__ADD_SUB__X_1 BitVec[22]))
:extrafuns ((SUM_SUB__ADD_SUB__Y_0 BitVec[22]))
:extrafuns ((SUM_SUB__ADD_SUB__Y_1 BitVec[22]))

:exists(A_GRT_B_0 A_GRT_B_1 OPRNR_0 OPRNR_1 SUM_SUB__ADD_SUB__X_0 SUM_SUB__ADD_SUB__X_1 SUM_SUB__ADD_SUB__Y_0 SUM_SUB__ADD_SUB__Y_1 FDBCK_SUM_0 FDBCK_SUM_1 COMP2DIN_0 COMP2DIN_1)


:formula (and (and (and (and (and (and (and (if_then_else (= OPRNR_1 bv0[1] ) (= (extract[21 :0 ] SUM128UL_1) (bvadd SUM_SUB__ADD_SUB__X_1 SUM_SUB__ADD_SUB__Y_1 ) ) (= (extract[21 :0 ] SUM128UL_1) (bvadd (bvmul bv1[22]  SUM_SUB__ADD_SUB__X_1 ) (bvmul bv4194303[22]  SUM_SUB__ADD_SUB__Y_1 ) ) )) (if_then_else (= A_GRT_B_1 bv1[1] ) (= SUM_SUB__ADD_SUB__X_1 (extract[21 :0 ] FDBCK_SUM_1) ) (= SUM_SUB__ADD_SUB__X_1 (extract[21 :0 ] COMP2DIN_1) )) ) (if_then_else (= A_GRT_B_1 bv1[1] ) (= SUM_SUB__ADD_SUB__Y_1 (extract[21 :0 ] COMP2DIN_1) ) (= SUM_SUB__ADD_SUB__Y_1 (extract[21 :0 ] FDBCK_SUM_1) )) ) (= (extract[22 :22 ] SUM128UL_1) (extract[22 :22 ] SUM128UL_0) ) ) (if_then_else (or (not (= (extract[21 :0 ] FDBCK_SUM_1) (extract[21 :0 ] FDBCK_SUM_0) )) (not (= (extract[21 :0 ] COMP2DIN_1) (extract[21 :0 ] COMP2DIN_0) )) ) (if_then_else (bvuge (extract[21 :0 ] FDBCK_SUM_1) (extract[21 :0 ] COMP2DIN_1) ) (= A_GRT_B_1 bv1[1] ) (= A_GRT_B_1 bv0[1] )) (if_then_else (or (not (= (extract[21 :0 ] FDBCK_SUM_0) (extract[21 :0 ] FDBCK_SUM_0) )) (not (= (extract[21 :0 ] COMP2DIN_0) (extract[21 :0 ] COMP2DIN_0) )) ) (if_then_else (bvuge (extract[21 :0 ] FDBCK_SUM_0) (extract[21 :0 ] COMP2DIN_0) ) (= A_GRT_B_1 bv1[1] ) (= A_GRT_B_1 bv0[1] )) (= A_GRT_B_1 A_GRT_B_0 ))) ) (= OPRNR_1 (bvadd (extract[22 :22 ] FDBCK_SUM_1) (extract[22 :22 ] COMP2DIN_1) ) ) ) (if_then_else (or (not (= (extract[21 :0 ] FDBCK_SUM_0) (extract[21 :0 ] FDBCK_SUM_0) )) (not (= (extract[21 :0 ] COMP2DIN_0) (extract[21 :0 ] COMP2DIN_0) )) ) (if_then_else (bvuge (extract[21 :0 ] FDBCK_SUM_0) (extract[21 :0 ] COMP2DIN_0) ) (= A_GRT_B_0 bv1[1] ) (= A_GRT_B_0 bv0[1] )) (= A_GRT_B_0 A_GRT_B_0 )) ) (and (and (and (if_then_else (= A_GRT_B_0 bv1[1] ) (= SUM_SUB__ADD_SUB__X_0 (extract[21 :0 ] FDBCK_SUM_0) ) (= SUM_SUB__ADD_SUB__X_0 (extract[21 :0 ] COMP2DIN_0) )) (if_then_else (= A_GRT_B_0 bv1[1] ) (= SUM_SUB__ADD_SUB__Y_0 (extract[21 :0 ] COMP2DIN_0) ) (= SUM_SUB__ADD_SUB__Y_0 (extract[21 :0 ] FDBCK_SUM_0) )) ) (if_then_else (or (not (= (extract[21 :0 ] FDBCK_SUM_0) (extract[21 :0 ] FDBCK_SUM_0) )) (not (= (extract[21 :0 ] COMP2DIN_0) (extract[21 :0 ] COMP2DIN_0) )) ) (if_then_else (bvuge (extract[21 :0 ] FDBCK_SUM_0) (extract[21 :0 ] COMP2DIN_0) ) (= A_GRT_B_0 bv1[1] ) (= A_GRT_B_0 bv0[1] )) (= A_GRT_B_0 A_GRT_B_0 )) ) (= OPRNR_0 (bvadd (extract[22 :22 ] FDBCK_SUM_0) (extract[22 :22 ] COMP2DIN_0) ) ) ) )

)


