(benchmark test
:extrafuns ((autoname_1 BitVec[1]))
:extrafuns ((autoname_2 BitVec[1]))
:extrafuns ((autoname_3 BitVec[5]))
:extrafuns ((autoname_4 BitVec[1]))
:extrafuns ((autoname_5 BitVec[1]))
:extrafuns ((b_addr_0_1 BitVec[8]))
:extrafuns ((b_ds_0_1 BitVec[2]))
:extrafuns ((jumper_0_1 BitVec[5]))
:extrafuns ((local_access__comp_check_cycle__as_state_0_0 BitVec[8]))
:extrafuns ((local_access__comp_check_cycle__as_state_0_1 BitVec[8]))
:extrafuns ((local_access__comp_check_cycle__control_word2_0_0 BitVec[32]))
:extrafuns ((local_access__comp_check_cycle__control_word2_0_1 BitVec[32]))
:extrafuns ((local_access__comp_check_cycle__control_word_0_0 BitVec[32]))
:extrafuns ((local_access__comp_check_cycle__control_word_0_1 BitVec[32]))
:extrafuns ((reset_0_1 BitVec[1]))
:extrafuns ((vme_iack_in_0_1 BitVec[1]))

:exists ( local_access__comp_check_cycle__as_state_0_0  local_access__comp_check_cycle__control_word2_0_0  local_access__comp_check_cycle__control_word_0_0  local_access__comp_check_cycle__control_word_0_1 )

:formula (if_then_else (= reset_0_1 bv0[1] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv0[8] ) (if_then_else (or (and (= autoname_1 bv0[1] ) (= autoname_2 bv0[1] ) ) (= vme_iack_in_0_1 bv0[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv16[32] )) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv1[8] ) (if_then_else (and (= vme_iack_in_0_1 bv0[1] ) (= autoname_1 bv0[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (and (and (= autoname_1 bv0[1] ) (= autoname_2 bv0[1] ) ) (= vme_iack_in_0_1 bv1[1] ) ) (if_then_else (= jumper_0_1 autoname_3 ) (if_then_else (and (= autoname_4 bv0[1] ) (= autoname_5 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 )) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 )) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv2[8] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv3[8] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv4[8] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv5[8] ) (if_then_else (and (= autoname_1 bv1[1] ) (= autoname_2 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv16[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 )) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv6[8] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv7[8] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv8[8] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv9[8] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv16[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (and (= local_access__comp_check_cycle__control_word_0_0 bv0[32] ) (= local_access__comp_check_cycle__control_word2_0_0 bv0[32] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv17[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= local_access__comp_check_cycle__control_word_0_0 (bvmul bv2[32] local_access__comp_check_cycle__control_word2_0_0 ) ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv1[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv18[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= local_access__comp_check_cycle__control_word_0_0 (bvadd (bvmul bv2[32] local_access__comp_check_cycle__control_word2_0_0 ) bv16[32] ) ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (and (= local_access__comp_check_cycle__control_word_0_0 bv2[32] ) (= local_access__comp_check_cycle__control_word2_0_0 bv1[32] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv19[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (and (= local_access__comp_check_cycle__control_word_0_0 bv0[32] ) (= local_access__comp_check_cycle__control_word2_0_0 bv16[32] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv20[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv3[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv4[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv21[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv7[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv6[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv22[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv8[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv23[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv0[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv9[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv24[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv9[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv25[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv0[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv32[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv0[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv33[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv0[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv34[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv0[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv35[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv0[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv36[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv0[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (if_then_else (= local_access__comp_check_cycle__as_state_0_0 bv37[8] ) (if_then_else (or (= vme_iack_in_0_1 bv1[1] ) (= autoname_1 bv1[1] ) ) (= local_access__comp_check_cycle__control_word_0_1 bv1[32] ) (if_then_else (= local_access__comp_check_cycle__control_word_0_0 bv0[32] ) (= local_access__comp_check_cycle__control_word_0_1 bv2[32] ) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))) (= local_access__comp_check_cycle__control_word_0_1 local_access__comp_check_cycle__control_word_0_0 ))))))))))))))))))))))))))))

)


