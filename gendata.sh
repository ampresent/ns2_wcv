#!/usr/bin/env zsh

echo > initialize
echo > data_flow
echo > charging_request
echo > location

cat > initialize << EOF
set val(nn)             5                         ;# number of mobilenodes
set val(x)		50
set val(y)		50
set wcv			4
set base		3
EOF

cat > location << EOF
\$node_(0) set X_ 0.0
\$node_(0) set Y_ 10.0
\$node_(0) set Z_ 0.000000000000
\$node_(1) set X_ 10.0
\$node_(1) set Y_ 10.0
\$node_(1) set Z_ 0.000000000000
\$node_(2) set X_ 5.0
\$node_(2) set Y_ 0.0
\$node_(2) set Z_ 0.000000000000
\$node_(3) set X_ 20.0
\$node_(3) set Y_ 20.0
\$node_(3) set Z_ 0.000000000000
\$node_(4) set X_ 0.0
\$node_(4) set Y_ 0.0
\$node_(4) set Z_ 0.000000000000
EOF


#N=30; (for i in `seq $N`; do for j in `seq 0 1`; do echo "\$ns_ at "$((i*(20.1/$N)+70)) '"$con_('$j') publish"'; done; echo "\$ns_ at "$(((i+1)*(20.1/$N)+70)) '"$snk_ subscribe"'; done; )|tee -a data_flow

N=7; (for i in `seq $N`; do for j in 0 1 2; do rand_int=$(shuf -i 1-1000000 -n 1); t=$(echo "$rand_int*200/1000000" | bc -l); echo "\$ns_ at $t "'"$con_('$j') publish"'; done; rand_int=$(shuf -i 1-1000000 -n 1); t=$(echo "$rand_int*200/1000000" | bc -l); echo "\$ns_ at $t "'"$snk_ subscribe"'; done; )|tee data_flow

N=10; (for i in `seq $N`; do for j in 0 1 2; do rand_int=$(shuf -i 1-1000000 -n 1); t=$(echo "$rand_int*200/1000000" | bc -l); auto=""; [ $(bc<<<"$t>100") -eq 1 ] && auto='auto'; echo "\$ns_ at $t "'"$src_('$j') publish '$auto'"'; done; done;echo "\$ns_ at 0 "'"$wcv_ subscribe"'; )|tee charging_request
