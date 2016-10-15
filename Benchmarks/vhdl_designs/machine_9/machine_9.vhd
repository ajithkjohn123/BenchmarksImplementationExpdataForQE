entity b14 is
port (
	clk : in bit;
        reset : in bit;
	addr : out bit_vector (31 downto 0);
	datai : in bit_vector (31 downto 0);
	datao : out bit_vector (31 downto 0);
        rd : out bit;
	wr : out bit
	);
end b14;
architecture behav of b14 is

signal reg0 : bit_vector (31 downto 0);
signal reg1 : bit_vector (31 downto 0);
signal reg2 : bit_vector (31 downto 0);
signal reg3 : bit_vector (31 downto 0);

signal b : bit;

signal mar : bit_vector (31 downto 0);
signal mbr : bit_vector (31 downto 0);

signal mf : bit_vector (31 downto 0);
signal df : bit_vector (31 downto 0);
signal cf : bit;

signal mystate : bit_vector (31 downto 0);
signal tail : bit_vector (31 downto 0);
signal ir : bit_vector (31 downto 0);

signal state : bit;

signal r : bit_vector (31 downto 0);
signal m : bit_vector (31 downto 0);
signal t : bit_vector (31 downto 0);
signal d : bit_vector (31 downto 0);
signal temp : bit_vector (31 downto 0);
signal s : bit_vector (31 downto 0);


begin 

process(clk,reset)


begin

  if reset = '1' then
      mar <= x"00000000";
      mbr <= x"00000000";
      ir <= x"00000000";
      d <= x"00000000";
      r <= x"00000000";
      m <= x"00000000";
      s <= x"00000000";
      temp <= x"00000000";
      mf <= x"00000000";
      df <= x"00000000";
      mystate <= x"00000000";
      cf <= x"00000000";
      tail <= x"00000000";
      b <= '0';
      reg0 <= x"00000000";
      reg1 <= x"00000000";
      reg2 <= x"00000000";
      reg3 <= x"00000000";
      addr <= x"00000000";
      rd <= '0';
      wr <= '0';
      datao <= x"00000000";
      state <= '0';
 elsif clk'event and clk='1' then
      rd <= '0';
      wr <= '0';
      datao <= x"00000001";
      case state is
        when '0' =>
          addr <= mar;
          rd <= '1';
          mbr <= datai;
          ir <= mbr;
          state <= '1';
        when '1' =>

          case mystate is
            when x"00000000" =>
                          rd<= '1';
                          wr<='0';
                          reg0 <= x"00000000";
                          reg1 <= x"00000000";
                          reg2 <= x"00000000";
                          reg3 <= x"00000000";
                          addr <= x"00000000";
                          mystate <= x"00000001";
                          
            when x"00000001" =>
                         if reg0 > reg1 then
                            rd<= '1';
                            wr<='0';
                            mystate <= x"00000002";
                         else
                           mystate <= x"00000003";
                         end if;  
            when x"00000002" =>
                          rd<= '1';
                          wr<='1';
                          reg0 <= x"00000001";
                          reg1 <= x"00000001";
                          reg2 <= x"00000000";
                          reg3 <= x"00000000";
                          addr <= x"00000000";
                          mystate <= x"00000001";

             when x"00000003" =>
                          rd<= '0';
                          wr<='0';
                          reg0 <= x"00000001";
                          reg1 <= x"00000011";
                          reg2 <= x"00000001";
                          reg3 <= x"00000010";
                          addr <= x"00000011";
                          mystate <= x"00000004";

             when x"00000004" =>
                          if reg0 > reg1 then
                            rd<= '1';
                            wr<='0';
                            mystate <= x"00000002";
                         else
                           mystate <= x"00000005";
                         end if;  
             when x"00000005" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"00000001";
                          reg1 <= x"00000011";
                          reg2 <= x"00000001";
                          reg3 <= x"00000010";
                          addr <= x"00000000";
                          mystate <= x"00000006";

             when x"00000006" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"00000001";
                          reg1 <= x"00000001";
                          reg2 <= x"00000001";
                          reg3 <= x"00000010";
                          addr <= x"00000000";
                          mystate <= x"00000007";

             when x"00000007" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"00000001";
                          reg1 <= x"00000011";
                          reg2 <= x"00000001";
                          reg3 <= x"00000011";
                          addr <= x"00000011";
                          mystate <= x"00000008";

             when x"00000008" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"00000001";
                          reg1 <= x"00000011";
                          reg2 <= x"00000001";
                          reg3 <= x"00000011";
                          addr <= x"00000001";
                          mystate <= x"00000009";

             when x"00000009" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"00000001";
                          reg1 <= x"00000011";
                          reg2 <= x"00000001";
                          reg3 <= x"00000011";
                          addr <= x"00000000";
                          mystate <= x"00000010";

             when x"00000010" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"00000000";
                          reg1 <= x"00000010";
                          reg2 <= x"00000000";
                          reg3 <= x"00000010";
                          addr <= x"00000000";
                          mystate <= x"00000011";

             when x"00000011" =>
                         if reg2 > reg3 then
                            rd<= '1';
                            wr<='1';
                            mystate <= x"00000012";
                         else
                           mystate <= x"00000001";
                         end if; 

             when x"00000012" =>
                          rd<='0';
                          wr<='1';
                          reg0 <= x"00000000";
                          reg1 <= x"00000001";
                          reg2 <= x"00000001";
                          reg3 <= x"00000010";
                          addr <= x"00000000";
                          mystate <= x"00000013";

             when x"00000013" =>
                          rd<='0';
                          wr<='1';
                          reg0 <= x"00000001";
                          reg1 <= x"00000001";
                          reg2 <= x"00000001";
                          reg3 <= x"00000010";
                          addr <= x"00000000";
                          mystate <= x"00000014";

             when x"00000014" =>
                          rd<='1';
                          wr<='0';
                          reg0 <= x"00000001";
                          reg1 <= x"00000010";
                          reg2 <= x"00000011";
                          reg3 <= x"00000010";
                          addr <= x"00000010";
                          mystate <= x"00000015";

             when x"00000015" =>
                          rd<='1';
                          wr<='0';
                          reg0 <= x"00000001";
                          reg1 <= x"00000001";
                          reg2 <= x"00000011";
                          reg3 <= x"00000010";
                          addr <= x"00000010";
                          mystate <= x"0000001";

 	  end case;-- case mystate

          state <= '0';
          
      end case; --case state
end if; --elsif clk'event and clk='1' 

end process;

end behav;


