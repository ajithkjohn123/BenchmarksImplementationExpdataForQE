(benchmark test
:extrafuns ((A_1 BitVec[16]))
:extrafuns ((A_2 BitVec[16]))
:extrafuns ((COUNT_1 BitVec[8]))
:extrafuns ((COUNT_2 BitVec[8]))
:extrafuns ((COUNT_3 BitVec[8]))
:extrafuns ((FDBCKSUM_0 BitVec[16]))
:extrafuns ((SUM128L_0 BitVec[16]))

:exists( A_1 A_2 COUNT_1 COUNT_2 COUNT_3)

:formula (and (and (and (and (and (and (bvule COUNT_3 bv128[8]  ) (= (bvadd (bvmul bv1[16]  A_1 ) (bvmul bv65535[16]  A_2 ) ) bv0[16]  ) ) (= (bvadd (bvmul bv1[8]  COUNT_1 ) (bvmul bv255[8]  COUNT_2 ) ) bv0[8]  ) ) (= (bvadd (bvmul bv65535[16]  A_1 ) (bvmul bv1[16]  FDBCKSUM_0 ) ) bv0[16]  ) ) (= (bvadd (bvmul bv1[8]  COUNT_2 ) (bvmul bv255[8]  COUNT_3 ) ) bv1[8]  ) ) (bvuge COUNT_1 bv129[8]  ) ) (= (bvadd (bvmul bv65535[16]  A_1 ) (bvmul bv1[16]  SUM128L_0 ) ) bv0[16]  ) )

)
