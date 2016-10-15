set terminal postscript "Helvetica" 32
set output "Layer2Contributions_Vhdl.eps"
set xlabel 'Project Call'
set ylabel 'Layer2 Contribution'
set yrange[0:135]
set xtics 0,2000
plot 'DataReal.txt' u ($0):($2*100) notitle with points pt 8 lc 1 ps 2
quit

