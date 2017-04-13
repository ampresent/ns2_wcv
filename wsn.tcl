proc getopt {argc argv} {
	global opt
	lappend optlist nn
	for {set i 0} {$i < $argc} {incr i} {
		set opt($i) [lindex $argv $i]
	}
}

getopt $argc $argv

set opt(chan) Channel/WirelessChannel
set opt(prop) Propagation/TwoRayGround
set opt(netif) Phy/WirelessPhy

set opt(mac) Mac/802_11
if { $opt(0)=="DSR" } {
	set opt(ifq)	CMUPriQueue
} else {
	set opt(ifq)	Queue/DropTail/PriQueue
}

set opt(ll)	LL
set opt(ant)	Antenna/OmniAntenna

set opt(x)	500
set opt(y)	500
set opt(ifqlen)	2000
set opt(seed)	0.0

set opt(tr)	trace1.tr
set opt(adhocRouting) $opt(0)

set opt(nn)	100
set opt(cp)	"cbr_n100_m10_r10"
set opt(sc)	"scen_100n_100p_10M_100t_300_300"
set opt(stop)	100.0

Mac/802_11 set CWMin_			31
Mac/802_11 set CWMax_			1023
Mac/802_11 set SlotTime_		0.000020
Mac/802_11 set SIFS_			0.000010
Mac/802_11 set PreambleLength_		144
Mac/802_11 set PreambleDataRate_	1.0e6
Mac/802_11 set PLCPHeaderLength_	48
Mac/802_11 set PLCPDataRate_		1.0e6
Mac/802_11 set ShortRetryLimit_		7
Mac/802_11 set LongRetryLimit_		4

Mac/802_11 set dataRate_		2Mb
Mac/802_11 set basicRate_		1Mb

set ns_	[new Simulator]
set wtopo [new Topography]
set tracefd [open $opt(tr) w]
$ns_ trace-all $tracefd

$wtopo load_flatgrid $opt(x) $opt(y)
set god_ [create-god $opt(nn)]

set chan_1_ [new $opt(chan)]

$ns_ node-config -energyModel "EnergyModel" \
	-initialEnergy		1000 \
	-rxPower		10   \
	-txPower		10   \
	-idlePower		1    \
	-sleepPower		0.1  \
	-sleepTime		1    \
	-transitionPower	2    \
	-transitionTime		1    

$ns_ node-config -adhocRouting $opt(adhocRouting) \
	-llType $opt(ll)       \
	-macType $opt(mac)     \
	-ifqType $opt(ifq)     \
	-ifqLen $opt(ifqlen)   \
	-antType $opt(ant)     \
	-propType $opt(prop)   \
	-phyType $opt(netif)   \
	-channel $chan_1_      \
	-topoInstance $wtopo   \
	-agentTrace ON         \
	-routerTrace ON        \
	-macTrace OFF

for {set i 0} {$i < $opt(nn)} {incr i} {
	set node_($i) [$ns_ node]
	$node_($i) random-motion 0
}

puts "Loading connection pattern"
source $opt(cp)

puts "Loading scenario file..."
source $opt(sc)

for {set i 0} {$i < $opt(nn)} {incr i} {
	$ns_ initial_node_pos $node_($i) 20
}

for {set i 0} {$i < $opt(nn)} {incr i} {
	$ns_ at $opt(stop).1 "$node_($i) reset";
}

$ns_ at $opt(stop).1 "puts \"ns_ EXITING...\"; $ns_ halt"
puts "Starting Simulation..."
$ns_ run
