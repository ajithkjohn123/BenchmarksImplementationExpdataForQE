set terminal postscript "Helvetica" 25
set size square
set output "qelmdd_vs_qecombined.eps" 
set xlabel 'QE_LMDD QE Time'
set ylabel 'QE_Combined QE Time'
set logscale x
set logscale y
plot 'qesmt_vs_qelmdd_vs_qecombined.txt' u ($3+1):($4+1) notitle with points pt 9 lc 1 ps 2
replot 'line2.txt' notitle with linespoints lt -1
quit
