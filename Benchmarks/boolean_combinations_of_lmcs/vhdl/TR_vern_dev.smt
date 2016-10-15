(benchmark tr
:extrafuns ((CPGRTRP_0 BitVec[1]))
:extrafuns ((CPGRTRP_1 BitVec[1]))
:extrafuns ((CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_0 BitVec[16]))
:extrafuns ((CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_1 BitVec[16]))
:extrafuns ((CP_RP_DIFF__CP_DIFF_RP__TIMER_0 BitVec[8]))
:extrafuns ((CP_RP_DIFF__CP_DIFF_RP__TIMER_1 BitVec[8]))
:extrafuns ((CRRNTPWR_0 BitVec[16]))
:extrafuns ((CRRNTPWR_1 BitVec[16]))
:extrafuns ((DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 BitVec[8]))
:extrafuns ((DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 BitVec[8]))
:extrafuns ((RESET_0 BitVec[1]))
:extrafuns ((RESET_1 BitVec[1]))
:extrafuns ((RFRNCPWR_0 BitVec[16]))
:extrafuns ((RFRNCPWR_1 BitVec[16]))
:extrafuns ((RPGRTCP_0 BitVec[1]))
:extrafuns ((RPGRTCP_1 BitVec[1]))
:extrafuns ((TIMER__TIMER_TCK__COUNT_0 BitVec[4]))
:extrafuns ((TIMER__TIMER_TCK__COUNT_1 BitVec[4]))
:extrafuns ((TIMETCK_0 BitVec[1]))
:extrafuns ((TIMETCK_1 BitVec[1]))
:extrafuns ((VERNDACLOAD_0 BitVec[1]))
:extrafuns ((VERNDACLOAD_1 BitVec[1]))
:extrafuns ((VERNDACSEL_0 BitVec[1]))
:extrafuns ((VERNDACSEL_1 BitVec[1]))
:extrafuns ((clk_0 BitVec[1]))
:extrafuns ((clk_1 BitVec[1]))

:exists (TIMETCK_0 TIMETCK_1 CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_0 CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_1 TIMER__TIMER_TCK__COUNT_0 TIMER__TIMER_TCK__COUNT_1 CP_RP_DIFF__CP_DIFF_RP__TIMER_0 CP_RP_DIFF__CP_DIFF_RP__TIMER_1)


:formula (and (and (and (and (and (and (if_then_else (or (or (not (= CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_1 (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) )) (not (= RESET_1 RESET_1 )) ) (not (= clk_1 clk_1 )) ) (if_then_else (= RESET_1 bv1[1] ) (= CPGRTRP_1 bv0[1] ) (if_then_else (bvult CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_1 bv1536[16] ) (= CPGRTRP_1 bv0[1] ) (if_then_else (or (or (not (= (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_0 )) (not (= RESET_1 RESET_0 )) ) (not (= clk_1 clk_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= CPGRTRP_1 bv0[1] ) (if_then_else (bvult (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) bv1536[16] ) (= CPGRTRP_1 bv0[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (bvule (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) bv32767[16] ) (if_then_else (= CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv255[8] ) (= CPGRTRP_1 bv1[1] ) (= CPGRTRP_1 CPGRTRP_0 )) (if_then_else (= CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv255[8] ) (= CPGRTRP_1 bv0[1] ) (= CPGRTRP_1 CPGRTRP_0 ))) (= CPGRTRP_1 CPGRTRP_0 )))) (= CPGRTRP_1 CPGRTRP_0 )))) (if_then_else (or (or (not (= (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_0 )) (not (= RESET_1 RESET_0 )) ) (not (= clk_1 clk_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= CPGRTRP_1 bv0[1] ) (if_then_else (bvult (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) bv1536[16] ) (= CPGRTRP_1 bv0[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (bvule (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) bv32767[16] ) (if_then_else (= CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv255[8] ) (= CPGRTRP_1 bv1[1] ) (= CPGRTRP_1 CPGRTRP_0 )) (if_then_else (= CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv255[8] ) (= CPGRTRP_1 bv0[1] ) (= CPGRTRP_1 CPGRTRP_0 ))) (= CPGRTRP_1 CPGRTRP_0 )))) (= CPGRTRP_1 CPGRTRP_0 ))) (= CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_1 (bvadd (bvmul bv1[16]  CRRNTPWR_1 ) (bvmul bv65535[16]  RFRNCPWR_1 ) ) ) ) (if_then_else (or (or (not (= (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_0 )) (not (= RESET_1 RESET_0 )) ) (not (= clk_1 clk_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 bv0[8] ) (if_then_else (bvult (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) bv1536[16] ) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 bv0[8] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv255[8] ) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 CP_RP_DIFF__CP_DIFF_RP__TIMER_0 ) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 (bvadd CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv1[8] ) )) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 CP_RP_DIFF__CP_DIFF_RP__TIMER_0 )))) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 CP_RP_DIFF__CP_DIFF_RP__TIMER_0 )) ) (= CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_0 (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) ) ) (and (and (and (if_then_else (or (or (not (= CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_1 (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) )) (not (= RESET_1 RESET_1 )) ) (not (= clk_1 clk_1 )) ) (if_then_else (= RESET_1 bv1[1] ) (= RPGRTCP_1 bv0[1] ) (if_then_else (bvult CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_1 bv1536[16] ) (= RPGRTCP_1 bv0[1] ) (if_then_else (or (or (not (= (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_0 )) (not (= RESET_1 RESET_0 )) ) (not (= clk_1 clk_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= RPGRTCP_1 bv0[1] ) (if_then_else (bvult (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) bv1536[16] ) (= RPGRTCP_1 bv0[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (bvule (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) bv32767[16] ) (if_then_else (= CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv255[8] ) (= RPGRTCP_1 bv0[1] ) (= RPGRTCP_1 RPGRTCP_0 )) (if_then_else (= CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv255[8] ) (= RPGRTCP_1 bv1[1] ) (= RPGRTCP_1 RPGRTCP_0 ))) (= RPGRTCP_1 RPGRTCP_0 )))) (= RPGRTCP_1 RPGRTCP_0 )))) (if_then_else (or (or (not (= (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_0 )) (not (= RESET_1 RESET_0 )) ) (not (= clk_1 clk_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= RPGRTCP_1 bv0[1] ) (if_then_else (bvult (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) bv1536[16] ) (= RPGRTCP_1 bv0[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (bvule (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) bv32767[16] ) (if_then_else (= CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv255[8] ) (= RPGRTCP_1 bv0[1] ) (= RPGRTCP_1 RPGRTCP_0 )) (if_then_else (= CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv255[8] ) (= RPGRTCP_1 bv1[1] ) (= RPGRTCP_1 RPGRTCP_0 ))) (= RPGRTCP_1 RPGRTCP_0 )))) (= RPGRTCP_1 RPGRTCP_0 ))) (= CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_1 (bvadd (bvmul bv1[16]  CRRNTPWR_1 ) (bvmul bv65535[16]  RFRNCPWR_1 ) ) ) ) (if_then_else (or (or (not (= (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_0 )) (not (= RESET_1 RESET_0 )) ) (not (= clk_1 clk_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 bv0[8] ) (if_then_else (bvult (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) bv1536[16] ) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 bv0[8] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv255[8] ) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 CP_RP_DIFF__CP_DIFF_RP__TIMER_0 ) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 (bvadd CP_RP_DIFF__CP_DIFF_RP__TIMER_0 bv1[8] ) )) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 CP_RP_DIFF__CP_DIFF_RP__TIMER_0 )))) (= CP_RP_DIFF__CP_DIFF_RP__TIMER_1 CP_RP_DIFF__CP_DIFF_RP__TIMER_0 )) ) (= CP_RP_DIFF__CP_DIFF_RP__CPSUBRP_0 (bvadd (bvmul bv1[16]  CRRNTPWR_0 ) (bvmul bv65535[16]  RFRNCPWR_0 ) ) ) ) ) (and (and (if_then_else (or (not (= clk_1 clk_0 )) (not (= RESET_1 RESET_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= TIMER__TIMER_TCK__COUNT_1 bv0[4] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (bvuge TIMER__TIMER_TCK__COUNT_0 bv15[4] ) (= TIMER__TIMER_TCK__COUNT_1 bv0[4] ) (= TIMER__TIMER_TCK__COUNT_1 (bvadd TIMER__TIMER_TCK__COUNT_0 bv1[1] ) )) (= TIMER__TIMER_TCK__COUNT_1 TIMER__TIMER_TCK__COUNT_0 ))) (= TIMER__TIMER_TCK__COUNT_1 TIMER__TIMER_TCK__COUNT_0 )) (and (if_then_else (or (not (= RESET_1 RESET_0 )) (not (= clk_1 clk_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv0[8] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv0[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv1[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv2[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv1[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv2[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv2[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv3[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv2[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv3[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv4[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv5[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv4[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv6[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv7[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv5[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv0[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv7[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv6[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv5[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv7[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv7[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv8[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv8[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv9[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv16[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv9[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv16[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv16[8] ) (if_then_else (= TIMETCK_0 bv0[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv17[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv17[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv18[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv19[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv18[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv19[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv20[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv19[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv0[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv7[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv20[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv21[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv21[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv22[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv22[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv0[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv23[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv24[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv23[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv24[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv24[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv25[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv25[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv32[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv33[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv32[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv33[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv34[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv33[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv0[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv7[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv34[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv35[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv36[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv35[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv36[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv36[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv37[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv38[8] )) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 )))))))))))))))))))))))))) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 ))) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 )) (if_then_else (or (not (= clk_1 clk_0 )) (not (= RESET_1 RESET_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= TIMETCK_1 bv0[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (bvuge TIMER__TIMER_TCK__COUNT_0 bv15[4] ) (= TIMETCK_1 bv1[1] ) (= TIMETCK_1 bv0[1] )) (= TIMETCK_1 TIMETCK_0 ))) (= TIMETCK_1 TIMETCK_0 )) ) ) (if_then_else (or (not (= RESET_1 RESET_0 )) (not (= clk_1 clk_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= VERNDACLOAD_1 bv1[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv0[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv1[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv2[8] ) (= VERNDACLOAD_1 bv1[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv3[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv4[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv1[1] ) (= VERNDACLOAD_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv5[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv6[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv7[8] ) (= VERNDACLOAD_1 bv1[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv8[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv9[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 VERNDACLOAD_0 )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv16[8] ) (if_then_else (= TIMETCK_0 bv0[1] ) (= VERNDACLOAD_1 bv1[1] ) (= VERNDACLOAD_1 VERNDACLOAD_0 )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv17[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv18[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv1[1] ) (= VERNDACLOAD_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv19[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv20[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv21[8] ) (= VERNDACLOAD_1 bv1[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv22[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv0[8] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv23[8] ) (= VERNDACLOAD_1 bv0[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv24[8] ) (= VERNDACLOAD_1 bv1[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv25[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv32[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv1[1] ) (= VERNDACLOAD_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv33[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv34[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv35[8] ) (= VERNDACLOAD_1 bv1[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv36[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACLOAD_1 bv0[1] ) (= VERNDACLOAD_1 bv1[1] )) (= VERNDACLOAD_1 VERNDACLOAD_0 )))))))))))))))))))))))))) (= VERNDACLOAD_1 VERNDACLOAD_0 ))) (= VERNDACLOAD_1 VERNDACLOAD_0 )) ) ) (and (and (if_then_else (or (not (= clk_1 clk_0 )) (not (= RESET_1 RESET_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= TIMER__TIMER_TCK__COUNT_1 bv0[4] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (bvuge TIMER__TIMER_TCK__COUNT_0 bv15[4] ) (= TIMER__TIMER_TCK__COUNT_1 bv0[4] ) (= TIMER__TIMER_TCK__COUNT_1 (bvadd TIMER__TIMER_TCK__COUNT_0 bv1[1] ) )) (= TIMER__TIMER_TCK__COUNT_1 TIMER__TIMER_TCK__COUNT_0 ))) (= TIMER__TIMER_TCK__COUNT_1 TIMER__TIMER_TCK__COUNT_0 )) (and (if_then_else (or (not (= RESET_1 RESET_0 )) (not (= clk_1 clk_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv0[8] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv0[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv1[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv2[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv1[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv2[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv2[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv3[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv2[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv3[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv4[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv5[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv4[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv6[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv7[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv5[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv0[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv7[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv6[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv5[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv7[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv7[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv8[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv8[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv9[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv16[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv9[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv16[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv16[8] ) (if_then_else (= TIMETCK_0 bv0[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv17[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv17[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv18[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv19[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv18[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv19[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv20[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv19[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv0[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv7[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv20[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv21[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv21[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv22[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv22[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv0[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv23[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv24[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv23[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv24[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv24[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv25[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv25[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv32[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv33[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv32[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv33[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv34[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv33[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv0[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv7[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv34[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv35[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv36[8] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv35[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv36[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv36[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv37[8] ) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 bv38[8] )) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 )))))))))))))))))))))))))) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 ))) (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_1 DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 )) (if_then_else (or (not (= clk_1 clk_0 )) (not (= RESET_1 RESET_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= TIMETCK_1 bv0[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (bvuge TIMER__TIMER_TCK__COUNT_0 bv15[4] ) (= TIMETCK_1 bv1[1] ) (= TIMETCK_1 bv0[1] )) (= TIMETCK_1 TIMETCK_0 ))) (= TIMETCK_1 TIMETCK_0 )) ) ) (if_then_else (or (not (= RESET_1 RESET_0 )) (not (= clk_1 clk_0 )) ) (if_then_else (= RESET_1 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv0[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv0[1] ) (= VERNDACSEL_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv1[8] ) (= VERNDACSEL_1 bv1[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv2[8] ) (= VERNDACSEL_1 bv1[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv3[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv4[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv5[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv6[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv7[8] ) (= VERNDACSEL_1 bv0[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv8[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv0[1] ) (= VERNDACSEL_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv9[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 VERNDACSEL_0 )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv16[8] ) (if_then_else (= TIMETCK_0 bv0[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 VERNDACSEL_0 )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv17[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv18[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv19[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv20[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv21[8] ) (= VERNDACSEL_1 bv0[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv22[8] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv0[8] ) (= VERNDACSEL_1 bv0[1] ) (= VERNDACSEL_1 bv1[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv23[8] ) (= VERNDACSEL_1 bv1[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv24[8] ) (= VERNDACSEL_1 bv1[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv25[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv32[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv33[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv34[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv1[1] ) (= VERNDACSEL_1 bv0[1] )) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv35[8] ) (= VERNDACSEL_1 bv0[1] ) (if_then_else (= DAC_CNTRL_SGNLS__DAC_CNTRL__STATE_0 bv36[8] ) (if_then_else (= TIMETCK_0 bv1[1] ) (= VERNDACSEL_1 bv0[1] ) (= VERNDACSEL_1 bv1[1] )) (= VERNDACSEL_1 VERNDACSEL_0 )))))))))))))))))))))))))) (= VERNDACSEL_1 VERNDACSEL_0 ))) (= VERNDACSEL_1 VERNDACSEL_0 )) ) )

)


