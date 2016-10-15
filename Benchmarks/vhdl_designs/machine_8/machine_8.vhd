entity b14 is
port (
	clk : in bit;
        reset : in bit;
	addr : out bit_vector (15 downto 0);
	datai : in bit_vector (7 downto 0);
	datao : out bit_vector (7 downto 0);
        rd : out bit;
	wr : out bit
	);
end b14;
architecture behav of b14 is

signal reg0 : bit_vector (7 downto 0);
signal reg1 : bit_vector (7 downto 0);
signal reg2 : bit_vector (7 downto 0);
signal reg3 : bit_vector (7 downto 0);

signal b : bit;

signal mar : bit_vector (15 downto 0);
signal mbr : bit_vector (7 downto 0);

signal mf : bit_vector (3 downto 0);
signal df : bit_vector (3 downto 0);
signal cf : bit;

signal mystate : bit_vector (7 downto 0);
signal tail : bit_vector (15 downto 0);
signal ir : bit_vector (7 downto 0);

signal state : bit;

signal r : bit_vector (7 downto 0);
signal m : bit_vector (7 downto 0);
signal t : bit_vector (7 downto 0);
signal d : bit_vector (7 downto 0);
signal temp : bit_vector (7 downto 0);
signal s : bit_vector (3 downto 0);


begin 

process(clk,reset)


begin

  if reset = '1' then
      mar <= x"0000";
      mbr <= x"0000";
      ir <= x"00";
      d <= x"00";
      r <= x"00";
      m <= x"00";
      s <= x"0";
      temp <= x"00";
      mf <= x"0";
      df <= x"0";
      mystate <= x"0";
      cf <= '0';
      tail <= x"0000";
      b <= '0';
      reg0 <= x"00";
      reg1 <= x"00";
      reg2 <= x"00";
      reg3 <= x"00";
      addr <= x"00";
      rd <= '0';
      wr <= '0';
      datao <= x"00";
      state <= '0';
 elsif clk'event and clk='1' then
      rd <= '0';
      wr <= '0';
      datao <= x"01";
      case state is
        when '0' =>
          addr <= mar;
          rd <= '1';
          mbr <= datai;
          ir <= mbr;
          state <= '1';
        when '1' =>

          case mystate is
            when x"00" =>
                          rd<= '1';
                          wr<='0';
                          reg0 <= x"00";
                          reg1 <= x"00";
                          reg2 <= x"00";
                          reg3 <= x"00";
                          addr <= x"00";
                          mystate <= x"01";
                          
            when x"01" =>
                         if reg0 > reg1 then
                            rd<= '1';
                            wr<='0';
                            mystate <= x"02";
                         else
                           mystate <= x"03";
                         end if;  
            when x"02" =>
                          rd<= '1';
                          wr<='1';
                          reg0 <= x"01";
                          reg1 <= x"01";
                          reg2 <= x"00";
                          reg3 <= x"00";
                          addr <= x"00";
                          mystate <= x"01";

             when x"03" =>
                          rd<= '0';
                          wr<='0';
                          reg0 <= x"01";
                          reg1 <= x"11";
                          reg2 <= x"01";
                          reg3 <= x"10";
                          addr <= x"11";
                          mystate <= x"04";

             when x"04" =>
                          if reg0 > reg1 then
                            rd<= '1';
                            wr<='0';
                            mystate <= x"02";
                         else
                           mystate <= x"05";
                         end if;  
             when x"05" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"01";
                          reg1 <= x"11";
                          reg2 <= x"01";
                          reg3 <= x"10";
                          addr <= x"00";
                          mystate <= x"06";

             when x"06" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"01";
                          reg1 <= x"01";
                          reg2 <= x"01";
                          reg3 <= x"10";
                          addr <= x"00";
                          mystate <= x"07";

             when x"07" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"01";
                          reg1 <= x"11";
                          reg2 <= x"01";
                          reg3 <= x"11";
                          addr <= x"11";
                          mystate <= x"08";

             when x"08" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"01";
                          reg1 <= x"11";
                          reg2 <= x"01";
                          reg3 <= x"11";
                          addr <= x"01";
                          mystate <= x"09";

             when x"09" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"01";
                          reg1 <= x"11";
                          reg2 <= x"01";
                          reg3 <= x"11";
                          addr <= x"00";
                          mystate <= x"10";

             when x"10" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"00";
                          reg1 <= x"10";
                          reg2 <= x"00";
                          reg3 <= x"10";
                          addr <= x"00";
                          mystate <= x"11";

             when x"11" =>
                         if reg2 > reg3 then
                            rd<= '1';
                            wr<='1';
                            mystate <= x"12";
                         else
                           mystate <= x"01";
                         end if; 

             when x"12" =>
                          rd<='0';
                          wr<='1';
                          reg0 <= x"00";
                          reg1 <= x"01";
                          reg2 <= x"01";
                          reg3 <= x"10";
                          addr <= x"00";
                          mystate <= x"13";

             when x"13" =>
                          rd<='0';
                          wr<='1';
                          reg0 <= x"01";
                          reg1 <= x"01";
                          reg2 <= x"01";
                          reg3 <= x"10";
                          addr <= x"00";
                          mystate <= x"14";

             when x"14" =>
                          rd<='1';
                          wr<='0';
                          reg0 <= x"01";
                          reg1 <= x"10";
                          reg2 <= x"11";
                          reg3 <= x"10";
                          addr <= x"10";
                          mystate <= x"15";

             when x"15" =>
                          rd<='1';
                          wr<='0';
                          reg0 <= x"01";
                          reg1 <= x"01";
                          reg2 <= x"11";
                          reg3 <= x"10";
                          addr <= x"10";
                          mystate <= x"1";

 	  end case;-- case mystate

          state <= '0';
          
      end case; --case state
end if; --elsif clk'event and clk='1' 

end process;

end behav;


