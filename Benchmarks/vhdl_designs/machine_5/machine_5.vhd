entity b14 is
port (
	clk : in bit;
        reset : in bit;
	addr : out bit_vector (23 downto 0);
	datai : in bit_vector (23 downto 0);
	datao : out bit_vector (23 downto 0);
        rd : out bit;
	wr : out bit
	);
end b14;
architecture behav of b14 is

signal reg0 : bit_vector (23 downto 0);
signal reg1 : bit_vector (23 downto 0);
signal reg2 : bit_vector (23 downto 0);
signal reg3 : bit_vector (23 downto 0);

signal b : bit;

signal mar : bit_vector (23 downto 0);
signal mbr : bit_vector (23 downto 0);

signal mf : bit_vector (23 downto 0);
signal df : bit_vector (23 downto 0);
signal cf : bit;

signal mystate : bit_vector (23 downto 0);
signal tail : bit_vector (23 downto 0);
signal ir : bit_vector (23 downto 0);

signal state : bit;

signal r : bit_vector (23 downto 0);
signal m : bit_vector (23 downto 0);
signal t : bit_vector (23 downto 0);
signal d : bit_vector (23 downto 0);
signal temp : bit_vector (23 downto 0);
signal s : bit_vector (23 downto 0);


begin 

process(clk,reset)


begin

  if reset = '1' then
      mar <= x"000000";
      mbr <= x"000000";
      ir <= x"000000";
      d <= x"000000";
      r <= x"000000";
      m <= x"000000";
      s <= x"000000";
      temp <= x"000000";
      mf <= x"000000";
      df <= x"000000";
      mystate <= x"000000";
      cf <= '0';
      tail <= x"000000";
      b <= '0';
      reg0 <= x"000000";
      reg1 <= x"000000";
      reg2 <= x"000000";
      reg3 <= x"000000";
      addr <= x"000000";
      rd <= '0';
      wr <= '0';
      datao <= x"000000";
      state <= '0';
 elsif clk'event and clk='1' then
      rd <= '0';
      wr <= '0';
      datao <= x"000001";
      case state is
        when '0' =>
          addr <= mar;
          rd <= '1';
          mbr <= datai;
          ir <= mbr;
          state <= '1';
        when '1' =>

          case mystate is
            when x"000000" =>
                          rd<= '1';
                          wr<='0';
                          reg0 <= x"000002" * r + m;
                          reg1 <= x"000004" * r + x"000002" * m + d;
                          reg2 <= x"000004" * r + x"000008" * m;
                          reg3 <= x"000004" * r + x"000003" * m + d;
                          addr <= x"000008" * r + x"000002" * m + d;
                          mystate <= x"000001";
                          
            when x"000001" =>
                         if reg0 = x"000004"*reg1+x"000100"*reg2 then
                            rd<= '1';
                            wr<='0';
                            mystate <= x"000002";
                         else
                           mystate <= x"000003";
                         end if;  
            when x"000002" =>
                          rd<= '1';
                          wr<='1';
                          reg0 <= x"000002"*reg1+x"000001";
                          reg1 <= x"000004"*reg2+x"000002";
                          reg2 <= x"000008"*reg1+x"000004"*reg0;
                          reg3 <= x"000002"*reg0+x"000004"*reg1+x"000008"*reg2;
                          addr <= x"000000";
                          mystate <= x"000001";

             when x"000003" =>
                          rd<= '0';
                          wr<='0';
                          reg0 <= x"000001";
                          reg1 <= x"000011";
                          reg2 <= x"000002"*reg0+x"000004"*reg1+x"000001";
                          reg3 <= x"000008"*reg2+x"000010";
                          addr <= x"000011";
                          mystate <= x"000004";

             when x"000004" =>
                          if reg0 = x"000008"*reg1+x"000002" then
				if reg2 = x"000004"*reg1+x"000002" then
                            		rd<= '1';
                            		wr<='0';
                            		mystate <= x"000002";
				end if;
                         else
                           mystate <= x"000005";
                         end if;  
             when x"000005" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"000002"*reg1+x"000001";
                          reg1 <= x"000011";
                          reg2 <= x"000001";
                          reg3 <= x"000008"*reg0+x"000004"*reg1+x"000002"*reg2;
                          addr <= x"000000";
                          mystate <= x"000006";

             when x"000006" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"000008"*reg1+x"000004"*reg2+x"000002"*reg3;
                          reg1 <= x"000001";
                          reg2 <= x"000001";
                          reg3 <= x"000010";
                          addr <= x"000000";
                          mystate <= x"000007";

             when x"000007" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"000001";
                          reg1 <= x"000011";
                          reg2 <= x"000001";
                          reg3 <= x"000011";
                          addr <= x"000011";
                          mystate <= x"000008";

             when x"000008" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"000001";
                          reg1 <= x"000011";
                          reg2 <= x"000001";
                          reg3 <= x"000011";
                          addr <= x"000001";
                          mystate <= x"000009";

             when x"000009" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"000001";
                          reg1 <= x"000011";
                          reg2 <= x"000001";
                          reg3 <= x"000011";
                          addr <= x"000000";
                          mystate <= x"000010";

             when x"000010" =>
                          rd<='1';
                          wr<='1';
                          reg0 <= x"000000";
                          reg1 <= x"000010";
                          reg2 <= x"000000";
                          reg3 <= x"000010";
                          addr <= x"000000";
                          mystate <= x"000011";

             when x"000011" =>
                         if reg2 = x"000004"*reg3 + x"000010" then
                            rd<= '1';
                            wr<='1';
                            mystate <= x"000012";
                         else
                           mystate <= x"000001";
                         end if; 

             when x"000012" =>
                          rd<='0';
                          wr<='1';
                          reg0 <= x"000000";
                          reg1 <= x"000001";
                          reg2 <= x"000001";
                          reg3 <= x"000010";
                          addr <= x"000000";
                          mystate <= x"000013";

             when x"000013" =>
                          rd<='0';
                          wr<='1';
                          reg0 <= x"000004" * reg1 + x"000002" * reg2 + reg3;
                          reg1 <= x"000001";
                          reg2 <= x"000001";
                          reg3 <= x"000010";
                          addr <= x"000000";
                          mystate <= x"000014";

             when x"000014" =>
                          rd<='1';
                          wr<='0';
                          reg0 <= x"000004" * reg1 + x"000002" * reg2 + reg3;
                          reg1 <= x"000010";
                          reg2 <= x"000011";
                          reg3 <= x"000010";
                          addr <= x"000010";
                          mystate <= x"000015";

             when x"000015" =>
                          rd<='1';
                          wr<='0';
                          reg0 <= x"000004" * reg1 + x"000002" * reg2 + reg3;
                          reg1 <= x"000002" * reg2 + reg3;
                          reg2 <= x"000008" * reg1 + reg3;
                          reg3 <= x"000004" * reg1 + x"000001" * reg2 + reg0;
                          addr <= x"000000";
                          mystate <= x"000001";

 	  end case;-- case mystate

          state <= '0';
          
      end case; --case state
end if; --elsif clk'event and clk='1' 

end process;

end behav;


