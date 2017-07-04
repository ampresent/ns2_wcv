#!/usr/bin/env zsh

script=`mktemp`

cat >> $script << EOF
set xlabel "time"
set ylabel "energy"
set title "Energy Curve"
EOF


for i in `seq 0 $1`; do 
	points="/tmp/points_$i"
	echo > $points
	ag '\-n '$i' \-e' scenario1.tr | awk '{print $3,$7}' >> $points
	echo 'e' >> $points
	request=`mktemp`
	grep 'Node'$i': Sending Data' log | awk '{print $1}' > $request
	request_full=`mktemp`
	awk 'NR==FNR{req[$1]=$2} NR>FNR{print $1,",",req[$1]}' $points $request > $request_full

	cat $request_full | while read line; do
		echo "set object circle at $line size 0.5" >> $script
	done

	#echo 'plot - title "Node: $1" with lines' >> $script
done
echo -n 'plot ' >> $script
for i in `seq 0 $1`; do 
	echo -n "'/tmp/points_$i' title 'Node: $i' with lines," >> $script
done

LD_LIBRARY_PATH=.  cat $points | gnuplot $script -p
