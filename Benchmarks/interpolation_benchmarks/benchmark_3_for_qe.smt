(benchmark tr
:logic QF_BV
:extrafuns ((clock_1 BitVec[1]))
:extrafuns ((clock_2 BitVec[1]))
:extrafuns ((clock_3 BitVec[1]))
:extrafuns ((cont1_1 BitVec[8]))
:extrafuns ((cont1_2 BitVec[8]))
:extrafuns ((cont_1 BitVec[8]))
:extrafuns ((cont_2 BitVec[8]))
:extrafuns ((r_in_1 BitVec[8]))
:extrafuns ((r_in_2 BitVec[8]))
:extrafuns ((reset_1 BitVec[1]))
:extrafuns ((reset_2 BitVec[1]))
:extrafuns ((reset_3 BitVec[1]))
:extrafuns ((stato_1 BitVec[4]))
:extrafuns ((stato_2 BitVec[4]))
:extrafuns ((stbi_2 BitVec[1]))
:extrafuns ((x_in_2 BitVec[8]))
:extrafuns ((x_out_1 BitVec[8]))
:extrafuns ((x_out_0 BitVec[8]))

;:exists (cont1_1 cont1_2 cont_1 cont_2 r_in_1 r_in_2 stato_1 stato_2)


:formula (and (and (and (and (if_then_else (or (not (= clock_1 clock_2 )) (not (= reset_1 reset_2 )) ) (if_then_else (= reset_1 bv1[1] ) (= x_out_0 bv0[8] ) (if_then_else (and (not (= clock_1 clock_2 )) (= clock_1 bv1[1] ) ) (if_then_else (= stato_1 bv0[4] ) (= x_out_0 bv0[8] ) (if_then_else (= stato_1 bv1[4] ) (= x_out_0 bv0[8] ) (if_then_else (= stato_1 bv2[4] ) (= x_out_0 bv0[8] ) (= x_out_0 x_out_1 )))) (= x_out_0 x_out_1 ))) (= x_out_0 x_out_1 )) (if_then_else (or (not (= clock_2 clock_3 )) (not (= reset_2 reset_3 )) ) (if_then_else (= reset_2 bv1[1] ) (= cont1_1 bv0[8] ) (if_then_else (and (not (= clock_2 clock_3 )) (= clock_2 bv1[1] ) ) (if_then_else (= stato_2 bv0[4] ) (= cont1_1 cont1_2 ) (if_then_else (= stato_2 bv1[4] ) (= cont1_1 cont1_2 ) (if_then_else (= stato_2 bv2[4] ) (if_then_else (or (= r_in_2 bv0[8] ) (= r_in_2 bv99[8] ) ) (= cont1_1 r_in_2 ) (= cont1_1 cont1_2 )) (if_then_else (= stato_2 bv3[4] ) (if_then_else (= r_in_2 bv1[8] ) (= cont1_1 (bvadd cont_2 cont_2 ) ) (= cont1_1 cont_2 )) (if_then_else (= stato_2 bv4[4] ) (if_then_else (= r_in_2 bv1[8] ) (= cont1_1 (bvadd r_in_2 cont1_2 ) ) (= cont1_1 (bvadd r_in_2 cont1_2 ) )) (if_then_else (= stato_2 bv5[4] ) (if_then_else (bvugt cont1_2 bv38[8] ) (= cont1_1 (bvadd cont1_2 bv38[8] ) ) (= cont1_1 cont1_2 )) (if_then_else (= stato_2 bv6[4] ) (if_then_else (bvugt cont1_2 bv99[8] ) (= cont1_1 (bvadd cont1_2 bv38[8] ) ) (= cont1_1 cont1_2 )) (= cont1_1 cont1_2 )))))))) (= cont1_1 cont1_2 ))) (= cont1_1 cont1_2 )) ) (if_then_else (or (not (= clock_2 clock_3 )) (not (= reset_2 reset_3 )) ) (if_then_else (= reset_2 bv1[1] ) (= cont_1 bv0[8] ) (if_then_else (and (not (= clock_2 clock_3 )) (= clock_2 bv1[1] ) ) (if_then_else (= stato_2 bv0[4] ) (= cont_1 bv0[8] ) (if_then_else (= stato_2 bv1[4] ) (= cont_1 cont_2 ) (if_then_else (= stato_2 bv2[4] ) (if_then_else (or (= r_in_2 bv0[8] ) (= r_in_2 bv99[8] ) ) (if_then_else (bvule cont_2 bv37[8] ) (= cont_1 (bvadd cont_2 bv1[8] ) ) (= cont_1 bv0[8] )) (= cont_1 cont_2 )) (= cont_1 cont_2 )))) (= cont_1 cont_2 ))) (= cont_1 cont_2 )) ) (if_then_else (or (not (= clock_2 clock_3 )) (not (= reset_2 reset_3 )) ) (if_then_else (= reset_2 bv1[1] ) (= r_in_1 bv0[8] ) (if_then_else (and (not (= clock_2 clock_3 )) (= clock_2 bv1[1] ) ) (if_then_else (= stato_2 bv0[4] ) (= r_in_1 x_in_2 ) (if_then_else (= stato_2 bv1[4] ) (= r_in_1 x_in_2 ) (= r_in_1 r_in_2 ))) (= r_in_1 r_in_2 ))) (= r_in_1 r_in_2 )) ) (if_then_else (or (not (= clock_2 clock_3 )) (not (= reset_2 reset_3 )) ) (if_then_else (= reset_2 bv1[1] ) (= stato_1 bv0[4] ) (if_then_else (and (not (= clock_2 clock_3 )) (= clock_2 bv1[1] ) ) (if_then_else (= stato_2 bv0[4] ) (= stato_1 bv1[4] ) (if_then_else (= stato_2 bv1[4] ) (if_then_else (= stbi_2 bv1[1] ) (= stato_1 bv1[4] ) (= stato_1 bv2[4] )) (if_then_else (= stato_2 bv2[4] ) (if_then_else (or (= r_in_2 bv0[8] ) (= r_in_2 bv99[8] ) ) (= stato_1 bv8[4] ) (if_then_else (bvule r_in_2 bv38[8] ) (= stato_1 bv3[4] ) (= stato_1 bv1[4] ))) (if_then_else (= stato_2 bv3[4] ) (= stato_1 bv4[4] ) (if_then_else (= stato_2 bv4[4] ) (if_then_else (= r_in_2 bv1[8] ) (= stato_1 bv5[4] ) (= stato_1 bv6[4] )) (if_then_else (= stato_2 bv5[4] ) (if_then_else (bvugt cont1_2 bv38[8] ) (= stato_1 bv5[4] ) (= stato_1 bv7[4] )) (if_then_else (= stato_2 bv6[4] ) (if_then_else (bvugt cont1_2 bv99[8] ) (= stato_1 bv6[4] ) (= stato_1 bv7[4] )) (= stato_1 stato_2 )))))))) (= stato_1 stato_2 ))) (= stato_1 stato_2 )) )

)


