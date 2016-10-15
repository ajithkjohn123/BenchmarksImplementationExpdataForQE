(benchmark test
:extrafuns ((fpga_alive_cnt_1 BitVec[16]))
:extrafuns ((fpga_alive_reg_0 BitVec[16]))
:extrafuns ((fpga_alive_reg_1 BitVec[16]))
:extrafuns ((fpga_alive_reg_written_0 BitVec[1]))
:extrafuns ((fpga_alive_reg_written_1 BitVec[1]))
:extrafuns ((s_clk_cnt_0 BitVec[32]))
:extrafuns ((s_clk_cnt_1 BitVec[32]))
:extrafuns ((s_clk_cnt_default_0 BitVec[32]))
:extrafuns ((s_clk_cnt_default_1 BitVec[32]))
:extrafuns ((state_fpga_alive_reg_0 BitVec[4]))
:extrafuns ((state_fpga_alive_reg_1 BitVec[4]))

:exists( fpga_alive_reg_0 fpga_alive_reg_1 fpga_alive_reg_written_0 fpga_alive_reg_written_1 s_clk_cnt_0 s_clk_cnt_1 s_clk_cnt_default_0 s_clk_cnt_default_1 state_fpga_alive_reg_0 state_fpga_alive_reg_1)

:formula (and (and (and (and (and (and (and (and (= (bvadd (bvmul bv1[16]  fpga_alive_cnt_1 ) (bvmul bv65535[16]  fpga_alive_reg_1 ) ) bv0[16]  ) (= (bvadd (bvmul bv65535[16]  fpga_alive_reg_0 ) (bvmul bv1[16]  fpga_alive_reg_1 ) ) bv0[16]  ) ) (= (bvadd (bvmul bv15[4]  state_fpga_alive_reg_0 ) (bvmul bv1[4]  state_fpga_alive_reg_1 ) ) bv0[4]  ) ) (= (bvmul bv1[4]  state_fpga_alive_reg_0 ) bv0[4]  ) ) (= (bvadd (bvmul bv1[1]  fpga_alive_reg_written_0 ) (bvmul bv1[1]  fpga_alive_reg_written_1 ) ) bv0[1]  ) ) (not (= (bvmul bv1[1]  fpga_alive_reg_written_0 ) bv1[1]  )) ) (= (bvadd (bvmul bv4294967295[32]  s_clk_cnt_0 ) (bvmul bv1[32]  s_clk_cnt_1 ) ) bv0[32]  ) ) (bvuge s_clk_cnt_default_0 bv80001[32]  ) ) (= (bvadd (bvmul bv4294967295[32]  s_clk_cnt_default_0 ) (bvmul bv1[32]  s_clk_cnt_default_1 ) ) bv1[32]  ) )

)
