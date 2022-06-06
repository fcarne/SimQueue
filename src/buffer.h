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
	const network_buffer *buf; // next buffer
	double prob; // probability of following this edge
};

class network_buffer: public buffer {
	long capacity;
	std::vector<edge> connected; // connections
public:
	// const pointer representing the rest of the world
	static const network_buffer *OUT;

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

	// computes the time necessary to a pack long length bits to be served by this buffer
	double get_transfer_time(long length) {
		return length / (double) capacity;
	}

	// checks if the cumulative probability of the connections is equal to 1
	bool is_valid() {
		double cumulative = 0.0;
		for (unsigned int i = 0; i < connected.size(); ++i) {
			cumulative += connected[i].prob;
		}

		return cumulative == 1.0;
	}
};

#endif
