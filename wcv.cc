#include <diffusion3/lib/main/tools.hh>
#include "wcv.h"
#include "mobilenode.h"

void WCVHandler::handle(Event* e) {
	// I seems pos_intr_ doesn't work at all. The two handler doesn't bind to the pos_intr_ at all.
	//PositionHandler::handle(e);
	
	struct timeval tv;
	GetTime(&tv);
	fprintf(stderr, "%ld.%06ld : ", tv.tv_sec, (long int) tv.tv_usec);
	fprintf(stderr, "Reached target\n");
	fflush(stderr);

	app -> subscribe();
}

int WCVNode::set_destination(double x, double y, double speed, WCVHandler* wcv_handler) {
	int ret = MobileNode::set_destination(x, y, speed);
	struct timeval tv;
	GetTime(&tv);
	fprintf(stderr, "%ld.%06ld : ", tv.tv_sec, (long int) tv.tv_usec);
	fprintf(stderr, "Heading destination (%lf, %lf) at speed %lf\n", destX_, destY_, speed_);
	Scheduler& s = Scheduler::instance();
	fprintf(stderr, "uid: %d\n", pos_intr_.uid_);
	s.schedule(wcv_handler, &pos_intr_, sqrt((destX_-X_)*(destX_-X_)+(destY_-Y_)*(destY_-Y_))/speed_);
	return ret;
}
