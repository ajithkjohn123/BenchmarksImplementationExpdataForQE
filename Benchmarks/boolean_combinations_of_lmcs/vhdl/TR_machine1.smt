(benchmark test
:extrafuns ((count_0_0 BitVec[32]))
:extrafuns ((count_0_1 BitVec[32]))
:extrafuns ((gamma_0_0 BitVec[32]))
:extrafuns ((gamma_0_1 BitVec[32]))
:extrafuns ((ind_0_0 BitVec[32]))
:extrafuns ((ind_0_1 BitVec[32]))
:extrafuns ((max_0_0 BitVec[32]))
:extrafuns ((max_0_1 BitVec[32]))
:extrafuns ((nl_0_0 BitVec[32]))
:extrafuns ((nl_0_1 BitVec[32]))
:extrafuns ((reset_0_1 BitVec[1]))
:extrafuns ((scan_0_0 BitVec[32]))
:extrafuns ((scan_0_1 BitVec[32]))
:extrafuns ((sound_0_0 BitVec[32]))
:extrafuns ((sound_0_1 BitVec[32]))
:extrafuns ((timebase_0_0 BitVec[32]))
:extrafuns ((timebase_0_1 BitVec[32]))

:exists ( count_0_0  gamma_0_0  ind_0_0  max_0_0  scan_0_0  timebase_0_0 )

:formula (if_then_else (= reset_0_1 bv1[1] ) (= nl_0_1 bv0[32] ) (if_then_else (= gamma_0_0 bv0[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv1[32] ) (= nl_0_1 bv0[32] ) (if_then_else (= gamma_0_0 bv2[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv3[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv4[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv5[32] ) (= nl_0_1 bv1[32] ) (if_then_else (= gamma_0_0 bv6[32] ) (if_then_else (= count_0_0 (bvadd (bvmul bv2[32] scan_0_0 ) max_0_0 ) ) (= nl_0_1 bv0[32] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv7[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv8[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv9[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv16[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv17[32] ) (= nl_0_1 bv1[32] ) (if_then_else (= gamma_0_0 bv18[32] ) (if_then_else (= count_0_0 (bvadd (bvmul bv8[32] scan_0_0 ) (bvmul bv2[32] max_0_0 ) ) ) (= nl_0_1 bv0[32] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv19[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv20[32] ) (= nl_0_1 bv1[32] ) (if_then_else (= gamma_0_0 bv21[32] ) (if_then_else (= count_0_0 (bvadd (bvmul bv8[32] scan_0_0 ) (bvmul bv4[32] max_0_0 ) ) ) (= nl_0_1 bv0[32] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv22[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv23[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv24[32] ) (= nl_0_1 nl_0_0 ) (if_then_else (= gamma_0_0 bv25[32] ) (= nl_0_1 bv1[32] ) (if_then_else (= gamma_0_0 bv32[32] ) (if_then_else (= count_0_0 (bvadd (bvmul bv2[32] scan_0_0 ) (bvmul bv4[32] max_0_0 ) ) ) (= nl_0_1 bv0[32] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv33[32] ) (if_then_else (= count_0_0 bv0[32] ) (if_then_else (= max_0_0 scan_0_0 ) (= nl_0_1 nl_0_0 ) (= nl_0_1 bv1[32] )) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv34[32] ) (if_then_else (= count_0_0 bv0[32] ) (= nl_0_1 bv0[32] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv35[32] ) (if_then_else (= count_0_0 (bvadd (bvadd (bvmul bv4[32] scan_0_0 ) (bvmul bv2[32] timebase_0_0 ) ) (bvmul bv8[32] ind_0_0 ) ) ) (= nl_0_1 bv1[32] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv36[32] ) (if_then_else (= count_0_0 (bvadd (bvadd (bvmul bv4[32] scan_0_0 ) (bvmul bv2[32] timebase_0_0 ) ) (bvmul bv8[32] ind_0_0 ) ) ) (= nl_0_1 bv1[32] ) (= nl_0_1 nl_0_0 )) (if_then_else (= gamma_0_0 bv37[32] ) (if_then_else (= count_0_0 (bvadd (bvmul bv4[32] scan_0_0 ) (bvmul bv2[32] ind_0_0 ) ) ) (= nl_0_1 bv0[32] ) (= nl_0_1 nl_0_0 )) (= nl_0_1 nl_0_0 ))))))))))))))))))))))))))))

)


