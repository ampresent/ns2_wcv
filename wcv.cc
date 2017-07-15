#include <diffusion3/lib/main/tools.hh>
#include "wcv.h"
#include "mobilenode.h"
#include "mac/wireless-phy.h"

// A multiplexing handler for many states

void WCVHandler::handle(Event* e) {
	EnergyModel *em = NULL;
	if (sensor) {
       		em = sensor -> energy_model();
	}
	Scheduler& s = Scheduler::instance();
	double charging_time, to_charge, wcv_energy, wcv_initial_energy, get_back, reserve, base_energy, idle;
	States state = app -> get_state();
	if (state == MOVING || state == REACHED) {
		assert(em != NULL && sensor != NULL);
		to_charge = em -> initialenergy() - em -> energy();
		wcv_energy = node -> energy_model() -> energy();
		get_back = node->moving_energy(node->distance(app->base));
		// Prepare double the get_back energy in case
		reserve = get_back;
		idle = node -> idle_energy(charging_time);
		assert(wcv_energy >= get_back + reserve);
		if (wcv_energy - get_back - reserve - idle < to_charge / CHARGING_EFFICIENCY) {
			to_charge = (wcv_energy - get_back - reserve - idle) * CHARGING_EFFICIENCY;
		}
		charging_time = to_charge / CHARGING_SPEED;
	}

	switch (app -> get_state()) {
	case MOVING:
		// Moving done. Reached destination. 
		// Give up charging if noticing not enough power when reached destination.
		if (to_charge < EPS) {
			app -> set_state(SCHEDULING);
			app -> schedule();
		} else {
			//sensor -> energy_model() -> node_on_ = false;
			fprintf(stderr, "Node %d has energy %f J now\n", sensor -> address(), sensor -> energy_model() -> energy());
			struct timeval tv;
			GetTime(&tv);
			fprintf(stderr, "%ld.%06ld : ", tv.tv_sec, (long int) tv.tv_usec);
			fflush(stderr);

			app -> set_state(REACHED);
			s.schedule(this, &charged_, charging_time);
			break;
		}
	case REACHED:
		// Charging done
		if (to_charge > EPS) {
			node -> energy_model() -> DecrTxEnergy(charging_time, CHARGING_SPEED / CHARGING_EFFICIENCY);
			em -> DecrRcvEnergy(charging_time, -CHARGING_SPEED);
			if (em -> energy() > em -> initialenergy()) {
				em -> setenergy(em->initialenergy());
			}
		}
		fprintf(stderr, "Node %d has energy %f J now\n", sensor -> address(), sensor -> energy_model() -> energy());
		app -> ack();
		app -> set_state(CHARGED);
		fprintf(stderr, "WCVHandler::handle()\n");
		fflush(stderr);
		app -> run();
		break;
	case RECEIVING:
		// Retry 
		app -> set_state(SCHEDULING);
		app -> schedule();
		break;
	case BACK:
		// Reached base
		base_energy = app -> base -> energy_model() -> energy();
		wcv_energy = node -> energy_model() -> energy();
		wcv_initial_energy = node -> energy_model() -> initialenergy();
		to_charge = wcv_initial_energy - wcv_energy;
		charging_time = to_charge / WIRED_CHARGING_SPEED;
		assert(base_energy >= to_charge / WIRED_CHARGING_EFFICIENCY);
		// Charging_energy
		node -> energy_model() -> DecrRcvEnergy(charging_time, -WIRED_CHARGING_SPEED);
		app -> base -> energy_model() -> DecrTxEnergy(charging_time, WIRED_CHARGING_SPEED / WIRED_CHARGING_EFFICIENCY);
		app -> set_state(CHARGED);
		app -> run();
		break;
	default:
		fprintf(stderr, "Exit unexpectedly!!!\n");
		fflush(stderr);
		exit(1);
	}
}

double WCVNode::moving_energy(double dist) {
	assert(ifhead() != NULL && ifhead().next_node() == NULL);
	WirelessPhy* phy = static_cast<WirelessPhy*>(ifhead().lh_first);
	return WCV_POWER_PER_M * dist + phy -> P_idle() * (dist / MOVING_SPEED);
}

double WCVNode::idle_energy(double time) {
	assert(ifhead() != NULL && ifhead().next_node() == NULL);
	WirelessPhy* phy = static_cast<WirelessPhy*>(ifhead().lh_first);
	return  phy -> P_idle() * time;
}
	
int WCVNode::set_destination(double x, double y, double speed, WCVHandler* wcv_handler) {
	double dist = sqrt((x-X_)*(x-X_)+(y-Y_)*(y-Y_));
	fprintf(stderr, "Heading destination (%lf, %lf) at speed %lf with distance %lf\n", x, y, speed, dist);
	// Too close
	if (dist < 0.15) {
		return -1;
	}
	int ret = MobileNode::set_destination(x, y, speed);
	if (ret) {
		return ret;
	}
	struct timeval tv;
	GetTime(&tv);
	fprintf(stderr, "%ld.%06ld : ", tv.tv_sec, (long int) tv.tv_usec);
	Scheduler& s = Scheduler::instance();
	fprintf(stderr, "uid: %d\n", pos_intr_.uid_);
	s.schedule(wcv_handler, &pos_intr_, sqrt((destX_-X_)*(destX_-X_)+(destY_-Y_)*(destY_-Y_))/speed_);
	return 0;
}

void WCVNode::recv(WCVHandler* wcv_handler) {
	Scheduler& s = Scheduler::instance();
	s.schedule(wcv_handler, &pos_intr_, RECV_PERIOD);
}

void WCVNode::giveup_sched(WCVHandler* wcv_handler) {
	Scheduler& s = Scheduler::instance();
	s.schedule(wcv_handler, &pos_intr_, RECV_PERIOD);
}


map<int, OPPPingSenderApp*> WCVNode::node2app;

void WCVNode::add2map(int node_id, OPPPingSenderApp* app) {
	WCVNode::node2app[node_id] = app;
}

