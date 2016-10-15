set terminal postscript "Helvetica" 32
set output "Layer2Contributions.eps"
set xlabel 'Benchmark'
set ylabel 'Layer2 Contribution'
set yrange[0:135]
set xtics 0,20000
plot 'DataLinDD.txt' u ($0):($2*100) notitle with points pt 8 lc 1 ps 2
quit

