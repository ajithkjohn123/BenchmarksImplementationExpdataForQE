(benchmark test
:extrafuns ((count_0_0 BitVec[24]))
:extrafuns ((count_0_1 BitVec[24]))
:extrafuns ((gamma_0_0 BitVec[24]))
:extrafuns ((gamma_0_1 BitVec[24]))
:extrafuns ((ind_0_0 BitVec[24]))
:extrafuns ((ind_0_1 BitVec[24]))
:extrafuns ((max_0_0 BitVec[24]))
:extrafuns ((max_0_1 BitVec[24]))
:extrafuns ((nloss_0_0 BitVec[1]))
:extrafuns ((nloss_0_1 BitVec[1]))
:extrafuns ((reset_0_1 BitVec[1]))
:extrafuns ((scan_0_0 BitVec[24]))
:extrafuns ((scan_0_1 BitVec[24]))
:extrafuns ((sound_0_0 BitVec[24]))
:extrafuns ((sound_0_1 BitVec[24]))
:extrafuns ((timebase_0_0 BitVec[24]))
:extrafuns ((timebase_0_1 BitVec[24]))

:exists ( count_0_0  gamma_0_0  ind_0_0  max_0_0  scan_0_0  timebase_0_0 )

:formula (if_then_else (= reset_0_1 bv1[1] ) (= nloss_0_1 bv0[1] ) (if_then_else (= gamma_0_0 bv0[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv1[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv2[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv3[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv4[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv5[24] ) (= nloss_0_1 bv1[1] ) (if_then_else (= gamma_0_0 bv6[24] ) (if_then_else (= count_0_0 (bvadd (bvmul bv2[24] scan_0_0 ) max_0_0 ) ) (= nloss_0_1 bv0[1] ) (= nloss_0_1 nloss_0_0 )) (if_then_else (= gamma_0_0 bv7[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv8[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv9[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv16[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv17[24] ) (= nloss_0_1 bv1[1] ) (if_then_else (= gamma_0_0 bv18[24] ) (if_then_else (= count_0_0 (bvadd (bvmul bv8[24] scan_0_0 ) (bvmul bv2[24] max_0_0 ) ) ) (= nloss_0_1 bv0[1] ) (= nloss_0_1 nloss_0_0 )) (if_then_else (= gamma_0_0 bv19[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv20[24] ) (= nloss_0_1 bv1[1] ) (if_then_else (= gamma_0_0 bv21[24] ) (if_then_else (= count_0_0 (bvadd (bvmul bv8[24] scan_0_0 ) (bvmul bv4[24] max_0_0 ) ) ) (= nloss_0_1 bv0[1] ) (= nloss_0_1 nloss_0_0 )) (if_then_else (= gamma_0_0 bv22[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv23[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv24[24] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv25[24] ) (= nloss_0_1 bv1[1] ) (if_then_else (= gamma_0_0 bv32[24] ) (if_then_else (= count_0_0 (bvadd (bvmul bv2[24] scan_0_0 ) (bvmul bv4[24] max_0_0 ) ) ) (= nloss_0_1 bv0[1] ) (= nloss_0_1 nloss_0_0 )) (if_then_else (= gamma_0_0 bv33[24] ) (if_then_else (= count_0_0 bv0[24] ) (if_then_else (= max_0_0 scan_0_0 ) (= nloss_0_1 nloss_0_0 ) (= nloss_0_1 bv1[1] )) (= nloss_0_1 nloss_0_0 )) (if_then_else (= gamma_0_0 bv34[24] ) (if_then_else (= count_0_0 bv0[24] ) (= nloss_0_1 bv0[1] ) (= nloss_0_1 nloss_0_0 )) (if_then_else (= gamma_0_0 bv35[24] ) (if_then_else (= count_0_0 (bvadd (bvadd (bvmul bv4[24] scan_0_0 ) (bvmul bv2[24] timebase_0_0 ) ) (bvmul bv8[24] ind_0_0 ) ) ) (= nloss_0_1 bv1[1] ) (= nloss_0_1 nloss_0_0 )) (if_then_else (= gamma_0_0 bv36[24] ) (if_then_else (= count_0_0 (bvadd (bvadd (bvmul bv4[24] scan_0_0 ) (bvmul bv2[24] timebase_0_0 ) ) (bvmul bv8[24] ind_0_0 ) ) ) (= nloss_0_1 bv1[1] ) (= nloss_0_1 nloss_0_0 )) (if_then_else (= gamma_0_0 bv37[24] ) (if_then_else (= count_0_0 (bvadd (bvmul bv4[24] scan_0_0 ) (bvmul bv2[24] ind_0_0 ) ) ) (= nloss_0_1 bv0[1] ) (= nloss_0_1 nloss_0_0 )) (= nloss_0_1 nloss_0_0 ))))))))))))))))))))))))))))

)


