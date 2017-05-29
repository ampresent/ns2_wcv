#include <common/mobilenode.h>
#include "wcv_receiver.hh"

class WCVNode;

#ifndef __ns_wcv_h__
#define __ns_wcv_h__

class WCVReceiverApp;

// The sensor to be charged
// Suppose WCV and sensor work together on WCVHandler 
// Like an simplified end-to-end protocol
// So sensor appearing here can be explained
class WCVHandler : public Handler{
public:
	WCVHandler(WCVNode* n, WCVReceiverApp* a, MobileNode* s, double rm) : node(n), app(a), charging(false), sensor(s), remaining(rm) { }
	void handle(Event*);
private:
	WCVNode* node;
	WCVReceiverApp* app;
	MobileNode* sensor;
	bool charging;
	double remaining;
	Event charged_;
};


class WCVNode : public MobileNode {
	friend class WCVHandler;
	// Remember WCVNode is just an agent, can't have any member varible
	public:
		int set_destination(double x, double y, double s, WCVHandler* wcv_handler);
		void recv(WCVHandler* wcv_handler);
		void giveup_sched(WCVHandler* wcv_handler);
};

#endif // ns_wcv_h
