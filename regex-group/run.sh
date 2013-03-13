for i in `seq 2 15`;
do 

./regex -p ./ruleset/ruleset16.txt $i

gnuplot << EOF
set term post eps color
set output "result_$i.eps"
set xlabel "x"
set ylabel "y"
set key noautotitle
plot [][0:] '/home/fu/GProject/regex-group/results/record_$i.txt' with linespoints
EOF

done
