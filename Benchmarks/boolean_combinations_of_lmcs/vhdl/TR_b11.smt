(benchmark tr
:extrafuns ((clock_0 BitVec[1]))
:extrafuns ((clock_1 BitVec[1]))
:extrafuns ((cont1_0 BitVec[8]))
:extrafuns ((cont1_1 BitVec[8]))
:extrafuns ((cont_0 BitVec[8]))
:extrafuns ((cont_1 BitVec[8]))
:extrafuns ((r_in_0 BitVec[8]))
:extrafuns ((r_in_1 BitVec[8]))
:extrafuns ((reset_0 BitVec[1]))
:extrafuns ((reset_1 BitVec[1]))
:extrafuns ((stato_0 BitVec[4]))
:extrafuns ((stato_1 BitVec[4]))
:extrafuns ((stbi_1 BitVec[1]))
:extrafuns ((x_in_1 BitVec[8]))
:extrafuns ((x_out_0 BitVec[8]))
:extrafuns ((x_out_1 BitVec[8]))

:exists (cont1_0 cont1_1 cont_0 cont_1 r_in_0 r_in_1 stato_0 stato_1)

:formula (and (and (and (and (if_then_else (or (not (= clock_1 clock_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= cont1_1 bv0[8] ) (if_then_else (and (not (= clock_1 clock_0 )) (= clock_1 bv1[1] ) ) (if_then_else (= stato_0 bv0[4] ) (= cont1_1 cont1_0 ) (if_then_else (= stato_0 bv1[4] ) (= cont1_1 cont1_0 ) (if_then_else (= stato_0 bv2[4] ) (if_then_else (or (= r_in_0 bv0[8] ) (= r_in_0 bv99[8] ) ) (= cont1_1 r_in_0 ) (= cont1_1 cont1_0 )) (if_then_else (= stato_0 bv3[4] ) (if_then_else (= r_in_0 bv1[1] ) (= cont1_1 (bvadd cont_0 cont_0 ) ) (= cont1_1 cont_0 )) (if_then_else (= stato_0 bv4[4] ) (if_then_else (= r_in_0 bv1[1] ) (= cont1_1 (bvadd r_in_0 cont1_0 ) ) (= cont1_1 (bvadd (bvmul bv255[8]  cont1_0 ) (bvmul bv1[8]  r_in_0 ) ) )) (if_then_else (= stato_0 bv5[4] ) (if_then_else (bvugt cont1_0 bv38[8] ) (= cont1_1 (bvadd (bvmul bv1[8]  cont1_0 ) bv218[8]  ) ) (= cont1_1 cont1_0 )) (if_then_else (= stato_0 bv6[4] ) (if_then_else (bvugt cont1_0 bv99[8] ) (= cont1_1 (bvadd cont1_0 bv38[8] ) ) (= cont1_1 cont1_0 )) (= cont1_1 cont1_0 )))))))) (= cont1_1 cont1_0 ))) (= cont1_1 cont1_0 )) (if_then_else (or (not (= clock_1 clock_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= cont_1 bv0[8] ) (if_then_else (and (not (= clock_1 clock_0 )) (= clock_1 bv1[1] ) ) (if_then_else (= stato_0 bv0[4] ) (= cont_1 bv0[8] ) (if_then_else (= stato_0 bv1[4] ) (= cont_1 cont_0 ) (if_then_else (= stato_0 bv2[4] ) (if_then_else (or (= r_in_0 bv0[8] ) (= r_in_0 bv99[8] ) ) (if_then_else (bvule cont_0 bv37[8] ) (= cont_1 (bvadd cont_0 bv1[1] ) ) (= cont_1 bv0[8] )) (= cont_1 cont_0 )) (= cont_1 cont_0 )))) (= cont_1 cont_0 ))) (= cont_1 cont_0 )) ) (if_then_else (or (not (= clock_1 clock_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= r_in_1 bv0[8] ) (if_then_else (and (not (= clock_1 clock_0 )) (= clock_1 bv1[1] ) ) (if_then_else (= stato_0 bv0[4] ) (= r_in_1 x_in_1 ) (if_then_else (= stato_0 bv1[4] ) (= r_in_1 x_in_1 ) (= r_in_1 r_in_0 ))) (= r_in_1 r_in_0 ))) (= r_in_1 r_in_0 )) ) (if_then_else (or (not (= clock_1 clock_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= stato_1 bv0[4] ) (if_then_else (and (not (= clock_1 clock_0 )) (= clock_1 bv1[1] ) ) (if_then_else (= stato_0 bv0[4] ) (= stato_1 bv1[4] ) (if_then_else (= stato_0 bv1[4] ) (if_then_else (= stbi_1 bv1[1] ) (= stato_1 bv1[4] ) (= stato_1 bv2[4] )) (if_then_else (= stato_0 bv2[4] ) (if_then_else (or (= r_in_0 bv0[8] ) (= r_in_0 bv99[8] ) ) (= stato_1 bv8[4] ) (if_then_else (bvule r_in_0 bv38[8] ) (= stato_1 bv3[4] ) (= stato_1 bv1[4] ))) (if_then_else (= stato_0 bv3[4] ) (= stato_1 bv4[4] ) (if_then_else (= stato_0 bv4[4] ) (if_then_else (= r_in_0 bv1[1] ) (= stato_1 bv5[4] ) (= stato_1 bv6[4] )) (if_then_else (= stato_0 bv5[4] ) (if_then_else (bvugt cont1_0 bv38[8] ) (= stato_1 bv5[4] ) (= stato_1 bv7[4] )) (if_then_else (= stato_0 bv6[4] ) (if_then_else (bvugt cont1_0 bv99[8] ) (= stato_1 bv6[4] ) (= stato_1 bv7[4] )) (= stato_1 stato_0 )))))))) (= stato_1 stato_0 ))) (= stato_1 stato_0 )) ) (if_then_else (or (not (= clock_1 clock_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= x_out_1 bv0[8] ) (if_then_else (and (not (= clock_1 clock_0 )) (= clock_1 bv1[1] ) ) (if_then_else (= stato_0 bv0[4] ) (= x_out_1 bv0[8] ) (if_then_else (= stato_0 bv1[4] ) (= x_out_1 bv0[8] ) (if_then_else (= stato_0 bv2[4] ) (= x_out_1 bv0[8] ) (= x_out_1 x_out_0 )))) (= x_out_1 x_out_0 ))) (= x_out_1 x_out_0 )) )

)


