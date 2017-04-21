#include <common/mobilenode.h>
#include "2pp_ping_receiver.hh"

class WCVNode;

#ifndef __ns_wcv_h__
#define __ns_wcv_h__

class WCVHandler : public Handler{
public:
	WCVHandler(WCVNode* n) : node(n){}
	void handle(Event*);
private:
	WCVNode *node;
};


class WCVNode : public MobileNode {
	friend class WCVHandler;
	/*
	private:
		TPPPingReceiverApp * wcvapp;
		*/
	public:
		//void update_position();
		int set_destination(double x, double y, double s, WCVHandler* wcv_handler);
};

#endif // ns_wcv_h
