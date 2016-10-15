(benchmark test
:extrafuns ((A_1 BitVec[16]))
:extrafuns ((COUNT_0 BitVec[8]))
:extrafuns ((COUNT_1 BitVec[8]))
:extrafuns ((FDBCKSUM_1 BitVec[16]))
:extrafuns ((SUM128UL_1 BitVec[16]))

:exists( A_1 COUNT_0 COUNT_1)

:formula (and (and (and (= (bvadd (bvmul bv1[16]  A_1 ) (bvmul bv65535[16]  SUM128UL_1 ) ) bv0[16]  ) (= (bvadd (bvmul bv255[8]  COUNT_0 ) (bvmul bv1[8]  COUNT_1 ) ) bv1[8]  ) ) (= (bvadd (bvmul bv65535[16]  A_1 ) (bvmul bv1[16]  FDBCKSUM_1 ) ) bv0[16]  ) ) (bvule COUNT_0 bv128[8]  ) )

)
