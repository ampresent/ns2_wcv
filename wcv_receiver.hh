//
// wcv_receiver.hh : Ping Receiver Include File
// author           : Fabio Silva
//
// Copyright (C) 2000-2002 by the University of Southern California
// $Id: wcv_receiver.hh,v 1.2 2005/09/13 04:53:46 tomh Exp $
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License,
// version 2, as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
//
// Linking this file statically or dynamically with other modules is making
// a combined work based on this file.  Thus, the terms and conditions of
// the GNU General Public License cover the whole combination.
//
// In addition, as a special exception, the copyright holders of this file
// give you permission to combine this file with free software programs or
// libraries that are released under the GNU LGPL and with code included in
// the standard release of ns-2 under the Apache 2.0 license or under
// otherwise-compatible licenses with advertising requirements (or modified
// versions of such code, with unchanged license).  You may copy and
// distribute such a system following the terms of the GNU GPL for this
// file and the licenses of the other code concerned, provided that you
// include the source code of that other code when and as the GNU GPL
// requires distribution of source code.
//
// Note that people who make modified versions of this file are not
// obligated to grant this special exception for their modified versions;
// it is their choice whether to do so.  The GNU General Public License
// gives permission to release a modified version without this exception;
// this exception also makes it possible to release a modified version
// which carries forward this exception.

#ifndef _WCV_RECEIVER_HH_
#define _WCV_RECEIVER_HH_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "ping.hh"
#include "wcv.h"

extern char * state_str[];

class WCVReceiverReceive;

class WCVReceiverApp : public DiffApp {
public:
#ifdef NS_DIFFUSION
  WCVReceiverApp();
  int command(int argc, const char*const* argv);
#else
  WCVReceiverApp(int argc, char **argv);
#endif // NS_DIFFUSION

  void recv(NRAttrVec *data, NR::handle my_handle);
  void run();
  void schedule();

  States get_state() {
          struct timeval tmv;
	  GetTime(&tmv);
          fprintf(stderr, "%ld.%06ld : ", tmv.tv_sec, (long int) tmv.tv_usec);
	  fprintf(stderr, "Get status %s\n", state_str[wcv_state]);
	  fflush(stderr);
	  return wcv_state;
  }
  void set_state(States s) {
          struct timeval tmv;
	  GetTime(&tmv);
          fprintf(stderr, "%ld.%06ld : ", tmv.tv_sec, (long int) tmv.tv_usec);
	  fprintf(stderr, "Set status %s\n", state_str[s]);
	  fflush(stderr);
	  wcv_state = s; 
  }
  void ack();

private:
  // NR Specific variables
  WCVReceiverReceive *mr_;
  handle subHandle_;
  struct request{
	  NR::handle handle;
	  float lat;
	  float lon;
	  double energy;
  };

  struct request req_queue[MODULER];
  unsigned front;
  unsigned rear;

  // Ping App variables
  int last_seq_recv_;
  int num_msg_recv_;
  int first_msg_recv_;

  handle setupSubscription();
  WCVHandler* wcv_handler;
  States wcv_state;
};

class WCVReceiverReceive : public NR::Callback {
public:
  WCVReceiverReceive(WCVReceiverApp *app) : app_(app) {};
  void recv(NRAttrVec *data, NR::handle my_handle);

  WCVReceiverApp *app_;
};

#endif // !_PING_RECEIVER_HH_
