(benchmark test
:extrafuns ((d_0_0 BitVec[24]))
:extrafuns ((d_0_1 BitVec[24]))
:extrafuns ((m_0_0 BitVec[24]))
:extrafuns ((m_0_1 BitVec[24]))
:extrafuns ((mystate_0_0 BitVec[24]))
:extrafuns ((mystate_0_1 BitVec[24]))
:extrafuns ((r_0_0 BitVec[24]))
:extrafuns ((r_0_1 BitVec[24]))
:extrafuns ((rd_0_1 BitVec[1]))
:extrafuns ((reg0_0_0 BitVec[24]))
:extrafuns ((reg0_0_1 BitVec[24]))
:extrafuns ((reg1_0_0 BitVec[24]))
:extrafuns ((reg1_0_1 BitVec[24]))
:extrafuns ((reg2_0_0 BitVec[24]))
:extrafuns ((reg2_0_1 BitVec[24]))
:extrafuns ((reg3_0_0 BitVec[24]))
:extrafuns ((reg3_0_1 BitVec[24]))
:extrafuns ((reset_0_1 BitVec[1]))
:extrafuns ((state_0_0 BitVec[1]))
:extrafuns ((state_0_1 BitVec[1]))

:exists ( mystate_0_0  reg0_0_0  reg1_0_0  reg2_0_0  reg3_0_0  state_0_0 )

:formula (if_then_else (= reset_0_1 bv1[1] ) (= rd_0_1 bv0[1] ) (if_then_else (= state_0_0 bv0[1] ) (= rd_0_1 bv1[1] ) (if_then_else (= state_0_0 bv1[1] ) (if_then_else (= mystate_0_0 bv0[24] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv1[24] ) (if_then_else (= reg0_0_0 (bvadd (bvmul bv4[24] reg1_0_0 ) (bvmul bv256[24] reg2_0_0 ) ) ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] )) (if_then_else (= mystate_0_0 bv2[24] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv3[24] ) (= rd_0_1 bv0[1] ) (if_then_else (= mystate_0_0 bv4[24] ) (if_then_else (= reg0_0_0 (bvadd (bvmul bv8[24] reg1_0_0 ) bv2[24] ) ) (if_then_else (= reg2_0_0 (bvadd (bvmul bv4[24] reg1_0_0 ) bv2[24] ) ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] )) (= rd_0_1 bv0[1] )) (if_then_else (= mystate_0_0 bv5[24] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv6[24] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv7[24] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv8[24] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv9[24] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv16[24] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv17[24] ) (if_then_else (= reg2_0_0 (bvadd (bvmul bv4[24] reg3_0_0 ) bv16[24] ) ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] )) (if_then_else (= mystate_0_0 bv18[24] ) (= rd_0_1 bv0[1] ) (if_then_else (= mystate_0_0 bv19[24] ) (= rd_0_1 bv0[1] ) (if_then_else (= mystate_0_0 bv20[24] ) (= rd_0_1 bv1[1] ) (if_then_else (= mystate_0_0 bv21[24] ) (= rd_0_1 bv1[1] ) (= rd_0_1 bv0[1] ))))))))))))))))) (= rd_0_1 bv0[1] ))))

)


