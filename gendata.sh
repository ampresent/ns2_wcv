#!/usr/bin/env zsh

nn=626
x=1000
y=1000
stop=8000
cease=2000
war=1000
echo > initialize
echo > data_flow
echo > charging_request
echo > location

cat > initialize << EOF
set val(nn)             $nn                         ;# number of mobilenodes
set val(x)		$((x+r*2))
set val(y)		$((y+r*2))
set wcv			$((nn-1))
set base		$((nn-2))
set malicious {`shuf -i 0-$((nn-3)) -n $(((nn-2) / 1)) | xargs `}            
set val(stop)		$stop
EOF
# | tr "\n" "," | sed "s/,$//g"`}

cat > /tmp/location.py << EOF
import random
X = $x
Y = $y
for i in range($nn):
  x = random.uniform($r, $x+$r)
  y = random.uniform($r, $y+$r)
  print "\$node_({}) set X_ {}".format(i ,x)
  print "\$node_({}) set Y_ {}".format(i, y)
  print "\$node_({}) set Z_ 0.000000000000".format(i)
EOF
python2 /tmp/location.py > ./location

cat ./location

#N=30; (for i in `seq $N`; do for j in `seq 0 1`; do echo "\$ns_ at "$((i*(20.1/$N)+70)) '"$con_('$j') publish"'; done; echo "\$ns_ at "$(((i+1)*(20.1/$N)+70)) '"$snk_ subscribe"'; done; )|tee -a data_flow

N=$((stop/4)); (for i in `seq $N`; do for j in `seq 0 $((nn-3))`; do rand_int=$(shuf -i 1-1000000 -n 1); t=$(echo "$rand_int*$stop/1000000" | bc -l); echo "\$ns_ at $t "'"$con_('$j') publish"'; done;  done; )|tee data_flow

for t in `seq 0 15 $stop`; do
	echo "\$ns_ at $t "'"$snk_ subscribe"' >> data_flow;
done
