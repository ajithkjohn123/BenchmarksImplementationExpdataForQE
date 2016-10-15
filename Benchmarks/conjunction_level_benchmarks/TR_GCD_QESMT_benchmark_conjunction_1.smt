(benchmark test
:extrafuns ((xvar_0 BitVec[16]))
:extrafuns ((xvar_1 BitVec[16]))
:extrafuns ((yvar_0 BitVec[16]))
:extrafuns ((yvar_1 BitVec[16]))

:exists( xvar_0 xvar_1 yvar_0 yvar_1)

:formula (and (= (bvadd (bvmul bv65535[16]  xvar_0 ) (bvmul bv1[16]  xvar_1 ) ) bv0[16]  ) (= (bvadd (bvmul bv65535[16]  yvar_0 ) (bvmul bv1[16]  yvar_1 ) ) bv0[16]  ) )

)
