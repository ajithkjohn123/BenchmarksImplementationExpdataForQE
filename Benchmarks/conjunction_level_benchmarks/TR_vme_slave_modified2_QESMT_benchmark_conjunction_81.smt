(benchmark test
:extrafuns ((count_0 BitVec[16]))
:extrafuns ((count_1 BitVec[16]))
:extrafuns ((cs_1 BitVec[1]))
:extrafuns ((ds0_0 BitVec[1]))
:extrafuns ((ds0_1 BitVec[1]))
:extrafuns ((ds1_0 BitVec[1]))
:extrafuns ((ds1_1 BitVec[1]))
:extrafuns ((ds_lsb_0 BitVec[1]))
:extrafuns ((ds_lsb_1 BitVec[1]))
:extrafuns ((ds_msb_0 BitVec[1]))
:extrafuns ((ds_msb_1 BitVec[1]))
:extrafuns ((latched_addr_15_9_0 BitVec[7]))
:extrafuns ((latched_addr_15_9_1 BitVec[7]))
:extrafuns ((s_cs_0 BitVec[1]))
:extrafuns ((s_cs_1 BitVec[1]))
:extrafuns ((state_0 BitVec[4]))
:extrafuns ((state_1 BitVec[4]))

:exists( count_0 count_1 ds_lsb_0 ds_lsb_1 ds_msb_0 ds_msb_1 latched_addr_15_9_0 latched_addr_15_9_1 s_cs_0 s_cs_1 state_0 state_1)

:formula (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (= (bvadd (bvmul bv1[1]  s_cs_0 ) (bvmul bv1[1]  s_cs_1 ) ) bv0[1]  ) (= (bvadd (bvmul bv1[1]  ds1_0 ) (bvmul bv1[1]  ds_lsb_0 ) ) bv0[1]  ) ) (= (bvadd (bvmul bv127[7]  latched_addr_15_9_0 ) (bvmul bv1[7]  latched_addr_15_9_1 ) ) bv0[7]  ) ) (= (bvadd (bvmul bv1[1]  ds1_1 ) (bvmul bv1[1]  ds_lsb_1 ) ) bv0[1]  ) ) (= (bvadd (bvmul bv1[1]  ds0_0 ) (bvmul bv1[1]  ds_msb_0 ) ) bv0[1]  ) ) (= (bvadd (bvmul bv1[1]  ds0_1 ) (bvmul bv1[1]  ds_msb_1 ) ) bv0[1]  ) ) (= (bvadd (bvmul bv1[1]  cs_1 ) (bvmul bv1[1]  s_cs_1 ) ) bv0[1]  ) ) (not (= (bvmul bv1[4]  state_0 ) bv8[4]  )) ) (not (= (bvmul bv1[4]  state_0 ) bv6[4]  )) ) (= (bvmul bv1[4]  state_1 ) bv0[4]  ) ) (not (= (bvmul bv1[4]  state_0 ) bv2[4]  )) ) (not (= (bvmul bv1[4]  state_0 ) bv7[4]  )) ) (not (= (bvmul bv1[4]  state_0 ) bv10[4]  )) ) (not (= (bvmul bv1[4]  state_0 ) bv0[4]  )) ) (not (= (bvmul bv1[4]  state_0 ) bv1[4]  )) ) (not (= (bvmul bv1[4]  state_0 ) bv3[4]  )) ) (not (= (bvmul bv1[4]  state_0 ) bv4[4]  )) ) (not (= (bvmul bv1[4]  state_0 ) bv9[4]  )) ) (= (bvmul bv1[16]  count_1 ) bv0[16]  ) ) (not (= (bvmul bv1[4]  state_0 ) bv5[4]  )) ) (= (bvmul bv1[4]  state_0 ) bv11[4]  ) ) (= (bvadd (bvmul bv65535[16]  count_0 ) (bvmul bv1[16]  count_1 ) ) bv1[16]  ) )

)
