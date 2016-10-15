(benchmark test
:extrafuns ((mystate_0_0 BitVec[8]))
:extrafuns ((mystate_0_1 BitVec[8]))
:extrafuns ((rd_0_1 BitVec[1]))
:extrafuns ((reg0_0_0 BitVec[8]))
:extrafuns ((reg0_0_1 BitVec[8]))
:extrafuns ((reg1_0_0 BitVec[8]))
:extrafuns ((reg1_0_1 BitVec[8]))
:extrafuns ((reg2_0_0 BitVec[8]))
:extrafuns ((reg2_0_1 BitVec[8]))
:extrafuns ((reg3_0_0 BitVec[8]))
:extrafuns ((reg3_0_1 BitVec[8]))
:extrafuns ((reset_0_1 BitVec[1]))
:extrafuns ((state_0_0 BitVec[1]))
:extrafuns ((state_0_1 BitVec[1]))

:exists ( mystate_0_0  reg0_0_0  reg1_0_0  reg2_0_0  reg3_0_0  state_0_0 )

:formula (if_then_else (= reset_0_1 bv1[1] ) (= rd_0_1 bv0[1] ) (if_then_else (= state_0_0 bv0[1] ) (= rd_0_1 bv1[1] ) (if_then_else (= state_0_0 bv1[1] ) (if_then_else (= mystate_0_0 bv0[8] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv1[8] ) (if_then_else (bvugt reg0_0_0 reg1_0_0 ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] )) (if_then_else (= mystate_0_0 bv2[8] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv3[8] ) (= rd_0_1 bv0[1] ) (if_then_else (= mystate_0_0 bv4[8] ) (if_then_else (bvugt reg0_0_0 reg1_0_0 ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] )) (if_then_else (= mystate_0_0 bv5[8] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv6[8] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv7[8] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv8[8] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv9[8] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv16[8] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv17[8] ) (if_then_else (bvugt reg2_0_0 reg3_0_0 ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] )) (if_then_else (= mystate_0_0 bv18[8] ) (= rd_0_1 bv0[1] ) (if_then_else (= mystate_0_0 bv19[8] ) (= rd_0_1 bv0[1] ) (if_then_else (= mystate_0_0 bv20[8] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv21[8] ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] ))))))))))))))))) (= rd_0_1 bv0[1] ))))

)


