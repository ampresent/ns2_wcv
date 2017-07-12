#!/usr/bin/env zsh

dist[5]=7.69113e-06
dist[9]=2.37381e-06
dist[10]=1.92278e-06
dist[11]=1.58908e-06
dist[12]=1.33527e-06
dist[13]=1.13774e-06
dist[14]=9.81011e-07
dist[15]=8.54570e-07
dist[16]=7.51087e-07
dist[20]=4.80696e-07
dist[25]=3.07645e-07
dist[30]=2.13643e-07
dist[35]=1.56962e-07
dist[40]=1.20174e-07
dist[100]=1.92278e-08

########################################
#
# Modify these parameters
#
#######################################
x=50
y=50
r=30
neighbors=8				# expectation of neighbors
stop=800
war=0
mal_ratiol=1.0
########################################

d=$dist[$r]
realr=$(./node_needed.py Pr="$d" x="$x" y="$y" neighbors="$neighbors" output="r")
nn=$(./node_needed.py Pr="$d" x="$x" y="$y" neighbors="$neighbors" output="num")
malicious=`shuf -i 0-$((nn-3)) -n $(python2 -c "print int(($nn-2)*$mal_ratiol)") | xargs `
echo > initialize
echo > data_flow
echo > charging_request
echo > location

cat > initialize << EOF
set val(nn)             $nn                         ;# number of mobilenodes
set val(x)		$((x+realr*2))
set val(y)		$((y+realr*2))
set rangex		$x
set rangey		$y
set wcv			$((nn-1))
set base		$((nn-2))
set malicious {$malicious}
set val(stop)		$stop

Phy/WirelessPhy set CSThresh_  $d
Phy/WirelessPhy set RXThresh_  $d
EOF
# | tr "\n" "," | sed "s/,$//g"`}

cat > /tmp/location.py << EOF
import random
for i in range($nn):
  x = random.uniform($realr, $x+$realr)
  y = random.uniform($realr, $y+$realr)
  print "\$node_({}) set X_ {}".format(i ,x)
  print "\$node_({}) set Y_ {}".format(i, y)
  print "\$node_({}) set Z_ 0.000000000000".format(i)
EOF
python2 /tmp/location.py > ./location

cat ./location

#N=30; (for i in `seq $N`; do for j in `seq 0 1`; do echo "\$ns_ at "$((i*(20.1/$N)+70)) '"$con_('$j') publish"'; done; echo "\$ns_ at "$(((i+1)*(20.1/$N)+70)) '"$snk_ subscribe"'; done; )|tee -a data_flow

N=$((stop/10)); (for i in `seq $N`; do for j in `seq 0 $((nn-3))`; do rand_int=$(shuf -i 1-1000000 -n 1); t=$(echo "$rand_int*$stop/1000000" | bc -l); echo "\$ns_ at $t "'"$con_('$j') publish"'; done;  done; )|tee data_flow

for t in `seq 0 15 $stop`; do
	echo "\$ns_ at $t "'"$snk_ subscribe"' >> data_flow;
done

N=20; (for i in `seq $N`; do for j in `seq 0 $((nn-3))`; do rand_int=$(shuf -i 1-1000000 -n 1); t=$(echo "$rand_int*$stop/1000000" | bc -l); auto=""; [[ "${malicious[@]}" =~ $j ]] && auto='auto'; echo "\$ns_ at $t "'"$src_('$j') publish '$auto'"'; done; done;echo "\$ns_ at 0 "'"$wcv_ subscribe"'; )|tee charging_request
