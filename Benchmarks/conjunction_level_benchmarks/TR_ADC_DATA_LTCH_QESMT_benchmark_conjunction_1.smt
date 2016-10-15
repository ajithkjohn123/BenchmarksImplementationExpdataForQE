(benchmark test
:extrafuns ((COUNT_1 BitVec[4]))
:extrafuns ((FLG_0 BitVec[1]))
:extrafuns ((FLG_1 BitVec[1]))

:exists( COUNT_1 FLG_0 FLG_1)

:formula (and (= (bvadd (bvmul bv1[1]  FLG_0 ) (bvmul bv1[1]  FLG_1 ) ) bv0[1]  ) (= (bvmul bv1[4]  COUNT_1 ) bv0[4]  ) )

)
