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
:extrafuns ((start_0_1 BitVec[1]))
:extrafuns ((timebase_0_0 BitVec[32]))
:extrafuns ((timebase_0_1 BitVec[32]))

:exists ( count_0_0  max_0_0  scan_0_0 )

:formula (and (if_then_else (= reset_0_1 bv1[1] ) (= gamma_0_1 bv0[32] ) (if_then_else (= gamma_0_0 bv0[32] ) (= gamma_0_1 bv1[32] ) (if_then_else (= gamma_0_0 bv1[32] ) (= gamma_0_1 bv2[32] ) (if_then_else (= gamma_0_0 bv2[32] ) (= gamma_0_1 bv3[32] ) (if_then_else (= gamma_0_0 bv3[32] ) (= gamma_0_1 bv4[32] ) (if_then_else (= gamma_0_0 bv4[32] ) (= gamma_0_1 bv5[32] ) (if_then_else (= gamma_0_0 bv5[32] ) (= gamma_0_1 bv6[32] ) (if_then_else (= gamma_0_0 bv6[32] ) (if_then_else (= count_0_0 bv0[32] ) (= gamma_0_1 bv7[32] ) (= gamma_0_1 bv6[32] )) (if_then_else (= gamma_0_0 bv7[32] ) (if_then_else (= count_0_0 bv0[32] ) (if_then_else (= scan_0_0 max_0_0 ) (= gamma_0_1 bv3[32] ) (= gamma_0_1 bv8[32] )) (= gamma_0_1 bv7[32] )) (if_then_else (= gamma_0_0 bv8[32] ) (= gamma_0_1 bv9[32] ) (if_then_else (= gamma_0_0 bv9[32] ) (= gamma_0_1 bv1[32] ) (if_then_else (= gamma_0_0 bv16[32] ) (if_then_else (= count_0_0 bv0[32] ) (= gamma_0_1 bv23[32] ) (if_then_else (= autoname_1 bv1[1] ) (= gamma_0_1 bv32[32] ) (if_then_else (= autoname_2 bv1[1] ) (= gamma_0_1 bv32[32] ) (if_then_else (= autoname_3 bv1[1] ) (= gamma_0_1 bv32[32] ) (if_then_else (= autoname_4 bv1[1] ) (= gamma_0_1 bv32[32] ) (= gamma_0_1 bv16[32] )))))) (if_then_else (= gamma_0_0 bv17[32] ) (= gamma_0_1 bv18[32] ) (if_then_else (= gamma_0_0 bv18[32] ) (if_then_else (= count_0_0 bv0[32] ) (= gamma_0_1 bv19[32] ) (= gamma_0_1 bv18[32] )) (if_then_else (= gamma_0_0 bv19[32] ) (if_then_else (= count_0_0 bv0[32] ) (if_then_else (= scan_0_0 max_0_0 ) (= gamma_0_1 bv8[32] ) (if_then_else (= max_0_0 bv0[32] ) (= gamma_0_1 bv2[32] ) (= gamma_0_1 bv36[32] ))) (= gamma_0_1 bv19[32] )) (if_then_else (= gamma_0_0 bv20[32] ) (= gamma_0_1 bv21[32] ) (if_then_else (= gamma_0_0 bv21[32] ) (if_then_else (= count_0_0 bv0[32] ) (= gamma_0_1 bv22[32] ) (= gamma_0_1 bv21[32] )) (if_then_else (= gamma_0_0 bv22[32] ) (if_then_else (= count_0_0 bv0[32] ) (= gamma_0_1 bv23[32] ) (= gamma_0_1 bv22[32] )) (if_then_else (= gamma_0_0 bv23[32] ) (= gamma_0_1 bv24[32] ) (if_then_else (= gamma_0_0 bv24[32] ) (= gamma_0_1 bv25[32] ) (if_then_else (= gamma_0_0 bv25[32] ) (= gamma_0_1 bv32[32] ) (if_then_else (= gamma_0_0 bv32[32] ) (if_then_else (= count_0_0 bv0[32] ) (= gamma_0_1 bv33[32] ) (= gamma_0_1 bv32[32] )) (if_then_else (= gamma_0_0 bv33[32] ) (if_then_else (= count_0_0 bv0[32] ) (if_then_else (= max_0_0 scan_0_0 ) (= gamma_0_1 bv23[32] ) (= gamma_0_1 bv34[32] )) (= gamma_0_1 bv33[32] )) (if_then_else (= gamma_0_0 bv34[32] ) (if_then_else (= count_0_0 bv0[32] ) (= gamma_0_1 bv35[32] ) (= gamma_0_1 bv34[32] )) (if_then_else (= gamma_0_0 bv35[32] ) (if_then_else (= count_0_0 bv0[32] ) (= gamma_0_1 bv34[32] ) (= gamma_0_1 bv35[32] )) (if_then_else (= gamma_0_0 bv36[32] ) (if_then_else (= count_0_0 bv0[32] ) (= gamma_0_1 bv37[32] ) (= gamma_0_1 bv36[32] )) (if_then_else (= gamma_0_0 bv37[32] ) (if_then_else (= count_0_0 bv0[32] ) (= gamma_0_1 bv36[32] ) (= gamma_0_1 bv37[32] )) (if_then_else (= start_0_1 bv1[1] ) (= gamma_0_1 bv1[32] ) (= gamma_0_1 gamma_0_0 ))))))))))))))))))))))))))))) (if_then_else (= reset_0_1 bv1[1] ) (= nloss_0_1 bv0[1] ) (if_then_else (= gamma_0_0 bv0[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv1[32] ) (= nloss_0_1 bv0[1] ) (if_then_else (= gamma_0_0 bv2[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv3[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv4[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv5[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv6[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv7[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv8[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv9[32] ) (= nloss_0_1 nloss_0_0 ) (if_then_else (= gamma_0_0 bv16[32] ) (if_then_else (= count_0_0 bv0[32] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_1 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_2 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_3 bv1[1] ) (= nloss_0_1 bv1[1] ) (if_then_else (= autoname_4 bv1[1] ) (= nloss_0_1 bv1[1] ) (= nloss_0_1 nloss_0_0 )))))) (= nloss_0_1 nloss_0_0 ))))))))))))) )

)


