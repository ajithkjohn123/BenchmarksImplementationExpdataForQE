set terminal postscript "Helvetica" 25
set size square
set output "qesmt_vs_qelmdd.eps" 
set xlabel 'QE_SMT QE Time'
set ylabel 'QE_LMDD QE Time'
set logscale x
set logscale y
plot 'qesmt_vs_qelmdd_vs_qecombined.txt' u ($2+1):($3+1) notitle with points pt 9 lc 1 ps 2
replot 'line2.txt' notitle with linespoints lt -1
quit
