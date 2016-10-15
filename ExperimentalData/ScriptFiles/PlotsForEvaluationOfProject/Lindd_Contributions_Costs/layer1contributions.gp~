set terminal postscript "Helvetica" 32
set output "Layer1Contributions.eps"
set xlabel 'Benchmark'
set ylabel 'Layer1 Contribution'
set yrange[0:135]
set xtics 0,20000
plot 'DataLinDD.txt' u ($0):($1*100) notitle with points pt 8 lc 1 ps 2
quit

