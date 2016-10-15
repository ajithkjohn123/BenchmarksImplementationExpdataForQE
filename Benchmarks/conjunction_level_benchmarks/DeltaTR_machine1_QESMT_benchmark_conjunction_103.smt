(benchmark test
:extrafuns ((count_1 BitVec[32]))
:extrafuns ((count_2 BitVec[32]))
:extrafuns ((gamma_1 BitVec[32]))
:extrafuns ((gamma_2 BitVec[32]))
:extrafuns ((ind_1 BitVec[32]))
:extrafuns ((ind_2 BitVec[32]))
:extrafuns ((max_1 BitVec[32]))
:extrafuns ((max_2 BitVec[32]))
:extrafuns ((scan_1 BitVec[32]))
:extrafuns ((scan_2 BitVec[32]))
:extrafuns ((sound_2 BitVec[32]))
:extrafuns ((sound_3 BitVec[32]))
:extrafuns ((timebase_1 BitVec[32]))
:extrafuns ((timebase_2 BitVec[32]))

:exists( count_1 count_2 gamma_1 gamma_2 ind_1 ind_2 max_1 max_2 scan_1 scan_2 sound_2 sound_3 timebase_1 timebase_2)

:formula (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (= (bvadd (bvmul bv1[32]  sound_2 ) (bvmul bv4294967295[32]  sound_3 ) ) bv0[32]  ) (= (bvadd (bvmul bv1[32]  max_1 ) (bvmul bv4294967295[32]  max_2 ) ) bv0[32]  ) ) (= (bvadd (bvmul bv1[32]  ind_1 ) (bvmul bv4294967295[32]  ind_2 ) ) bv0[32]  ) ) (= (bvadd (bvmul bv1[32]  scan_1 ) (bvmul bv4294967295[32]  scan_2 ) ) bv0[32]  ) ) (= (bvadd (bvmul bv1[32]  gamma_1 ) (bvmul bv4294967295[32]  gamma_2 ) ) bv0[32]  ) ) (= (bvadd (bvmul bv1[32]  count_1 ) (bvmul bv4294967295[32]  count_2 ) ) bv0[32]  ) ) (not (= (bvmul bv1[32]  gamma_1 ) bv0[32]  )) ) (= (bvadd (bvmul bv1[32]  timebase_1 ) (bvmul bv4294967295[32]  timebase_2 ) ) bv0[32]  ) ) (not (= (bvmul bv1[32]  gamma_1 ) bv9[32]  )) ) (not (= (bvmul bv1[32]  gamma_1 ) bv8[32]  )) ) (not (= (bvmul bv1[32]  gamma_1 ) bv1[32]  )) ) (not (= (bvmul bv1[32]  gamma_1 ) bv6[32]  )) ) (bvule count_1 (bvadd (bvmul bv2[32]  max_1 ) (bvmul bv4[32]  scan_1 ) ) ) ) (= (bvmul bv1[32]  gamma_1 ) bv16[32]  ) ) (not (= (bvmul bv1[32]  gamma_1 ) bv5[32]  )) ) (not (= (bvmul bv1[32]  gamma_1 ) bv7[32]  )) ) (not (= (bvmul bv1[32]  gamma_1 ) bv2[32]  )) ) (not (= (bvmul bv1[32]  gamma_1 ) bv4[32]  )) ) (not (= (bvmul bv1[32]  gamma_1 ) bv3[32]  )) )

)
