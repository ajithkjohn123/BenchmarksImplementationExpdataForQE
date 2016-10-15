(benchmark tr
:extrafuns ((A_1 BitVec[16]))
:extrafuns ((A_2 BitVec[16]))
:extrafuns ((COUNT_1 BitVec[8]))
:extrafuns ((COUNT_2 BitVec[8]))
:extrafuns ((COUNT_3 BitVec[8]))
:extrafuns ((FDBCKSUM_0 BitVec[16]))
:extrafuns ((RESET_1 BitVec[1]))
:extrafuns ((RESET_2 BitVec[1]))
:extrafuns ((RESET_3 BitVec[1]))
:extrafuns ((RESET_4 BitVec[1]))
:extrafuns ((RNGCHNGFLG_2 BitVec[1]))
:extrafuns ((RNGCHNGFLG_3 BitVec[1]))
:extrafuns ((RNGCHNGFLG_4 BitVec[1]))
:extrafuns ((SUM128L_1 BitVec[16]))
:extrafuns ((SUM128L_0 BitVec[16]))
:extrafuns ((SUM128UL_2 BitVec[16]))
:extrafuns ((VLDEOC_1 BitVec[1]))
:extrafuns ((VLDEOC_2 BitVec[1]))
:extrafuns ((VLDEOC_3 BitVec[1]))
:extrafuns ((VLDEOC_4 BitVec[1]))

:exists(COUNT_1 COUNT_2 COUNT_3 A_1 A_2)


:formula (and (if_then_else (or (or (not (= RNGCHNGFLG_3 RNGCHNGFLG_4 )) (not (= RESET_3 RESET_4 )) ) (not (= VLDEOC_3 VLDEOC_4 )) ) (if_then_else (= RESET_3 bv1[1] ) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 SUM128UL_2 ) ) )) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) )))) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) )) (if_then_else (= RNGCHNGFLG_3 bv1[1] ) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 SUM128UL_2 ) ) )) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) )))) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) )) (if_then_else (and (not (= VLDEOC_3 VLDEOC_4 )) (= VLDEOC_3 bv1[1] ) ) (if_then_else (bvugt COUNT_3 bv128[8] ) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 SUM128UL_2 ) ) )) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) )))) (and (= COUNT_2 bv0[1] ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) )) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (and (= COUNT_2 (bvadd COUNT_3 bv1[1] ) ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (and (= COUNT_2 (bvadd COUNT_3 bv1[1] ) ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (and (= COUNT_2 (bvadd COUNT_3 bv1[1] ) ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (and (= COUNT_2 (bvadd COUNT_3 bv1[1] ) ) (and (= FDBCKSUM_0 A_1 ) (= A_1 SUM128UL_2 ) ) )) (and (= COUNT_2 (bvadd COUNT_3 bv1[1] ) ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) )))) (and (= COUNT_2 (bvadd COUNT_3 bv1[1] ) ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) ))) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 SUM128UL_2 ) ) )) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) )))) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) ))))) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 bv0[16] ) ) ) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 SUM128UL_2 ) ) )) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) )))) (and (= COUNT_2 COUNT_3 ) (and (= FDBCKSUM_0 A_1 ) (= A_1 A_2 ) ) ))) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 bv0[1] ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 (bvadd COUNT_2 bv1[1] ) ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )))) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )))))) (if_then_else (or (or (not (= RNGCHNGFLG_2 RNGCHNGFLG_3 )) (not (= RESET_2 RESET_3 )) ) (not (= VLDEOC_2 VLDEOC_3 )) ) (if_then_else (= RESET_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (= RNGCHNGFLG_2 bv1[1] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (and (not (= VLDEOC_2 VLDEOC_3 )) (= VLDEOC_2 bv1[1] ) ) (if_then_else (bvugt COUNT_2 bv128[8] ) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 bv0[16] ) (= SUM128L_0 SUM128L_1 ) ) )) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 SUM128UL_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))))) (if_then_else (or (not (= VLDEOC_1 VLDEOC_2 )) (not (= RESET_1 RESET_2 )) ) (if_then_else (= RESET_1 bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (if_then_else (and (not (= VLDEOC_1 VLDEOC_2 )) (= VLDEOC_1 bv1[1] ) ) (if_then_else (bvugt COUNT_1 bv128[8] ) (if_then_else (= (extract[15 :15 ] A_1) bv1[1] ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 bv0[16] ) ) ) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 A_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) ))) (and (= COUNT_1 COUNT_2 ) (and (= A_1 A_2 ) (= SUM128L_0 SUM128L_1 ) ) )))) )

)


