/***************************************************************************
 BUFFER.H
 ***************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

#include "packet.h"

class buffer {
	packet *head;
	packet *last;
public:
	int status;
	double tot_delay;
	double tot_packs;

	buffer();
	~buffer() {
	}

	void insert(packet *pack);
	packet* get();

	packet* full() {
		return head;
	}
};

/***************************************************************************
 C5 Project extension - NETWORK BUFFER
 ***************************************************************************/

class network_buffer;

struct edge {
	const network_buffer *buf;
	double prob;
};

class network_buffer: public buffer {
	long capacity;
	std::vector<edge> connected;
public:
	static const network_buffer *EXIT;

	network_buffer(long capacity) :
			capacity(capacity) {
	}

	void add_next(edge e) {
		connected.push_back(e);
	}

	const std::vector<edge>& get_connected() {
		return connected;
	}

	long get_capacity() {
		return capacity;
	}

	double get_transfer_time(long length) {
		double t_transfer = length / (double) capacity;
		return t_transfer;
	}

	bool is_valid() {
		double cumulative = 0.0;
		for (unsigned int i = 0; i < connected.size(); ++i) {
			cumulative += connected[i].prob;
		}

		return cumulative == 1.0;
	}
};

#endif
