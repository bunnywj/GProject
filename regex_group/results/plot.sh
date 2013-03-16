#!/bin/bash
# just for gnuplot
gnuplot << EOF
set term post eps color
set output "result.eps"
set xlabel "x"
set ylabel "y"
set key noautotitle
plot [][0:] '/home/fu/GProject/regex-group/results/record.txt' with linespoints
EOF

evince result.eps

# end of file plot.sh
