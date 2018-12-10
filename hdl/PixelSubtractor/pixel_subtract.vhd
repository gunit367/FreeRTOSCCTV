

library ieee;
use ieee.std_logic_1164.ALL;
use ieee.NUMERIC_STD.ALL;

entity pixel_subtract is
   port (
      r1_in : in std_logic_vector(4 downto 0);
      g1_in : in std_logic_vector(5 downto 0);
      b1_in : in std_logic_vector(4 downto 0);
      
      r2_in : in std_logic_vector(4 downto 0);
      g2_in : in std_logic_vector(5 downto 0);
      b2_in : in std_logic_vector(4 downto 0);
      
      r_out : out std_logic_vector(4 downto 0);
      g_out : out std_logic_vector(5 downto 0);
      b_out : out std_logic_vector(4 downto 0)
   );
end pixel_subtract;

architecture behavioral of pixel_subtract is

begin

   r_out <= std_logic_vector(abs(signed('0' & r1_in) - signed('0' & r2_in)));
   g_out <= std_logic_vector(abs(signed('0' & g1_in) - signed('0' & g2_in)));
   b_out <= std_logic_vector(abs(signed('0' & b1_in) - signed('0' & b2_in)));

end behavioral;