#include <diffusion3/lib/main/tools.hh>
#include "wcv.h"
#include "mobilenode.h"

void WCVHandler::handle(Event* e) {
	// I seems pos_intr_ doesn't work at all. The two handler doesn't bind to the pos_intr_ at all.
	//PositionHandler::handle(e);
	
	Scheduler& s = Scheduler::instance();

	struct timeval tv;
	GetTime(&tv);
	fprintf(stderr, "%ld.%06ld : ", tv.tv_sec, (long int) tv.tv_usec);
	fprintf(stderr, "Updating position\n");
	fflush(stderr);
	s.schedule(this, &node->pos_intr_, node->position_update_interval_);
}

int WCVNode::set_destination(double x, double y, double speed, WCVHandler* wcv_handler) {
	fprintf(stderr, "Setting destination\n");
	int ret = MobileNode::set_destination(x, y, speed);
	Scheduler& s = Scheduler::instance();
	s.schedule(wcv_handler, &pos_intr_, position_update_interval_);
	return ret;
}
