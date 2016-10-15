(benchmark test
:extrafuns ((CP_MINUS_RP_0 BitVec[16]))
:extrafuns ((CP_SUB_RP_1 BitVec[16]))
:extrafuns ((CP_SUB_RP_2 BitVec[16]))
:extrafuns ((CP_SUB_RP_3 BitVec[16]))
:extrafuns ((CRRNT_PWR_2 BitVec[16]))
:extrafuns ((RFRNC_PWR_2 BitVec[16]))
:extrafuns ((TIMER_1 BitVec[16]))
:extrafuns ((TIMER_2 BitVec[16]))

:exists( CP_SUB_RP_1 CP_SUB_RP_2 TIMER_1 TIMER_2)

:formula (and (and (and (and (= (bvadd (bvmul bv1[16]  CP_SUB_RP_1 ) (bvmul bv65535[16]  CP_SUB_RP_2 ) ) bv0[16]  ) (= (bvadd (bvadd (bvmul bv1[16]  CP_SUB_RP_1 ) (bvmul bv65535[16]  CRRNT_PWR_2 ) ) (bvmul bv1[16]  RFRNC_PWR_2 ) ) bv0[16]  ) ) (= (bvadd (bvmul bv1[16]  CP_SUB_RP_2 ) (bvmul bv65535[16]  CP_SUB_RP_3 ) ) bv0[16]  ) ) (= (bvadd (bvmul bv1[16]  TIMER_1 ) (bvmul bv65535[16]  TIMER_2 ) ) bv0[16]  ) ) (= (bvadd (bvmul bv1[16]  CP_MINUS_RP_0 ) (bvmul bv65535[16]  CP_SUB_RP_1 ) ) bv0[16]  ) )

)
