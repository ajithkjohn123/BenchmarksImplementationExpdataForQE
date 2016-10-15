(benchmark tr
:extrafuns ((CLK_0 BitVec[1]))
:extrafuns ((CLK_1 BitVec[1]))
:extrafuns ((Start_0 BitVec[1]))
:extrafuns ((Start_1 BitVec[1]))
:extrafuns ((X_0 BitVec[16]))
:extrafuns ((X_1 BitVec[16]))
:extrafuns ((Y_0 BitVec[16]))
:extrafuns ((Y_1 BitVec[16]))
:extrafuns ((gcdoutput_0 BitVec[16]))
:extrafuns ((gcdoutput_1 BitVec[16]))
:extrafuns ((xvar_0 BitVec[16]))
:extrafuns ((xvar_1 BitVec[16]))
:extrafuns ((yvar_0 BitVec[16]))
:extrafuns ((yvar_1 BitVec[16]))

:exists(xvar_0 xvar_1 yvar_0 yvar_1)


:formula (and (and (if_then_else (or (or (or (not (= Y_0 Y_0 )) (not (= CLK_1 CLK_0 )) ) (not (= Start_1 Start_0 )) ) (not (= X_1 X_0 )) ) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (= Start_1 bv1[1] ) (= xvar_1 X_1 ) (if_then_else (= xvar_0 yvar_0 ) (= xvar_1 xvar_0 ) (if_then_else (bvugt yvar_0 xvar_0 ) (= xvar_1 xvar_0 ) (= xvar_1 (bvadd (bvmul bv1[16]  xvar_0 ) (bvmul bv65535[16]  yvar_0 ) ) )))) (= xvar_1 xvar_0 )) (= xvar_1 xvar_0 )) (if_then_else (or (or (or (not (= X_0 X_0 )) (not (= CLK_1 CLK_0 )) ) (not (= Start_1 Start_0 )) ) (not (= Y_1 Y_0 )) ) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (= Start_1 bv1[1] ) (= yvar_1 Y_1 ) (if_then_else (= xvar_0 yvar_0 ) (= yvar_1 yvar_0 ) (if_then_else (bvugt yvar_0 xvar_0 ) (= yvar_1 (bvadd (bvmul bv65535[16]  xvar_0 ) (bvmul bv1[16]  yvar_0 ) ) ) (= yvar_1 yvar_0 )))) (= yvar_1 yvar_0 )) (= yvar_1 yvar_0 )) ) (if_then_else (or (or (or (not (= X_0 X_0 )) (not (= CLK_1 CLK_0 )) ) (not (= Start_1 Start_0 )) ) (not (= Y_0 Y_0 )) ) (if_then_else (and (not (= CLK_1 CLK_0 )) (= CLK_1 bv1[1] ) ) (if_then_else (= Start_1 bv1[1] ) (= gcdoutput_1 gcdoutput_0 ) (if_then_else (= xvar_0 yvar_0 ) (= gcdoutput_1 xvar_0 ) (= gcdoutput_1 gcdoutput_0 ))) (= gcdoutput_1 gcdoutput_0 )) (= gcdoutput_1 gcdoutput_0 )) )

)


