(benchmark tr
:extrafuns ((CLK_1 BitVec[1]))
:extrafuns ((CLK_2 BitVec[1]))
:extrafuns ((CLK_3 BitVec[1]))
:extrafuns ((CP_GRT_RP_1 BitVec[1]))
:extrafuns ((CP_GRT_RP_0 BitVec[1]))
:extrafuns ((CP_MINUS_RP_0 BitVec[16]))
:extrafuns ((CP_SUB_RP_1 BitVec[16]))
:extrafuns ((CP_SUB_RP_2 BitVec[16]))
:extrafuns ((CP_SUB_RP_3 BitVec[16]))
:extrafuns ((CRRNT_PWR_2 BitVec[16]))
:extrafuns ((RESET_1 BitVec[1]))
:extrafuns ((RESET_2 BitVec[1]))
:extrafuns ((RESET_3 BitVec[1]))
:extrafuns ((RFRNC_PWR_2 BitVec[16]))
:extrafuns ((RP_GRT_CP_1 BitVec[1]))
:extrafuns ((RP_GRT_CP_0 BitVec[1]))
:extrafuns ((TIMER_1 BitVec[16]))
:extrafuns ((TIMER_2 BitVec[16]))

:exists(TIMER_1 TIMER_2 CP_SUB_RP_1 CP_SUB_RP_2)


:formula (and (and (and (and (if_then_else (or (or (not (= CP_SUB_RP_1 CP_SUB_RP_2 )) (not (= RESET_1 RESET_2 )) ) (not (= CLK_1 CLK_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (= CP_GRT_RP_0 bv0[1] ) (if_then_else (bvult CP_SUB_RP_1 bv1536[16] ) (= CP_GRT_RP_0 bv0[1] ) (if_then_else (and (not (= CLK_1 CLK_2 )) (= CLK_1 bv1[1] ) ) (if_then_else (bvuge CP_SUB_RP_1 bv1536[16] ) (if_then_else (bvule CP_SUB_RP_1 bv32767[16] ) (if_then_else (= TIMER_1 bv65535[16] ) (= CP_GRT_RP_0 bv1[1] ) (= CP_GRT_RP_0 CP_GRT_RP_1 )) (if_then_else (bvugt CP_SUB_RP_1 bv32767[16] ) (if_then_else (= TIMER_1 bv65535[16] ) (= CP_GRT_RP_0 bv0[1] ) (= CP_GRT_RP_0 CP_GRT_RP_1 )) (= CP_GRT_RP_0 CP_GRT_RP_1 ))) (= CP_GRT_RP_0 CP_GRT_RP_1 )) (= CP_GRT_RP_0 CP_GRT_RP_1 )))) (= CP_GRT_RP_0 CP_GRT_RP_1 )) (= CP_SUB_RP_1 (bvadd CRRNT_PWR_2 (bvmul bv65535[16] RFRNC_PWR_2 ) ) ) ) (if_then_else (or (or (not (= CP_SUB_RP_2 CP_SUB_RP_3 )) (not (= RESET_2 RESET_3 )) ) (not (= CLK_2 CLK_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (= TIMER_1 bv0[16] ) (if_then_else (bvult CP_SUB_RP_2 bv1536[16] ) (= TIMER_1 bv0[16] ) (if_then_else (and (not (= CLK_2 CLK_3 )) (= CLK_2 bv1[1] ) ) (if_then_else (bvuge CP_SUB_RP_2 bv1536[16] ) (if_then_else (bvule CP_SUB_RP_2 bv32767[16] ) (if_then_else (= TIMER_2 bv65535[16] ) (= TIMER_1 TIMER_2 ) (= TIMER_1 (bvadd TIMER_2 bv1[1] ) )) (if_then_else (bvugt CP_SUB_RP_2 bv32767[16] ) (if_then_else (= TIMER_2 bv65535[16] ) (= TIMER_1 TIMER_2 ) (= TIMER_1 (bvadd TIMER_2 bv1[1] ) )) (= TIMER_1 TIMER_2 ))) (= TIMER_1 TIMER_2 )) (= TIMER_1 TIMER_2 )))) (= TIMER_1 TIMER_2 )) ) (and (= CP_MINUS_RP_0 CP_SUB_RP_1 ) (= CP_SUB_RP_1 (bvadd CRRNT_PWR_2 (bvmul bv65535[16] RFRNC_PWR_2 ) ) ) ) ) (and (and (if_then_else (or (or (not (= CP_SUB_RP_1 CP_SUB_RP_2 )) (not (= RESET_1 RESET_2 )) ) (not (= CLK_1 CLK_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (= RP_GRT_CP_0 bv0[1] ) (if_then_else (bvult CP_SUB_RP_1 bv1536[16] ) (= RP_GRT_CP_0 bv0[1] ) (if_then_else (and (not (= CLK_1 CLK_2 )) (= CLK_1 bv1[1] ) ) (if_then_else (bvuge CP_SUB_RP_1 bv1536[16] ) (if_then_else (bvule CP_SUB_RP_1 bv32767[16] ) (if_then_else (= TIMER_1 bv65535[16] ) (= RP_GRT_CP_0 bv0[1] ) (= RP_GRT_CP_0 RP_GRT_CP_1 )) (if_then_else (bvugt CP_SUB_RP_1 bv32767[16] ) (if_then_else (= TIMER_1 bv65535[16] ) (= RP_GRT_CP_0 bv1[1] ) (= RP_GRT_CP_0 RP_GRT_CP_1 )) (= RP_GRT_CP_0 RP_GRT_CP_1 ))) (= RP_GRT_CP_0 RP_GRT_CP_1 )) (= RP_GRT_CP_0 RP_GRT_CP_1 )))) (= RP_GRT_CP_0 RP_GRT_CP_1 )) (= CP_SUB_RP_1 (bvadd CRRNT_PWR_2 (bvmul bv65535[16] RFRNC_PWR_2 ) ) ) ) (if_then_else (or (or (not (= CP_SUB_RP_2 CP_SUB_RP_3 )) (not (= RESET_2 RESET_3 )) ) (not (= CLK_2 CLK_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (= TIMER_1 bv0[16] ) (if_then_else (bvult CP_SUB_RP_2 bv1536[16] ) (= TIMER_1 bv0[16] ) (if_then_else (and (not (= CLK_2 CLK_3 )) (= CLK_2 bv1[1] ) ) (if_then_else (bvuge CP_SUB_RP_2 bv1536[16] ) (if_then_else (bvule CP_SUB_RP_2 bv32767[16] ) (if_then_else (= TIMER_2 bv65535[16] ) (= TIMER_1 TIMER_2 ) (= TIMER_1 (bvadd TIMER_2 bv1[1] ) )) (if_then_else (bvugt CP_SUB_RP_2 bv32767[16] ) (if_then_else (= TIMER_2 bv65535[16] ) (= TIMER_1 TIMER_2 ) (= TIMER_1 (bvadd TIMER_2 bv1[1] ) )) (= TIMER_1 TIMER_2 ))) (= TIMER_1 TIMER_2 )) (= TIMER_1 TIMER_2 )))) (= TIMER_1 TIMER_2 )) ) )

)


