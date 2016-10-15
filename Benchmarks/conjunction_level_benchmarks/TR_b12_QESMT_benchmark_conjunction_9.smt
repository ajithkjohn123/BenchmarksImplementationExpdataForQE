(benchmark test
:extrafuns ((count_1 BitVec[4]))
:extrafuns ((counter_0 BitVec[4]))
:extrafuns ((counter_1 BitVec[4]))
:extrafuns ((gamma_0 BitVec[8]))
:extrafuns ((gamma_1 BitVec[8]))
:extrafuns ((max_0 BitVec[4]))
:extrafuns ((max_1 BitVec[4]))
:extrafuns ((play_0 BitVec[1]))
:extrafuns ((play_1 BitVec[1]))
:extrafuns ((s_0 BitVec[1]))
:extrafuns ((s_1 BitVec[1]))
:extrafuns ((scan_0 BitVec[4]))
:extrafuns ((scan_1 BitVec[4]))
:extrafuns ((sound_0 BitVec[8]))
:extrafuns ((sound_1 BitVec[8]))
:extrafuns ((timebase_0 BitVec[4]))
:extrafuns ((timebase_1 BitVec[4]))

:exists( count_1 counter_0 counter_1 gamma_0 gamma_1 max_0 max_1 play_0 play_1 s_0 s_1 scan_0 scan_1 sound_0 sound_1 timebase_0 timebase_1)

:formula (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (bvule counter_0 bv5[4]  ) (= (bvadd (bvmul bv15[4]  max_0 ) (bvmul bv1[4]  max_1 ) ) bv0[4]  ) ) (= (bvmul bv1[8]  sound_1 ) bv0[8]  ) ) (not (= (bvmul bv1[8]  gamma_0 ) bv0[8]  )) ) (= (bvadd (bvmul bv1[1]  s_0 ) (bvmul bv1[1]  s_1 ) ) bv0[1]  ) ) (= (bvadd (bvmul bv15[4]  timebase_0 ) (bvmul bv1[4]  timebase_1 ) ) bv0[4]  ) ) (= (bvadd (bvmul bv15[4]  scan_0 ) (bvmul bv1[4]  scan_1 ) ) bv0[4]  ) ) (not (= (bvmul bv1[8]  gamma_0 ) bv2[8]  )) ) (not (= (bvmul bv1[8]  gamma_0 ) bv1[8]  )) ) (not (= (bvmul bv1[8]  sound_0 ) bv2[8]  )) ) (= (bvadd (bvmul bv1[4]  count_1 ) (bvmul bv15[4]  timebase_0 ) ) bv0[4]  ) ) (= (bvmul bv1[8]  gamma_0 ) bv5[8]  ) ) (= (bvmul bv1[1]  play_0 ) bv1[1]  ) ) (not (= (bvmul bv1[8]  gamma_0 ) bv3[8]  )) ) (not (= (bvmul bv1[8]  sound_0 ) bv1[8]  )) ) (not (= (bvmul bv1[8]  gamma_0 ) bv4[8]  )) ) (= (bvmul bv1[1]  play_1 ) bv1[1]  ) ) (not (= (bvmul bv1[8]  sound_0 ) bv0[8]  )) ) (= (bvadd (bvmul bv15[4]  counter_0 ) (bvmul bv1[4]  counter_1 ) ) bv1[4]  ) ) (not (= (bvmul bv1[8]  sound_0 ) bv4[8]  )) ) (= (bvmul bv1[8]  gamma_1 ) bv6[8]  ) ) (not (= (bvmul bv1[8]  sound_0 ) bv3[8]  )) ) (= (bvmul bv1[8]  sound_0 ) bv5[8]  ) )

)
