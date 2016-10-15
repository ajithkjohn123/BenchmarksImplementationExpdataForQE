set terminal postscript "Helvetica" 32
set output "Layer3Contributions.eps"
set xlabel 'Benchmark'
set ylabel 'Layer3 Contribution'
set yrange[0:135]
set xtics 0,20000
plot 'DataLinDD.txt' u ($0):($3*100) notitle with points pt 8 lc 1 ps 2 
quit

