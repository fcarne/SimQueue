/***********************************************************************
 EVENT.C
 ***********************************************************************/

#include "event.h"
#include "buffer.h"
#include "calendar.h"
#include "rand.h"
#include "network_graph.h"

extern calendar *cal;
extern double inter;
extern double duration;

void arrival::body() {
	event *ev;

	// generation of next arrival
	double esito;
	GEN_EXP(SEED, inter, esito);
	ev = new arrival(time + esito, buf);
	cal->put(ev);

	// insert the new packet in the queue
	packet *pack = new packet(time);
	// if some packet is already in the buffer, just insert the new one 
	if (buf->full() || buf->status) {
		buf->insert(pack);
	}
	// otherwise let the packet get in the service
	else {
		buf->tot_packs += 1.0;
		delete pack;
		GEN_EXP(SEED, duration, esito);
		ev = new service(time + esito, buf);
		cal->put(ev);
		buf->status = 1;
	}
}

void service::body() {
	// printf("ingresso             %f\n", time);
	packet *pack;
	pack = buf->get();
	if (pack != NULL) {
		double esito;
		GEN_EXP(SEED, duration, esito);

		event *ev = new service(time + esito, buf);
		cal->put(ev);
		buf->tot_delay += time - pack->get_time();
		// printf("%3.5f    %3.5f\n", time, time-pack->get_time());
		buf->tot_packs += 1.0;
		delete pack;
	} else
		buf->status = 0;
}

/***************************************************************************
 C5 Project extension - NETWORK EVENTS
 ***************************************************************************/

void network_arrival::body() {
	extern long L;
	event *ev;

	// generation of next arrival
	double delta;
	GEN_EXP(SEED, inter, delta);
	ev = new network_arrival(time + delta, buf);
	cal->put(ev);

	int length;
	GEN_EXP_INT(SEED, L, length);

	// insert the new packet in the queue
	network_packet *pack = new network_packet(time, length);
	// if some packet is already in the buffer, just insert the new one
	if (buf->full() || buf->status) {
		buf->insert(pack);
	}
	// otherwise let the packet get in the service
	else {
		//buf->tot_packs += 1.0;
		double t_transfer = buf->get_transfer_time(pack->get_length());
		ev = new network_transfer(time + t_transfer, pack, buf);
		cal->put(ev);
		buf->status = 1;
	}
}

void network_transfer::body() {
	extern network_graph *network;

	// proceed with the transfer of this packet
	double u;
	GEN_UNIF(SEED, 0, 1, u);

	auto connected = buf->get_connected();
	int i = 0;
	double cumulated_prob = connected[0].prob;
	while (u > cumulated_prob) {
		++i;
		cumulated_prob += connected[i].prob;
	}

	network_buffer *next_buf = (network_buffer*) connected[i].buf;

	//pack->arrival_time = time;
	if (next_buf == network_buffer::OUT) {
		network->tot_packets += 1;
		network->tot_transfer += time - pack->get_time();

		delete pack;
	} else {
		if (next_buf->full() || next_buf->status) {
			next_buf->insert(pack);
		} else {
			//next_buf->tot_packs += 1.0;

			long l;
			extern long L;
			GEN_EXP_INT(SEED, L, l);

			double t_transfer = next_buf->get_transfer_time(pack->get_length());

			event *ev = new network_transfer(time + t_transfer, pack, next_buf);
			cal->put(ev);
			next_buf->status = 1;
		}
	}

	// transfer of the next packet in this buffer
	network_packet *next_pack;
	next_pack = (network_packet*) buf->get();
	if (next_pack != NULL) {

		long l;
		extern long L;
		GEN_EXP_INT(SEED, L, l);

		double t_transfer = buf->get_transfer_time(next_pack->get_length());
		event *ev = new network_transfer(time + t_transfer, next_pack, buf);
		cal->put(ev);
		//buf->tot_delay += time - next_pack->arrival_time;
		//buf->tot_packs += 1.0;
	} else
		buf->status = 0;
}
