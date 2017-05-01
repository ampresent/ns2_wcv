#include <diffusion3/lib/main/tools.hh>
#include "wcv.h"
#include "mobilenode.h"

void WCVHandler::handle(Event* e) {
	EnergyModel *em = sensor -> energy_model();
	double charging_time = (em -> initialenergy() - remaining) / 0.3;
	Scheduler& s = Scheduler::instance();
	if (charging) {
		// Charged
		
		charging = false;
		//em -> node_on_ = true;
		em -> DecrRcvEnergy(charging_time, -0.3);
		if (em -> energy() > em -> initialenergy()) {
			em -> setenergy(em->initialenergy());
		}
		app -> subscribe();
	} else {
		// Reached

		struct timeval tv;
		GetTime(&tv);
		fprintf(stderr, "%ld.%06ld : ", tv.tv_sec, (long int) tv.tv_usec);
		fprintf(stderr, "Reached target\n");
		fflush(stderr);

		charging = true;
		//sensor -> energy_model() -> node_on_ = false;

		// TODO: Use the correct charging model, and charing schedule
		node -> energy_model() -> DecrTxEnergy(charging_time, 0.3);
		fprintf(stderr, "uid: %d\n", charged_.uid_);
		s.schedule(this, &charged_, charging_time);
	}
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
