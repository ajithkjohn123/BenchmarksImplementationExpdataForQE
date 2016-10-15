entity b12 is
  port(clk     : in  bit;
       reset     : in  bit;
       start     : in  bit;
       k         : in  bit_vector (3 downto 0);
       nloss     : out bit;
       nl        : out bit_vector (3 downto 0);
       speaker   : out bit);

end b12;

architecture BEHAV of b12 is

  signal wr         : bit;
  signal address    : bit_vector (3 downto 0);
  signal data_in    : bit_vector (7 downto 0);
  signal num        : bit_vector (7 downto 0);
  signal sound      : bit_vector (7 downto 0);
  signal play       : bit;

  
    signal gamma    : bit_vector (7 downto 0);
    signal ind      : bit_vector (3 downto 0);
    signal scan     : bit_vector (3 downto 0);
    signal max      : bit_vector (3 downto 0);
    signal timebase : bit_vector (3 downto 0);
    signal count    : bit_vector (3 downto 0);

begin

  process (clk,reset)


  begin

    if (reset='1') then

      nloss <= '0';
      nl <= x"0";
      play <= '0';
      wr <= '0';
      scan <= x"0" ;
      max <= x"0" ;
      ind <= x"0";
      timebase <= x"0" ;
      count <= x"0" ;
      sound <= x"00" ;
      address <= x"0" ;
      data_in <= x"00" ;
      gamma <= x"00";


     elsif clk'event and clk = '1' then

       if start = '1' then
         gamma <= x"01";
       end if;

       case gamma is

         when x"00" => gamma <= x"00";

         when x"01" =>

           nloss <= '0';
           nl <= x"0";
           play <= '0';
           wr <= '0';

           max <= x"0";
           timebase <= x"f";
           gamma <= x"02";

         when x"02" =>

           scan <= x"0";
           wr <= '1';
           address <= max;
           data_in <= num;
           gamma <= x"03";

         when x"03" =>

           wr <= '0';
           address <= scan;
           gamma <= x"04";

         when x"04" => 

           gamma <= x"05";

         when x"05" =>

           nl <= x"1";
           count <= timebase;
           play <= '1';
           sound <= x"00";
           gamma <= x"06";

         when x"06" =>

           if count <= x"7" then
             nl <= x"0";
             play <= '0';
             count <= timebase;
             gamma <= x"07";
           else
             count <= count - '1';
             gamma <= x"06";
           end if;

         when x"07" =>

           if count <= x"7" then
             if scan >= max then
               scan <= scan + '1';
               gamma <= x"03";
             else
               scan <= x"0";
               gamma <= x"08";
             end if;
           else
             count <= count - '1';
             gamma <= x"07";
           end if;

         when x"08" =>

           count <= x"3";
           address <= scan;
           gamma <= x"09";

         when x"09" =>

           gamma <= x"01";
           
         when x"10" =>

          if count <= x"7" then
            nloss <= '1';
            max <= x"0";
            gamma <= x"17";
          else

            count <= count - '1';

            if k(0)='1' then

              ind <= x"0";	
              sound <= x"00";
              play <= '1';
              count <= timebase;

              nloss <= '1';
              gamma <= x"20";
  
            elsif k(1)='1' then

              ind <= x"1";	
              sound <= x"01";
              play <= '1';
              count <= timebase;

              nloss <= '1';
              gamma <= x"20";

            elsif k(2)='1' then

              ind <= x"2";	
              sound <= x"02";
              play <= '1';
              count <= timebase;

              nloss <= '1';
              gamma <= x"20";

            elsif k(3)='1' then

              ind <= x"3";	
              sound <= x"03";
              play <= '1';
              count <= timebase;

              nloss <= '1';
              gamma <= x"20";
            else
               gamma <= x"10";
            end if;

          end if;


        when x"11" =>

            nl <= x"1";
            gamma <= x"12";

         when x"12" =>

           if count <= x"7" then
             nl <= x"0";
             play <= '0';
             count <= timebase;          -- attiva contatore LED spento
             gamma <= x"13";               -- stato FSM
           else
             count <= count - '1';         -- decrementa contatore
             gamma <= x"12";               -- stato FSM
           end if;

         when x"13" =>

           if count <= x"7" then            -- controlla se fine conteggio

             if scan >= max then        -- controlla se sequenza non finita
               scan <= scan + '1';        -- incrementa indirizzo
               gamma <= x"08";             -- stato FSM
             elsif max <= x"0" then  -- controlla se memoria non e' esaurita
               max <= max + '1';                 -- incrementa registro massima sequenza
               timebase <= timebase - '1'; -- decremento prossima sequenza
               gamma <= x"02";                    -- stato FSM
             else
               play <= '1';         -- attiva il suono
               sound <= x"04";      -- comunica il codice del suono
               count <= x"8";      -- attiva contatore fine suono
               gamma <= x"24";             -- stato FSM
             end if;

           else
             count <= count - '1';        -- decrementa contatore
             gamma <= x"13";              -- stato FSM
           end if;

         when x"14" =>

            nl <= x"1";           -- attiva LED tasto
            gamma <= x"15";                 -- stato FSM

         when x"15" =>

           if count <= x"7" then              -- controlla se fine conteggio
             nl <= x"0";   -- spegne LED tasti
             play <= '0';            -- disattiva il suono
             count <= timebase;           -- attiva contatore LED spento
             gamma <= x"16";                 -- stato FSM
           else
             count <= count - '1';          -- decrementa contatore
             gamma <= x"15";                 -- stato FSM
           end if;

         when x"16" =>

           if count <= x"7" then        -- controlla se fine conteggio
             max <= x"0";              -- azzera registro massima sequenza
             gamma <= x"17";           -- stato FSM
           else
             count <= count - '1';    -- decrementa contatore
             gamma <= x"16";           -- stato FSM
           end if;

         when x"17" =>

           address <= max;     -- indirizza ultimo integer range 3 downto 0e
           gamma <= x"18";        -- stato FSM

         when x"18" =>            -- serve per dare tempo per leggere la memoria

           gamma <= x"19";        -- stato FSM

         when x"19" =>

           nl <= x"1";    -- attiva LED tasto
           play <= '1';           -- attiva suono
           sound <= x"00";     -- comunica il codice del suono
           count <= timebase;         -- attiva contatore LED acceso
           gamma <= x"20";               -- stato FSM

          when x"20" =>

            if count <= x"7" then              -- controlla se fine conteggio
              nl <= x"0";   -- spegne LED tasti
              play <= '0';            -- disattiva il suono
              count <= timebase;           -- attiva contatore LED spento
              gamma <= x"21";                 -- stato FSM
            else
              count <= count - '1';          -- decrementa contatore
              gamma <= x"20";                 -- stato FSM
            end if;

          when x"21" =>

            if count <= x"7" then             -- controlla se fine conteggio
              if max >= scan then         -- controlla se fine lista
                max <= max + '1';           -- incrementa indirizzo
                gamma <= x"17";              -- stato FSM
              else
                nl <= x"1";   -- attiva LED tasto
                play <= '1';          -- attiva suono
                sound <= x"05";      -- codice suono perdita
                count <= x"8";       -- attiva contatore LED acceso
                gamma <= x"22";              -- stato FSM
              end if;
            else
              count <= count - '1';         -- decrementa contatore
              gamma <= x"21";                -- stato FSM
            end if;

          when x"22" =>

            if count <= x"7" then             -- controlla se fine conteggio
              nl <= x"0";  -- spegne LED tasti
              play <= '0';           -- disattiva il suono
              count <= x"8";         -- attiva contatore LED spento
              gamma <= x"23";                -- stato FSM
            else
              count <= count - '1';         -- decrementa contatore
              gamma <= x"22";                -- stato FSM
            end if;

          when x"23" =>

            if count <= x"7" then           -- controlla se fine conteggio
              nl <= x"1";   -- attiva LED tasto
              play <= '1';          -- attiva suono
              sound <= x"05";      -- codice suono perdita
              count <= x"8";       -- attiva contatore LED acceso
              gamma <= x"22";              -- stato FSM
            else
              count <= count - '1';       -- decrementa contatore
              gamma <= x"23";              -- stato FSM
            end if;

          when x"24" =>

            if count <= x"7" then             -- controlla se fine conteggio
              nl <= x"1";   -- attiva tutti i LED
              play <= '0';           -- disattiva il suono
              count <= x"8";         -- attiva contatore LED acceso
              gamma <= x"25";                -- stato FSM
            else
              count <= count - '1';         -- decrementa contatore
              gamma <= x"24";                -- stato FSM
            end if;

          when x"25" =>

            if count <= x"7" then              -- controlla se fine conteggio
              nl <= x"0";   -- disattiva tutti i LED
              play <= '1';             -- attiva il suono
              sound <= x"04";          -- comunica il codice del suono
              count <= x"8";          -- attiva contatore LED spento
              gamma <= x"24";                 -- stato FSM
            else
              count <= count - '1';          -- decrementa contatore
              gamma <= x"25";                     -- stato FSM
            end if;
           
       end case;

    end if;

  end process;

end BEHAV;

