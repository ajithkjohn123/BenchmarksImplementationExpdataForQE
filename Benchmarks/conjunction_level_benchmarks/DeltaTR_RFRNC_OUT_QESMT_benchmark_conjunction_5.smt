(benchmark test
:extrafuns ((CRRNT_PWR_1 BitVec[16]))
:extrafuns ((CRRNT_PWR_2 BitVec[16]))

:exists( CRRNT_PWR_1 CRRNT_PWR_2)

:formula (and (not (= (bvadd (bvmul bv1[16]  CRRNT_PWR_1 ) (bvmul bv65535[16]  CRRNT_PWR_2 ) ) bv0[16]  )) (bvuge bv4703[16]  CRRNT_PWR_1 ) )

)
