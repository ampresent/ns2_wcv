#include <common/mobilenode.h>
#include "2pp_ping_receiver.hh"

class WCVNode;

#ifndef __ns_wcv_h__
#define __ns_wcv_h__

class TPPPingReceiverApp;

class WCVHandler : public Handler{
public:
	WCVHandler(WCVNode* n, TPPPingReceiverApp* a) : node(n), app(a){}
	void handle(Event*);
private:
	WCVNode* node;
	TPPPingReceiverApp* app;
};


class WCVNode : public MobileNode {
	friend class WCVHandler;
	// Remember WCVNode is just an agent, can't have any member varible
	public:
		//void update_position();
		int set_destination(double x, double y, double s, WCVHandler* wcv_handler);
		bool is_moving() {
			return pos_intr_.uid_ > 0;
		}

};

#endif // ns_wcv_h
