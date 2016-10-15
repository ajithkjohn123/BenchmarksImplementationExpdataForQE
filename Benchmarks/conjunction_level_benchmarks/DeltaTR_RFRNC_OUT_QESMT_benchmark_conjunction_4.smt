(benchmark test
:extrafuns ((CRRNT_PWR_1 BitVec[16]))
:extrafuns ((CRRNT_PWR_2 BitVec[16]))

:exists( CRRNT_PWR_1 CRRNT_PWR_2)

:formula (and (and (bvule bv4704[16]  CRRNT_PWR_1 ) (bvule CRRNT_PWR_1 bv9504[16]  ) ) (not (= (bvadd (bvmul bv1[16]  CRRNT_PWR_1 ) (bvmul bv65535[16]  CRRNT_PWR_2 ) ) bv0[16]  )) )

)
