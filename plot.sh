n=$1
f=`mktemp`
echo -n 'plot' >> $f
for i in `seq 0 $n`; do
	if [ $i -ne 0 ]; then
		echo -n ',' >> $f
	fi
	echo -n ' "energy_'$i'" with linespoints' >> $f
done
echo >> $f
cat $f | gnuplot -p 
