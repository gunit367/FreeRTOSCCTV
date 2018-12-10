
library ieee;
use ieee.std_logic_1164.ALL;
use ieee.NUMERIC_STD.ALL;

entity pixel_average is
   port (
      p_old_in : in std_logic_vector(15 downto 0);
      p_new_in : in std_logic_vector(15 downto 0);
      p_out : out std_logic_vector(15 downto 0);
   );
end piexel_average;



architecture behavioral of pixel_average is


signal r_old_in : std_logic_vector(4 downto 0);
signal g_old_in : std_logic_vector(5 downto 0);
signal b_old_in : std_logic_vector(4 downto 0);

signal r_new_in : std_logic_vector(4 downto 0);
signal g_new_in : std_logic_vector(5 downto 0);
signal b_new_in : std_logic_vector(4 downto 0);

signal r_out : std_logic_vector(4 downto 0);
signal g_out : std_logic_vector(5 downto 0);
signal b_out : std_logic_vector(4 downto 0);

begin

   r_old_in <= p_old_in(15 downto 11);
   g_old_in <= p_old_in(10 downto 5);
   b_old_in <= p_old_in(4 downto 0);
   
   r_new_in <= p_new_in(15 downto 11);
   g_new_in <= p_new_in(10 downto 5);
   b_new_in <= p_new_in(4 downto 0);
   


   r_out <= std_logic_vector(unsigned('0' & r_old_in(4 downto 1)) + unsigned('0' & r_new_in(4 downto 1)));
   g_out <= std_logic_vector(unsigned('0' & g_old_in(5 downto 1)) + unsigned('0' & g_new_in(5 downto 1)));
   b_out <= std_logic_vector(unsigned('0' & b_old_in(4 downto 1)) + unsigned('0' & b_new_in(4 downto 1)));
   
   p_out <= r_out & g_out & b_out;

end behavioral;