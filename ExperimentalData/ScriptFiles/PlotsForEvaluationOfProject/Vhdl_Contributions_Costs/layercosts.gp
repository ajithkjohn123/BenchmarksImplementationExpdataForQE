set terminal postscript "Helvetica" 28
set output "LayerTimes_Vhdl.eps"
set xlabel "Project Call"
set ylabel "Time Per Quantifier (milli secs)"
set logscale y
set yrange[1:10000]
set xtics 0,2000
plot 'DataReal.txt' u ($0):($4+1) title "Layer 1" with points pt 2 lc 1 ps 2
replot 'DataReal.txt' u ($0):($5+1) title "Layer 2" with points pt 6 lc 2 ps 2
replot 'DataReal.txt' u ($0):($6+1) title "Layer 3" with points pt 8 lc 3 ps 2
quit

