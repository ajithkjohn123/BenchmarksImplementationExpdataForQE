(benchmark test
:extrafuns ((X_1 BitVec[16]))
:extrafuns ((Y_1 BitVec[16]))
:extrafuns ((xvar_1 BitVec[16]))
:extrafuns ((yvar_1 BitVec[16]))

:exists( xvar_1 yvar_1)

:formula (and (= (bvadd (bvmul bv65535[16]  X_1 ) (bvmul bv1[16]  xvar_1 ) ) bv0[16]  ) (= (bvadd (bvmul bv65535[16]  Y_1 ) (bvmul bv1[16]  yvar_1 ) ) bv0[16]  ) )

)
