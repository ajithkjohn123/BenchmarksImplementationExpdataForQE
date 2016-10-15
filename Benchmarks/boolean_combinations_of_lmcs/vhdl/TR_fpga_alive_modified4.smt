(benchmark tr
:extrafuns ((clk_0 BitVec[1]))
:extrafuns ((clk_1 BitVec[1]))
:extrafuns ((data_in_1 BitVec[16]))
:extrafuns ((fpga_alive_cnt_1 BitVec[16]))
:extrafuns ((fpga_alive_dis_0 BitVec[1]))
:extrafuns ((fpga_alive_dis_1 BitVec[1]))
:extrafuns ((fpga_alive_pulse_0 BitVec[1]))
:extrafuns ((fpga_alive_pulse_1 BitVec[1]))
:extrafuns ((fpga_alive_reg_0 BitVec[16]))
:extrafuns ((fpga_alive_reg_1 BitVec[16]))
:extrafuns ((fpga_alive_reg_written_0 BitVec[1]))
:extrafuns ((fpga_alive_reg_written_1 BitVec[1]))
:extrafuns ((fpga_rst_0 BitVec[1]))
:extrafuns ((fpga_rst_1 BitVec[1]))
:extrafuns ((reset_0 BitVec[1]))
:extrafuns ((reset_1 BitVec[1]))
:extrafuns ((s_clk_cnt_0 BitVec[32]))
:extrafuns ((s_clk_cnt_1 BitVec[32]))
:extrafuns ((s_clk_cnt_default_0 BitVec[32]))
:extrafuns ((s_clk_cnt_default_1 BitVec[32]))
:extrafuns ((sel_fpga_alive_reg_1 BitVec[1]))
:extrafuns ((sfpga_alive_0 BitVec[1]))
:extrafuns ((sfpga_alive_1 BitVec[1]))
:extrafuns ((state_fpga_alive_gen_pulse_0 BitVec[4]))
:extrafuns ((state_fpga_alive_gen_pulse_1 BitVec[4]))
:extrafuns ((state_fpga_alive_reg_0 BitVec[4]))
:extrafuns ((state_fpga_alive_reg_1 BitVec[4]))

:exists (fpga_alive_reg_0 fpga_alive_reg_1 fpga_alive_reg_written_0 fpga_alive_reg_written_1 s_clk_cnt_0 s_clk_cnt_1 s_clk_cnt_default_0 s_clk_cnt_default_1 state_fpga_alive_reg_0 state_fpga_alive_reg_1)


:formula (and (and (if_then_else (or (not (= clk_1 clk_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= fpga_rst_1 bv0[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (= fpga_rst_1 bv1[1] ) (= fpga_rst_1 fpga_rst_0 ))) (= fpga_rst_1 fpga_rst_0 )) (and (and (= fpga_alive_cnt_1 fpga_alive_reg_1 ) (if_then_else (or (not (= clk_1 clk_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= state_fpga_alive_reg_1 bv0[4] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= state_fpga_alive_reg_0 bv0[4] ) (if_then_else (bvugt (bvmul bv2[16] data_in_1 ) bv4[16] ) (if_then_else (= sel_fpga_alive_reg_1 bv1[1] ) (= state_fpga_alive_reg_1 bv1[4] ) (= state_fpga_alive_reg_1 state_fpga_alive_reg_0 )) (= state_fpga_alive_reg_1 state_fpga_alive_reg_0 )) (if_then_else (= state_fpga_alive_reg_0 bv1[4] ) (= state_fpga_alive_reg_1 bv2[4] ) (if_then_else (= state_fpga_alive_reg_0 bv2[4] ) (= state_fpga_alive_reg_1 bv2[4] ) (= state_fpga_alive_reg_1 bv0[4] )))) (= state_fpga_alive_reg_1 state_fpga_alive_reg_0 ))) (= state_fpga_alive_reg_1 state_fpga_alive_reg_0 )) ) (if_then_else (or (not (= clk_1 clk_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= fpga_alive_reg_1 bv0[16] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= state_fpga_alive_reg_0 bv0[4] ) (if_then_else (bvugt (bvmul bv2[16] data_in_1 ) bv4[16] ) (if_then_else (= sel_fpga_alive_reg_1 bv1[1] ) (= fpga_alive_reg_1 data_in_1 ) (= fpga_alive_reg_1 fpga_alive_reg_0 )) (= fpga_alive_reg_1 fpga_alive_reg_0 )) (= fpga_alive_reg_1 fpga_alive_reg_0 )) (= fpga_alive_reg_1 fpga_alive_reg_0 ))) (= fpga_alive_reg_1 fpga_alive_reg_0 )) ) ) (and (and (and (and (if_then_else (or (not (= sfpga_alive_1 sfpga_alive_1 )) (not (= fpga_alive_dis_1 fpga_alive_dis_1 )) ) (if_then_else (and (= fpga_alive_dis_1 bv0[1] ) (= sfpga_alive_1 bv1[1] ) ) (= fpga_alive_pulse_1 bv1[1] ) (= fpga_alive_pulse_1 bv0[1] )) (if_then_else (or (not (= sfpga_alive_1 sfpga_alive_0 )) (not (= fpga_alive_dis_1 fpga_alive_dis_1 )) ) (if_then_else (and (= fpga_alive_dis_1 bv0[1] ) (= sfpga_alive_1 bv1[1] ) ) (= fpga_alive_pulse_1 bv1[1] ) (= fpga_alive_pulse_1 bv0[1] )) (if_then_else (or (not (= sfpga_alive_0 sfpga_alive_0 )) (not (= fpga_alive_dis_1 fpga_alive_dis_0 )) ) (if_then_else (and (= fpga_alive_dis_1 bv0[1] ) (= sfpga_alive_0 bv1[1] ) ) (= fpga_alive_pulse_1 bv1[1] ) (= fpga_alive_pulse_1 bv0[1] )) (= fpga_alive_pulse_1 fpga_alive_pulse_0 )))) (if_then_else (or (not (= clk_1 clk_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= fpga_alive_reg_1 bv0[16] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= state_fpga_alive_reg_0 bv0[4] ) (if_then_else (bvugt (bvmul bv2[16] data_in_1 ) bv4[16] ) (if_then_else (= sel_fpga_alive_reg_1 bv1[1] ) (= fpga_alive_reg_1 data_in_1 ) (= fpga_alive_reg_1 fpga_alive_reg_0 )) (= fpga_alive_reg_1 fpga_alive_reg_0 )) (= fpga_alive_reg_1 fpga_alive_reg_0 )) (= fpga_alive_reg_1 fpga_alive_reg_0 ))) (= fpga_alive_reg_1 fpga_alive_reg_0 )) ) (if_then_else (or (not (= clk_1 clk_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= state_fpga_alive_reg_1 bv0[4] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= state_fpga_alive_reg_0 bv0[4] ) (if_then_else (bvugt (bvmul bv2[16] data_in_1 ) bv4[16] ) (if_then_else (= sel_fpga_alive_reg_1 bv1[1] ) (= state_fpga_alive_reg_1 bv1[4] ) (= state_fpga_alive_reg_1 state_fpga_alive_reg_0 )) (= state_fpga_alive_reg_1 state_fpga_alive_reg_0 )) (if_then_else (= state_fpga_alive_reg_0 bv1[4] ) (= state_fpga_alive_reg_1 bv2[4] ) (if_then_else (= state_fpga_alive_reg_0 bv2[4] ) (= state_fpga_alive_reg_1 bv2[4] ) (= state_fpga_alive_reg_1 bv0[4] )))) (= state_fpga_alive_reg_1 state_fpga_alive_reg_0 ))) (= state_fpga_alive_reg_1 state_fpga_alive_reg_0 )) ) (and (and (and (if_then_else (or (not (= clk_1 clk_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= fpga_alive_reg_written_1 bv0[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= state_fpga_alive_reg_0 bv0[4] ) (if_then_else (bvugt (bvmul bv2[16] data_in_1 ) bv4[16] ) (if_then_else (= sel_fpga_alive_reg_1 bv1[1] ) (= fpga_alive_reg_written_1 bv1[1] ) (= fpga_alive_reg_written_1 fpga_alive_reg_written_0 )) (= fpga_alive_reg_written_1 fpga_alive_reg_written_0 )) (= fpga_alive_reg_written_1 fpga_alive_reg_written_0 )) (= fpga_alive_reg_written_1 fpga_alive_reg_written_0 ))) (= fpga_alive_reg_written_1 fpga_alive_reg_written_0 )) (if_then_else (or (not (= clk_1 clk_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= s_clk_cnt_1 bv0[32] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= state_fpga_alive_gen_pulse_0 bv0[4] ) (= s_clk_cnt_1 bv0[32] ) (if_then_else (= state_fpga_alive_gen_pulse_0 bv1[4] ) (if_then_else (bvugt s_clk_cnt_0 bv4369[32] ) (= s_clk_cnt_1 bv0[32] ) (= s_clk_cnt_1 (bvadd s_clk_cnt_0 bv1[1] ) )) (= s_clk_cnt_1 s_clk_cnt_0 ))) (= s_clk_cnt_1 s_clk_cnt_0 ))) (= s_clk_cnt_1 s_clk_cnt_0 )) ) (if_then_else (or (not (= clk_1 clk_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= s_clk_cnt_default_1 bv0[32] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= state_fpga_alive_gen_pulse_0 bv0[4] ) (= s_clk_cnt_default_1 bv0[32] ) (if_then_else (= state_fpga_alive_gen_pulse_0 bv1[4] ) (= s_clk_cnt_default_1 s_clk_cnt_default_0 ) (if_then_else (= state_fpga_alive_gen_pulse_0 bv2[4] ) (if_then_else (bvule s_clk_cnt_default_0 bv80000[32] ) (= s_clk_cnt_default_1 bv0[32] ) (= s_clk_cnt_default_1 (bvadd s_clk_cnt_default_0 bv1[1] ) )) (= s_clk_cnt_default_1 s_clk_cnt_default_0 )))) (= s_clk_cnt_default_1 s_clk_cnt_default_0 ))) (= s_clk_cnt_default_1 s_clk_cnt_default_0 )) ) (if_then_else (or (not (= clk_1 clk_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= state_fpga_alive_gen_pulse_1 bv0[4] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= state_fpga_alive_gen_pulse_0 bv0[4] ) (if_then_else (= fpga_alive_reg_written_0 bv1[1] ) (= state_fpga_alive_gen_pulse_1 bv1[4] ) (= state_fpga_alive_gen_pulse_1 bv2[4] )) (if_then_else (= state_fpga_alive_gen_pulse_0 bv1[4] ) (= state_fpga_alive_gen_pulse_1 bv1[4] ) (if_then_else (= state_fpga_alive_gen_pulse_0 bv2[4] ) (if_then_else (bvule s_clk_cnt_default_0 bv80000[32] ) (= state_fpga_alive_gen_pulse_1 bv2[4] ) (if_then_else (= fpga_alive_reg_written_0 bv1[1] ) (= state_fpga_alive_gen_pulse_1 bv0[4] ) (= state_fpga_alive_gen_pulse_1 state_fpga_alive_gen_pulse_0 ))) (= state_fpga_alive_gen_pulse_1 bv0[4] )))) (= state_fpga_alive_gen_pulse_1 state_fpga_alive_gen_pulse_0 ))) (= state_fpga_alive_gen_pulse_1 state_fpga_alive_gen_pulse_0 )) ) ) (if_then_else (or (not (= clk_1 clk_0 )) (not (= reset_1 reset_0 )) ) (if_then_else (= reset_1 bv1[1] ) (= sfpga_alive_1 bv1[1] ) (if_then_else (and (not (= clk_1 clk_0 )) (= clk_1 bv1[1] ) ) (if_then_else (= state_fpga_alive_gen_pulse_0 bv0[4] ) (= sfpga_alive_1 bv1[1] ) (if_then_else (= state_fpga_alive_gen_pulse_0 bv1[4] ) (if_then_else (bvugt s_clk_cnt_0 bv4369[32] ) (= sfpga_alive_1 bv0[1] ) (= sfpga_alive_1 bv1[1] )) (if_then_else (= state_fpga_alive_gen_pulse_0 bv2[4] ) (if_then_else (bvule s_clk_cnt_default_0 bv80000[32] ) (= sfpga_alive_1 bv0[1] ) (= sfpga_alive_1 bv1[1] )) (= sfpga_alive_1 sfpga_alive_0 )))) (= sfpga_alive_1 sfpga_alive_0 ))) (= sfpga_alive_1 sfpga_alive_0 )) ) )

)


