/***************************************************************************
 PACKET.H
 ***************************************************************************/

#ifndef PACKET_H
#define PACKET_H

#include "global.h"

class packet {
	double gen_time;
public:
	packet(double Gen_time);
	virtual ~packet() {
	}
	packet *next;
public:
	double get_time() {
		return gen_time;
	}

};

inline packet::packet(double Gen_time) {
	gen_time = Gen_time;
	next = NULL;
}

/***************************************************************************
 C5 Project extension - NETWORK PACKET
 ***************************************************************************/

class network_packet: public packet {
	long length;
	//network_packet *next;
public:
	double arrival_time;
	network_packet(double gen_time, long length) :
			packet(gen_time), length(length) {
		arrival_time = gen_time;
	}

	long get_length() {
		return length;
	}

	double get_transfer_time(long capacity) {
		double t_transfer = length / (double) capacity;
		return t_transfer;
	}
};

#endif
