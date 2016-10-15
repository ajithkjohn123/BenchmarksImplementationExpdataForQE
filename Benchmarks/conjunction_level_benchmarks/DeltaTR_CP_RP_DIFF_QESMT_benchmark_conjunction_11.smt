(benchmark test
:extrafuns ((CP_MINUS_RP_0 BitVec[16]))
:extrafuns ((CP_SUB_RP_1 BitVec[16]))
:extrafuns ((CP_SUB_RP_2 BitVec[16]))
:extrafuns ((CRRNT_PWR_2 BitVec[16]))
:extrafuns ((RFRNC_PWR_2 BitVec[16]))
:extrafuns ((TIMER_1 BitVec[16]))

:exists( CP_SUB_RP_1 CP_SUB_RP_2 TIMER_1)

:formula (and (and (and (and (and (and (bvuge CP_SUB_RP_1 bv1536[16]  ) (bvule CP_SUB_RP_1 bv32767[16]  ) ) (= (bvadd (bvadd (bvmul bv1[16]  CP_SUB_RP_1 ) (bvmul bv65535[16]  CRRNT_PWR_2 ) ) (bvmul bv1[16]  RFRNC_PWR_2 ) ) bv0[16]  ) ) (= (bvadd (bvmul bv1[16]  CP_MINUS_RP_0 ) (bvmul bv65535[16]  CP_SUB_RP_1 ) ) bv0[16]  ) ) (= (bvmul bv1[16]  TIMER_1 ) bv0[16]  ) ) (bvuge bv1535[16]  CP_SUB_RP_2 ) ) (not (= (bvmul bv1[16]  TIMER_1 ) bv65535[16]  )) )

)
