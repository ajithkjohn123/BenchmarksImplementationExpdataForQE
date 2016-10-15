set terminal postscript "Helvetica" 25
set size square
set output "allpathqelim_vs_qelmdd.eps"
set xlabel "All_Path_QElim QE Time"
set ylabel "QE_LMDD QE Time"
set logscale x
set logscale y
plot 'allpathqelim_vs_qelmdd.txt' u ($2+1):($3+1) notitle with points pt 9 lc 1 ps 2
replot 'line3.txt' notitle with linespoints lt -1
quit
