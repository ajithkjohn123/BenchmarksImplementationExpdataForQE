(benchmark test
:extrafuns ((CLOCK_0_0 BitVec[1]))
:extrafuns ((CLOCK_0_1 BitVec[1]))
:extrafuns ((autoname_1 BitVec[1]))
:extrafuns ((autoname_2 BitVec[1]))
:extrafuns ((autoname_3 BitVec[1]))
:extrafuns ((autoname_4 BitVec[1]))
:extrafuns ((count_0_0 BitVec[4]))
:extrafuns ((count_0_1 BitVec[4]))
:extrafuns ((dual_0_0 BitVec[1]))
:extrafuns ((dual_0_1 BitVec[1]))
:extrafuns ((gamma_0_0 BitVec[8]))
:extrafuns ((gamma_0_1 BitVec[8]))
:extrafuns ((k_0_1 BitVec[4]))
:extrafuns ((max_0_0 BitVec[4]))
:extrafuns ((max_0_1 BitVec[4]))
:extrafuns ((nloss_0_0 BitVec[1]))
:extrafuns ((nloss_0_1 BitVec[1]))
:extrafuns ((reset_0_0 BitVec[1]))
:extrafuns ((reset_0_1 BitVec[1]))
:extrafuns ((scan_0_0 BitVec[4]))
:extrafuns ((scan_0_1 BitVec[4]))
:extrafuns ((timebase_0_0 BitVec[4]))
:extrafuns ((timebase_0_1 BitVec[4]))

:exists ( count_0_0  dual_0_0  gamma_0_0 )

:formula (if_then_else (or (not (= CLOCK_0_1 CLOCK_0_0 )) (not (= reset_0_1 reset_0_0 )) ) (if_then_else (= reset_0_1 bv1[1] ) (= nloss_0_1 bv0[1] ) (if_then_else (and (not (= CLOCK_0_1 CLOCK_0_0 )) (= CLOCK_0_1 bv1[1] ) ) (if_then_else (= gamma_0_0 bv0[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv1[8] ) (if_then_else (= dual_0_0 bv1[1] ) (= nloss_0_1 bv0[1] ) (= nloss_0_1 nloss_0_0 )) (if_then_else (= gamma_0_0 bv2[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv3[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv4[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv5[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv6[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv7[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv8[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv9[8] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv16[8] ) (if_then_else (bvuge count_0_0 bv7[4] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_1 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_2 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_3 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_4 bv1[1] ) (= nloss_0_1 bv1[1] ) (= nloss_0_1 nloss_0_0 )))))) (= nloss_0_1 nloss_0_0 )))))))))))) (= nloss_0_1 nloss_0_0 ))) (= nloss_0_1 nloss_0_0 ))

)


