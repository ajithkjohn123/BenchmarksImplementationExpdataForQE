(benchmark test
:extrafuns ((COUNT_0 BitVec[4]))
:extrafuns ((COUNT_1 BitVec[4]))
:extrafuns ((FLG_0 BitVec[1]))
:extrafuns ((FLG_1 BitVec[1]))

:exists( COUNT_0 COUNT_1 FLG_0 FLG_1)

:formula (and (and (and (and (= (bvadd (bvmul bv1[1]  FLG_0 ) (bvmul bv1[1]  FLG_1 ) ) bv0[1]  ) (bvule COUNT_0 bv7[4]  ) ) (= (bvmul bv1[1]  FLG_0 ) bv1[1]  ) ) (not (= (bvmul bv1[4]  COUNT_0 ) bv5[4]  )) ) (= (bvadd (bvmul bv15[4]  COUNT_0 ) (bvmul bv1[4]  COUNT_1 ) ) bv1[4]  ) )

)
