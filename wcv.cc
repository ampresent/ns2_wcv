#include <diffusion3/lib/main/tools.hh>
#include "wcv.h"
#include "mobilenode.h"

// A multiplexing handler for many states

void WCVHandler::handle(Event* e) {
	EnergyModel *em = NULL;
	if (sensor) {
       		em = sensor -> energy_model();
	}
	Scheduler& s = Scheduler::instance();
	double charging_time;

	switch (app -> get_state()) {
	case MOVING:
		charging_time = (em -> initialenergy() - remaining) / 0.3;
		app -> set_state(REACHED);
		//sensor -> energy_model() -> node_on_ = false;
		node -> energy_model() -> DecrTxEnergy(charging_time, 0.3);
		struct timeval tv;
		GetTime(&tv);
		fprintf(stderr, "%ld.%06ld : ", tv.tv_sec, (long int) tv.tv_usec);
		fprintf(stderr, "Target %d has energyc %f J now\n", sensor -> address(), sensor -> energy_model() -> energy());
		fflush(stderr);
		s.schedule(this, &charged_, charging_time);
		break;
	case REACHED:
		charging_time = (em -> initialenergy() - remaining) / 0.3;
		em -> DecrRcvEnergy(charging_time, -0.3);
		if (em -> energy() > em -> initialenergy()) {
			em -> setenergy(em->initialenergy());
		}
		app -> set_state(CHARGED);
		fprintf(stderr, "WCVHandler::handle()\n");
		fflush(stderr);
		app -> run();
		break;
	case RECEIVING:
		app -> set_state(SCHEDULING);
		app -> schedule();
		break;
	default:
		exit(1);
	}
}

int WCVNode::set_destination(double x, double y, double speed, WCVHandler* wcv_handler) {
	double dist = sqrt((x-X_)*(x-X_)+(y-Y_)*(y-Y_));
	fprintf(stderr, "Heading destination (%lf, %lf) at speed %lf with distance %lf\n", x, y, speed, dist);
	// Too close
	if (dist < 0.15) {
		return -1;
	}
	int ret = MobileNode::set_destination(x, y, speed);
	struct timeval tv;
	GetTime(&tv);
	fprintf(stderr, "%ld.%06ld : ", tv.tv_sec, (long int) tv.tv_usec);
	Scheduler& s = Scheduler::instance();
	fprintf(stderr, "uid: %d\n", pos_intr_.uid_);
	s.schedule(wcv_handler, &pos_intr_, sqrt((destX_-X_)*(destX_-X_)+(destY_-Y_)*(destY_-Y_))/speed_);
	return ret;
}

void  WCVNode::recv(WCVHandler* wcv_handler) {
	Scheduler& s = Scheduler::instance();
	s.schedule(wcv_handler, &pos_intr_, 4);
}

