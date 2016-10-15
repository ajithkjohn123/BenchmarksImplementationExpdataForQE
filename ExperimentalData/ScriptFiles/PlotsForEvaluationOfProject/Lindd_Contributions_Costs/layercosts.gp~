set terminal postscript "Helvetica" 28
set output "LayerTimes.eps"
set xlabel "Benchmark"
set ylabel "Time Per Quantifier (milli secs)"
set logscale y
set yrange[1:20000000]
set xtics 0,20000
plot 'DataLinDD.txt' u ($0):($4+1) title "Layer 1" with points pt 2 lc 1 ps 2
replot 'DataLinDD.txt' u ($0):($5+1) title "Layer 2" with points pt 6 lc 2 ps 2
replot 'DataLinDD.txt' u ($0):($6+1) title "Layer 3" with points pt 8 lc 3 ps 2
quit

