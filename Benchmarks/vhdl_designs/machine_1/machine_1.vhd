entity b12 is
  port(clk     : in  bit;
       reset     : in  bit;
       start     : in  bit;
       k         : in  bit_vector (31 downto 0);
       num       : in  bit_vector (31 downto 0);
       nloss     : out bit;
       nl        : out bit_vector (31 downto 0);
       speaker   : out bit);

end b12;

architecture BEHAV of b12 is

  signal wr         : bit;
  signal address    : bit_vector (31 downto 0);
  signal data_in    : bit_vector (31 downto 0);  
  signal sound      : bit_vector (31 downto 0);
  signal play       : bit;

  
    signal gamma    : bit_vector (31 downto 0);
    signal ind      : bit_vector (31 downto 0);
    signal scan     : bit_vector (31 downto 0);
    signal max      : bit_vector (31 downto 0);
    signal timebase : bit_vector (31 downto 0);
    signal count    : bit_vector (31 downto 0);

begin

  process (clk,reset)


  begin

    if (reset='1') then

      nloss <= '0';
      nl <= x"00000000";
      play <= '0';
      wr <= '0';
      scan <= x"00000000" ;
      max <= x"00000000" ;
      ind <= x"00000000";
      timebase <= x"00000000" ;
      count <= x"00000000" ;
      sound <= x"00000000" ;
      address <= x"00000000" ;
      data_in <= x"00000000" ;
      gamma <= x"00000000";


     elsif clk'event and clk = '1' then

       if start = '1' then
         gamma <= x"00000004"*num+x"00000002"*sound;
       end if;

       case gamma is

         when x"00000000" => gamma <= x"00000002"*num+x"00000002"*sound;

         when x"00000001" =>

           nloss <= '0';
           nl <= x"00000000";
           play <= '0';
           wr <= '0';

           max <= x"00000000";
           timebase <= x"0000000f";
           gamma <= x"00000008"*num+x"00000004"*sound;

         when x"00000002" =>

           scan <= x"00000000";
           wr <= '1';
           address <= max;
           data_in <= num;
           gamma <= x"00000003";

         when x"00000003" =>

           wr <= '0';
           address <= scan;
           gamma <= x"00000016"*num+x"00000002"*sound;

         when x"00000004" => 

           gamma <= x"00000005";

         when x"00000005" =>

           nl <= x"00000001";
           count <= timebase;
           play <= '1';
           sound <= x"00000000";
           gamma <= x"00000006";

         when x"00000006" =>

           if count = x"00000002"*scan+max then
             nl <= x"00000000";
             play <= '0';
             count <= timebase;
             gamma <= x"00000007";
           else
             count <= count - '1';
             gamma <= x"00000006";
           end if;

         when x"00000007" =>

           if count = x"00000004"*scan+x"00000002"*max then
             if scan = x"00000002"*max then
               scan <= scan + '1';
               gamma <= x"00000003";
             else
               scan <= x"00000000";
               gamma <= x"00000008";
             end if;
           else
             count <= count - '1';
             gamma <= x"00000007";
           end if;

         when x"00000008" =>

           count <= x"00000003";
           address <= scan;
           gamma <= x"00000009";

         when x"00000009" =>

           gamma <= x"00000001";
           
         when x"00000010" =>

          if count = x"00000004"*scan+x"00000002"*max then
            nloss <= '1';
            max <= x"00000000";
            gamma <= x"00000017";
          else

            count <= count - '1';

            if k(0)='1' then

              ind <= x"00000000";	
              sound <= x"00000000";
              play <= '1';
              count <= timebase;

              nloss <= '1';
              gamma <= x"00000020";
  
            elsif k(1)='1' then

              ind <= x"00000001";	
              sound <= x"00000001";
              play <= '1';
              count <= timebase;

              nloss <= '1';
              gamma <= x"00000020";

            elsif k(2)='1' then

              ind <= x"00000002";	
              sound <= x"00000002";
              play <= '1';
              count <= timebase;

              nloss <= '1';
              gamma <= x"00000020";

            elsif k(3)='1' then

              ind <= x"00000003";	
              sound <= x"00000003";
              play <= '1';
              count <= timebase;

              nloss <= '1';
              gamma <= x"00000020";
            else
               gamma <= x"00000010";
            end if;

          end if;


        when x"00000011" =>

            nl <= x"00000001";
            gamma <= x"00000012";

         when x"00000012" =>

           if count = x"00000008"*scan+x"00000002"*max then
             nl <= x"00000000";
             play <= '0';
             count <= timebase;          -- attiva contatore LED spento
             gamma <= x"00000013";               -- stato FSM
           else
             count <= count - '1';         -- decrementa contatore
             gamma <= x"00000012";               -- stato FSM
           end if;

         when x"00000013" =>

           if count = x"00000000" then            -- controlla se fine conteggio

             if scan = max then        -- controlla se sequenza non finita
               scan <= scan + '1';        -- incrementa indirizzo
               gamma <= x"00000008";             -- stato FSM
             elsif max = x"00000000" then  -- controlla se memoria non e' esaurita
               max <= max + '1';                 -- incrementa registro massima sequenza
               timebase <= timebase - '1'; -- decremento prossima sequenza
               gamma <= x"00000002";                    -- stato FSM
             else
               play <= '1';         -- attiva il suono
               sound <= x"00000004";      -- comunica il codice del suono
               count <= x"00000008";      -- attiva contatore fine suono
               gamma <= x"00000024";             -- stato FSM
             end if;

           else
             count <= count - '1';        -- decrementa contatore
             gamma <= x"00000013";              -- stato FSM
           end if;

         when x"00000014" =>

            nl <= x"00000001";           -- attiva LED tasto
            gamma <= x"00000015";                 -- stato FSM

         when x"00000015" =>

           if count = x"00000008"*scan+x"00000004"*max then              -- controlla se fine conteggio
             nl <= x"00000000";   -- spegne LED tasti
             play <= '0';            -- disattiva il suono
             count <= timebase;           -- attiva contatore LED spento
             gamma <= x"00000016";                 -- stato FSM
           else
             count <= count - '1';          -- decrementa contatore
             gamma <= x"00000015";                 -- stato FSM
           end if;

         when x"00000016" =>

           if count = x"00000002"*scan+x"00000008"*max then        -- controlla se fine conteggio
             max <= x"00000000";              -- azzera registro massima sequenza
             gamma <= x"00000017";           -- stato FSM
           else
             count <= count - '1';    -- decrementa contatore
             gamma <= x"00000016";           -- stato FSM
           end if;

         when x"00000017" =>

           address <= max;     -- indirizza ultimo integer range 3 downto 0e
           gamma <= x"00000018";        -- stato FSM

         when x"00000018" =>            -- serve per dare tempo per leggere la memoria

           gamma <= x"00000019";        -- stato FSM

         when x"00000019" =>

           nl <= x"00000001";    -- attiva LED tasto
           play <= '1';           -- attiva suono
           sound <= x"00000000";     -- comunica il codice del suono
           count <= timebase;         -- attiva contatore LED acceso
           gamma <= x"00000020";               -- stato FSM

          when x"00000020" =>

            if count = x"00000002"*scan+x"00000004"*max then              -- controlla se fine conteggio
              nl <= x"00000000";   -- spegne LED tasti
              play <= '0';            -- disattiva il suono
              count <= timebase;           -- attiva contatore LED spento
              gamma <= x"00000021";                 -- stato FSM
            else
              count <= count - '1';          -- decrementa contatore
              gamma <= x"00000020";                 -- stato FSM
            end if;

          when x"00000021" =>

            if count = x"00000000" then             -- controlla se fine conteggio
              if max = scan then         -- controlla se fine lista
                max <= max + '1';           -- incrementa indirizzo
                gamma <= x"00000017";              -- stato FSM
              else
                nl <= x"00000001";   -- attiva LED tasto
                play <= '1';          -- attiva suono
                sound <= x"00000005";      -- codice suono perdita
                count <= x"00000008";       -- attiva contatore LED acceso
                gamma <= x"00000022";              -- stato FSM
              end if;
            else
              count <= count - '1';         -- decrementa contatore
              gamma <= x"00000021";                -- stato FSM
            end if;

          when x"00000022" =>

            if count = x"00000000" then             -- controlla se fine conteggio
              nl <= x"00000000";  -- spegne LED tasti
              play <= '0';           -- disattiva il suono
              count <= x"00000008";         -- attiva contatore LED spento
              gamma <= x"00000023";                -- stato FSM
            else
              count <= count - '1';         -- decrementa contatore
              gamma <= x"00000022";                -- stato FSM
            end if;

          when x"00000023" =>

            if count = x"00000004"*scan+x"00000002"*timebase+x"00000008"*ind then           -- controlla se fine conteggio
              nl <= x"00000001";   -- attiva LED tasto
              play <= '1';          -- attiva suono
              sound <= x"00000005";      -- codice suono perdita
              count <= x"00000008";       -- attiva contatore LED acceso
              gamma <= x"00000022";              -- stato FSM
            else
              count <= count - '1';       -- decrementa contatore
              gamma <= x"00000023";              -- stato FSM
            end if;

          when x"00000024" =>

            if count = x"00000004"*scan+x"00000002"*timebase+x"00000008"*ind then             -- controlla se fine conteggio
              nl <= x"00000001";   -- attiva tutti i LED
              play <= '0';           -- disattiva il suono
              count <= x"00000008";         -- attiva contatore LED acceso
              gamma <= x"00000025";                -- stato FSM
            else
              count <= count - '1';         -- decrementa contatore
              gamma <= x"00000024";                -- stato FSM
            end if;

          when x"00000025" =>

            if count = x"00000004"*scan+x"00000002"*ind then              -- controlla se fine conteggio
              nl <= x"00000000";   -- disattiva tutti i LED
              play <= '1';             -- attiva il suono
              sound <= x"00000004";          -- comunica il codice del suono
              count <= x"00000008";          -- attiva contatore LED spento
              gamma <= x"00000024";                 -- stato FSM
            else
              count <= count - '1';          -- decrementa contatore
              gamma <= x"00000025";                     -- stato FSM
            end if;
           
       end case;

    end if;

  end process;

end BEHAV;

