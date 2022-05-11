/*******************************************************
 EVENT . H
 *******************************************************/

#ifndef _EVENT_H
#define _EVENT_H

#include "global.h"
#include "buffer.h"

class event {
public:
	event *next;	// next event
	double time;	// event time

	event();
	event(double Time);
	event(event *Next, double Time);
	virtual ~event() {
	}

	virtual void body() = 0;
};

inline event::event() {
	next = NULL;
	time = -1;
}

inline event::event(event *Next, double Time) {
	next = Next;
	time = Time;
}

inline event::event(double Time) {
	next = NULL;
	time = Time;
}

class arrival: public event {
	buffer *buf;
public:
	arrival(double Time, buffer *Buf) :
			event(Time) {
		buf = Buf;
	}

	virtual void body();
};

class service: public event {
	buffer *buf;
public:
	service(double Time, buffer *Buf) :
			event(Time) {
		buf = Buf;
	}

	virtual void body();
};

/***************************************************************************
 C5 Project extension - NETWORK EVENTS
 ***************************************************************************/

class network_arrival: public event {
	network_buffer *buf;
public:
	network_arrival(double time, network_buffer *buf) :
			event(time), buf(buf) {
	}

	virtual void body();
};

class network_transfer: public event {
	network_packet *pack;
	network_buffer *buf;
public:
	network_transfer(double time, network_packet *pack, network_buffer *buf) :
			event(time), pack(pack), buf(buf) {
	}

	virtual void body();
};

#endif
