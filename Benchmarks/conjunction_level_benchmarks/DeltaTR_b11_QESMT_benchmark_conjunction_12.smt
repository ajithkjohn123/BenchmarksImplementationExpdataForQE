(benchmark test
:extrafuns ((cont1_1 BitVec[8]))
:extrafuns ((cont_1 BitVec[8]))
:extrafuns ((cont_2 BitVec[8]))
:extrafuns ((r_in_1 BitVec[8]))
:extrafuns ((r_in_2 BitVec[8]))
:extrafuns ((stato_1 BitVec[4]))
:extrafuns ((stato_2 BitVec[4]))

:exists( cont1_1 cont_1 cont_2 r_in_1 r_in_2 stato_1 stato_2)

:formula (and (and (and (and (and (and (and (and (= (bvmul bv1[8]  cont_1 ) bv0[8]  ) (bvuge cont_2 bv38[8]  ) ) (= (bvmul bv1[4]  stato_2 ) bv2[4]  ) ) (not (= (bvmul bv1[4]  stato_2 ) bv1[4]  )) ) (= (bvadd (bvmul bv1[8]  cont1_1 ) (bvmul bv255[8]  r_in_2 ) ) bv0[8]  ) ) (= (bvmul bv1[8]  r_in_2 ) bv0[8]  ) ) (= (bvmul bv1[4]  stato_1 ) bv8[4]  ) ) (= (bvadd (bvmul bv1[8]  r_in_1 ) (bvmul bv255[8]  r_in_2 ) ) bv0[8]  ) ) (not (= (bvmul bv1[4]  stato_2 ) bv0[4]  )) )

)
