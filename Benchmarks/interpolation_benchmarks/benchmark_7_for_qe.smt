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
:extrafuns ((autoname_1 BitVec[22]))
:extrafuns ((autoname_2 BitVec[22]))
:extrafuns ((autoname_3 BitVec[22]))
:extrafuns ((autoname_4 BitVec[22]))
:extrafuns ((autoname_5 BitVec[22]))
:extrafuns ((autoname_6 BitVec[22]))
:extrafuns ((autoname_7 BitVec[22]))
:extrafuns ((autoname_8 BitVec[1]))
:extrafuns ((autoname_9 BitVec[1]))
:extrafuns ((autoname_10 BitVec[1]))
:extrafuns ((autoname_11 BitVec[1]))

:exists(A_GRT_B_0 A_GRT_B_1 OPRNR_0 OPRNR_1 SUM_SUB__ADD_SUB__X_0 SUM_SUB__ADD_SUB__X_1 SUM_SUB__ADD_SUB__Y_0 SUM_SUB__ADD_SUB__Y_1 FDBCK_SUM_0 FDBCK_SUM_1 COMP2DIN_0 COMP2DIN_1)


:formula (and (and (and (and (and (and (and (if_then_else (= OPRNR_1 bv0[1] ) (= autoname_1 (bvadd SUM_SUB__ADD_SUB__X_1 SUM_SUB__ADD_SUB__Y_1 ) ) (= autoname_1 (bvadd (bvmul bv1[22]  SUM_SUB__ADD_SUB__X_1 ) (bvmul bv4194303[22]  SUM_SUB__ADD_SUB__Y_1 ) ) )) (if_then_else (= A_GRT_B_1 bv1[1] ) (= SUM_SUB__ADD_SUB__X_1 autoname_2 ) (= SUM_SUB__ADD_SUB__X_1 autoname_3 )) ) (if_then_else (= A_GRT_B_1 bv1[1] ) (= SUM_SUB__ADD_SUB__Y_1 autoname_3 ) (= SUM_SUB__ADD_SUB__Y_1 autoname_2 )) ) (= autoname_4 autoname_5 ) ) (if_then_else (or (not (= autoname_2 autoname_7 )) (not (= autoname_3 autoname_6 )) ) (if_then_else (bvuge autoname_2 autoname_3 ) (= A_GRT_B_1 bv1[1] ) (= A_GRT_B_1 bv0[1] )) (if_then_else (or (not (= autoname_7 autoname_7 )) (not (= autoname_6 autoname_6 )) ) (if_then_else (bvuge autoname_7 autoname_6 ) (= A_GRT_B_1 bv1[1] ) (= A_GRT_B_1 bv0[1] )) (= A_GRT_B_1 A_GRT_B_0 ))) ) (= OPRNR_1 (bvadd autoname_8 autoname_9 ) ) ) (if_then_else (or (not (= autoname_7 autoname_7 )) (not (= autoname_6 autoname_6 )) ) (if_then_else (bvuge autoname_7 autoname_6 ) (= A_GRT_B_0 bv1[1] ) (= A_GRT_B_0 bv0[1] )) (= A_GRT_B_0 A_GRT_B_0 )) ) (and (and (and (if_then_else (= A_GRT_B_0 bv1[1] ) (= SUM_SUB__ADD_SUB__X_0 autoname_7 ) (= SUM_SUB__ADD_SUB__X_0 autoname_6 )) (if_then_else (= A_GRT_B_0 bv1[1] ) (= SUM_SUB__ADD_SUB__Y_0 autoname_6 ) (= SUM_SUB__ADD_SUB__Y_0 autoname_7 )) ) (if_then_else (or (not (= autoname_7 autoname_7 )) (not (= autoname_6 autoname_6 )) ) (if_then_else (bvuge autoname_7 autoname_6 ) (= A_GRT_B_0 bv1[1] ) (= A_GRT_B_0 bv0[1] )) (= A_GRT_B_0 A_GRT_B_0 )) ) (= OPRNR_0 (bvadd autoname_10 autoname_11 ) ) ) )

)


