#include <common/mobilenode.h>
#include "wcv_receiver.hh"
#include "1pp_ping_sender.hh"

class WCVNode;

#ifndef __ns_wcv_h__
#define __ns_wcv_h__

class OPPPingSenderApp;
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
		static map<int, OPPPingSenderApp*> node2app;
		static map<int, int> statistics;
		int set_destination(double x, double y, double s, WCVHandler* wcv_handler);
		void recv(WCVHandler* wcv_handler);
		void giveup_sched(WCVHandler* wcv_handler);
		static void add2map(int node_id, OPPPingSenderApp* app);
};

#endif // ns_wcv_h
