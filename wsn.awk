BEGIN{
	highest_packet_id=0;
	sends=0;
	receives=0;
	routing_packets=0;
	first_received_ime=0;
	first=0;
	system("rm energy_*");
}
{
	action=$1;
	# Send / Receive / Forward
	if (action=="s"||action=="r"||action=="f"){
		time=$2;
		packet_id=$6;
		trace=$4;
		type=$7;

		# AGT means agent
		if (action=="s" && trace=="AGT"&&type=="cbr")
		{sends++;}

		if (packet_id > highest_packet_id)
		{highest_packet_id=packet_id;}

		if (start_time[packet_id]==0)
		{start_time[packet_id]=time;}

		if (action=="r" && trace=="AGT" && type=="cbr")
		{
			if (first == 0) {
				first_received_time = time;
				first = 1;
			}
			receives++;
			end_time[packet_id] = time;
		}else 
			end_time[packet_id] = -1;
			# Forward ??? or RTR or MAC???
	} else if (action=="N" ) {
		t=$3;
		n=$5;
		e=$7;
		printf("%f %f\n", t, e) >> "energy_"n;
	}

}

END{
	for (packet_id=0;packet_id<=highest_packet_id;packet_id++) {
		# What if end time is -1 ??????????
		packet_duration=end_time[packet_id] - start_time[packet_id];
		# The sum of end to end delay
		if (packet_duration>0) end_to_end_delay += packet_duration;
	}

	avg_end_to_end_delay = end_to_end_delay / (receives);
	pdfraction = (receives/sends) * 100;

	printf("Total packet sends: %d\n", sends);
	printf("Total packet receives: %d\n", receives);
	printf("Packet delivery fraction: %s\n", pdfraction);
	printf("Average End-to-End delay:%f s\n", avg_end_to_end_delay);
	printf("first packet received time:%f s\n", first_received_time);

	system("./plot.sh 5")
}

