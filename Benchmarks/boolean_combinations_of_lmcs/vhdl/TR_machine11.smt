(benchmark test
:extrafuns ((autoname_1 BitVec[1]))
:extrafuns ((autoname_2 BitVec[1]))
:extrafuns ((autoname_3 BitVec[1]))
:extrafuns ((autoname_4 BitVec[1]))
:extrafuns ((count_0_0 BitVec[4]))
:extrafuns ((count_0_1 BitVec[4]))
:extrafuns ((gamma_0_0 BitVec[8]))
:extrafuns ((gamma_0_1 BitVec[8]))
:extrafuns ((ind_0_0 BitVec[4]))
:extrafuns ((ind_0_1 BitVec[4]))
:extrafuns ((k_0_1 BitVec[4]))
:extrafuns ((max_0_0 BitVec[4]))
:extrafuns ((max_0_1 BitVec[4]))
:extrafuns ((nl_0_0 BitVec[4]))
:extrafuns ((nl_0_1 BitVec[4]))
:extrafuns ((nloss_0_0 BitVec[1]))
:extrafuns ((nloss_0_1 BitVec[1]))
:extrafuns ((reset_0_1 BitVec[1]))
:extrafuns ((scan_0_0 BitVec[4]))
:extrafuns ((scan_0_1 BitVec[4]))
:extrafuns ((sound_0_0 BitVec[8]))
:extrafuns ((sound_0_1 BitVec[8]))
:extrafuns ((timebase_0_0 BitVec[4]))
:extrafuns ((timebase_0_1 BitVec[4]))

:exists ( count_0_0  gamma_0_0  ind_0_0  max_0_0  scan_0_0  timebase_0_0 )

:formula (and (if_then_else (= reset_0_1 bv1[1] ) (= nl_0_1 bv0[4] ) (if_then_else (= gamma_0_0 bv0[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv1[8] ) (= nl_0_1 bv0[4] ) (if_then_else (= gamma_0_0 bv2[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv3[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv4[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv5[8] ) (= nl_0_1 bv1[4] ) (if_then_else (= gamma_0_0 bv6[8] ) (if_then_else (= count_0_0 (bvadd (bvmul bv2[4] scan_0_0 ) max_0_0 ) ) (= nl_0_1 bv0[4] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv7[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv8[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv9[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv16[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv17[8] ) (= nl_0_1 bv1[4] ) (if_then_else (= gamma_0_0 bv18[8] ) (if_then_else (= count_0_0 (bvadd (bvmul bv8[4] scan_0_0 ) (bvmul bv2[4] max_0_0 ) ) ) (= nl_0_1 bv0[4] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv19[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv20[8] ) (= nl_0_1 bv1[4] ) (if_then_else (= gamma_0_0 bv21[8] ) (if_then_else (= count_0_0 (bvadd (bvmul bv8[4] scan_0_0 ) (bvmul bv4[4] max_0_0 ) ) ) (= nl_0_1 bv0[4] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv22[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv23[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv24[8] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv25[8] ) (= nl_0_1 bv1[4] ) (if_then_else (= gamma_0_0 bv32[8] ) (if_then_else (= count_0_0 (bvadd (bvmul bv2[4] scan_0_0 ) (bvmul bv4[4] max_0_0 ) ) ) (= nl_0_1 bv0[4] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv33[8] ) (if_then_else (= count_0_0 bv0[4] ) (if_then_else (= max_0_0 scan_0_0 ) (= nl_0_1 nl_0_0 ) (= nl_0_1 bv1[4] )) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv34[8] ) (if_then_else (= count_0_0 bv0[4] ) (= nl_0_1 bv0[4] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv35[8] ) (if_then_else (= count_0_0 (bvadd (bvadd (bvmul bv4[4] scan_0_0 ) (bvmul bv2[4] timebase_0_0 ) ) (bvmul bv8[4] ind_0_0 ) ) ) (= nl_0_1 bv1[4] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv36[8] ) (if_then_else (= count_0_0 (bvadd (bvadd (bvmul bv4[4] scan_0_0 ) (bvmul bv2[4] timebase_0_0 ) ) (bvmul bv8[4] ind_0_0 ) ) ) (= nl_0_1 bv1[4] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv37[8] ) (if_then_else (= count_0_0 (bvadd (bvmul bv4[4] scan_0_0 ) (bvmul bv2[4] ind_0_0 ) ) ) (= nl_0_1 bv0[4] ) (= nl_0_1 nl_0_0 )) (= nl_0_1 nl_0_0 )))))))))))))))))))))))))))) (if_then_else (= reset_0_1 bv1[1] ) (= nloss_0_1 bv0[1] ) (if_then_else (= gamma_0_0 bv0[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv1[8] ) (= nloss_0_1 bv0[1] ) (if_then_else (= gamma_0_0 bv2[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv3[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv4[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv5[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv6[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv7[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv8[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv9[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv16[8] ) (if_then_else (= count_0_0 (bvadd (bvmul bv4[4] scan_0_0 ) (bvmul bv2[4] max_0_0 ) ) ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_1 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_2 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_3 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_4 bv1[1] ) (= nloss_0_1 bv1[1] ) (= nloss_0_1 nloss_0_0 )))))) (= nloss_0_1 nloss_0_0 ))))))))))))) )

)


