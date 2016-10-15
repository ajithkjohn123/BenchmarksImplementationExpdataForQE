set terminal postscript "Helvetica" 25
set size square
set output "project_vs_layer1blast.eps"
set xlabel "Average Project Time"
set ylabel "Average Layer1_Blast Time"
set logscale x
set logscale y
set xrange[1:10000000]
set yrange[1:10000000]
set xtics 1,100,1e7
set ytics 1,100,1e7
plot 'project_vs_layer1blast.txt' u ($2+1):($3+1) notitle with points pt 9 lc 1 ps 2
replot 'project_vs_layer1blast_unsuccessful.txt' u ($2+1):($3+1) notitle with points pt 6 lc 2 ps 2
replot 'line2.txt' notitle with linespoints lt -1
quit
