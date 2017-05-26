#!/usr/bin/env zsh

echo > data_flow
echo > charging_request
echo > location

cat > location << EOF
\$node_(0) set X_ 0.0
\$node_(0) set Y_ 25.0
\$node_(0) set Z_ 0.000000000000
\$node_(1) set X_ 10.0
\$node_(1) set Y_ 10.0
\$node_(1) set Z_ 0.000000000000
\$node_(2) set X_ 5.0
\$node_(2) set Y_ 0.0
\$node_(2) set Z_ 0.000000000000
\$node_(3) set X_ 5.0
\$node_(3) set Y_ 5.0
\$node_(3) set Z_ 0.000000000000
\$node_(4) set X_ 0.0
\$node_(4) set Y_ 0.0
\$node_(4) set Z_ 0.000000000000
EOF


#N=30; (for i in `seq $N`; do for j in `seq 0 1`; do echo "\$ns_ at "$((i*(20.1/$N)+70)) '"$con_('$j') publish"'; done; echo "\$ns_ at "$(((i+1)*(20.1/$N)+70)) '"$snk_ subscribe"'; done; )|tee -a data_flow

N=200; (for i in `seq $N`; do for j in `seq 0 1`; do echo "\$ns_ at "$((i*(100.1/$N))) '"$con_('$j') publish"'; done; echo "\$ns_ at "$(((i+1)*(100.1/$N))) '"$snk_ subscribe"'; done; )|tee data_flow

N=40; (for i in `seq $N`; do for j in `seq 0 1`; do echo "\$ns_ at "$(((i+j)*(130.1/$N)+50)) '"$src_('$j') publish"'; done; echo "\$ns_ at "$(((i-1)*(130.1/$N)+50.1)) '"$wcv_ subscribe"'; done; )|tee charging_request



