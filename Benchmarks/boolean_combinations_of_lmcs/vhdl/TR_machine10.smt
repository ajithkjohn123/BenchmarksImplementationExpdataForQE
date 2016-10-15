(benchmark test
:extrafuns ((autoname_1 BitVec[1]))
:extrafuns ((autoname_2 BitVec[1]))
:extrafuns ((autoname_3 BitVec[1]))
:extrafuns ((autoname_4 BitVec[1]))
:extrafuns ((count_0_0 BitVec[32]))
:extrafuns ((count_0_1 BitVec[32]))
:extrafuns ((gamma_0_0 BitVec[32]))
:extrafuns ((gamma_0_1 BitVec[32]))
:extrafuns ((k_0_1 BitVec[32]))
:extrafuns ((max_0_0 BitVec[32]))
:extrafuns ((max_0_1 BitVec[32]))
:extrafuns ((nloss_0_0 BitVec[1]))
:extrafuns ((nloss_0_1 BitVec[1]))
:extrafuns ((reset_0_1 BitVec[1]))
:extrafuns ((scan_0_0 BitVec[32]))
:extrafuns ((scan_0_1 BitVec[32]))
:extrafuns ((timebase_0_0 BitVec[32]))
:extrafuns ((timebase_0_1 BitVec[32]))

:exists ( count_0_0  gamma_0_0 )

:formula (if_then_else (= reset_0_1 bv1[1] ) (= nloss_0_1 bv0[1] ) (if_then_else (= gamma_0_0 bv0[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv1[32] ) (= nloss_0_1 bv0[1] ) (if_then_else (= gamma_0_0 bv2[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv3[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv4[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv5[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv6[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv7[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv8[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv9[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv16[32] ) (if_then_else (bvuge count_0_0 bv17895697[32] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_1 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_2 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_3 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_4 bv1[1] ) (= nloss_0_1 bv1[1] ) (= nloss_0_1 nloss_0_0 )))))) (= nloss_0_1 nloss_0_0 )))))))))))))

)


