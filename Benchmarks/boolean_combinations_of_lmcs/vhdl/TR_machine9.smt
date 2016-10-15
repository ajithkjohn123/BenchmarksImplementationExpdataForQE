(benchmark test
:extrafuns ((mystate_0_0 BitVec[32]))
:extrafuns ((mystate_0_1 BitVec[32]))
:extrafuns ((rd_0_1 BitVec[1]))
:extrafuns ((reg0_0_0 BitVec[32]))
:extrafuns ((reg0_0_1 BitVec[32]))
:extrafuns ((reg1_0_0 BitVec[32]))
:extrafuns ((reg1_0_1 BitVec[32]))
:extrafuns ((reg2_0_0 BitVec[32]))
:extrafuns ((reg2_0_1 BitVec[32]))
:extrafuns ((reg3_0_0 BitVec[32]))
:extrafuns ((reg3_0_1 BitVec[32]))
:extrafuns ((reset_0_1 BitVec[1]))
:extrafuns ((state_0_0 BitVec[1]))
:extrafuns ((state_0_1 BitVec[1]))

:exists ( mystate_0_0  reg0_0_0  reg1_0_0  reg2_0_0  reg3_0_0  state_0_0 )

:formula (if_then_else (= reset_0_1 bv1[1] ) (= rd_0_1 bv0[1] ) (if_then_else (= state_0_0 bv0[1] ) (= rd_0_1 bv1[1] ) (if_then_else (= state_0_0 bv1[1] ) (if_then_else (= mystate_0_0 bv0[32] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv1[32] ) (if_then_else (bvugt reg0_0_0 reg1_0_0 ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] )) (if_then_else (= mystate_0_0 bv2[32] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv3[32] ) (= rd_0_1 bv0[1] ) (if_then_else (= mystate_0_0 bv4[32] ) (if_then_else (bvugt reg0_0_0 reg1_0_0 ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] )) (if_then_else (= mystate_0_0 bv5[32] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv6[32] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv7[32] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv8[32] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv9[32] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv16[32] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv17[32] ) (if_then_else (bvugt reg2_0_0 reg3_0_0 ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] )) (if_then_else (= mystate_0_0 bv18[32] ) (= rd_0_1 bv0[1] ) (if_then_else (= mystate_0_0 bv19[32] ) (= rd_0_1 bv0[1] ) (if_then_else (= mystate_0_0 bv20[32] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv21[32] ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] ))))))))))))))))) (= rd_0_1 bv0[1] ))))

)


