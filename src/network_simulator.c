/* -*- C++ -*- */
/*******************************************************
 QUEUE.C
 *******************************************************/
#include "global.h"
#include <stdio.h>
#include "network_simulator.h"
#include "rand.h"
#include "buffer.h"
#include "event.h"
#include "calendar.h"
#include "calendar_heap.h"
#include "easyio.h"

extern calendar *cal;		// events calendar
extern double inter;
extern double Trslen;
extern double Runlen;
extern int NRUNmin;

long L;
network_graph *network;
double TIME_START = 0.0;

network_simulator::network_simulator(int argc, char *argv[]) :
		simulator(argc, argv) {
	cal = new calendar_heap();

	network = NULL;

	delay = new Sstat();
	transfer_time = new Sstat();
	execution_time = new Sstat();

	traffic_model = service_model = 0;
}

network_simulator::~network_simulator() {
	delete delay;
	delete transfer_time;
	delete execution_time;

	delete network;
	delete cal;
}

void network_simulator::input() {
	printf("\nMODEL PARAMETERS:\n");
	printf(" Arrivals model:\n");
	printf("  1 - Poisson\n");
	traffic_model = read_digit("  Selected model", 1, 1, 1);
	inter = read_double(" Average inter-arrival time (s)", 0.4, 0.01, 100);
	printf("\n");
	printf(" Service model:\n");
	printf("  1 - Exponential\n");
	service_model = read_digit("  Selected model", 1, 1, 1);
	L = read_long(" Average packet length (bit)", 1024, 8, 65536);
	printf("\nSIMULATION PARAMETERS:\n");
	Trslen = read_double(" Simulation transient len (s)", 100, 0.01, 10000);
	Runlen = read_double(" Simulation RUN len (s)", 100, 0.01, 10000);
	NRUNmin = read_int(" Simulation number of RUNs", 5, 2, 100);

	printf("\nNETWORK TOPOLOGY:\n");
	auto filename = read_string(" Network topology file path");
	network = network_graph::parse(filename);
}

void network_simulator::init() {
	input();
	auto entries = network->get_entry_buffers();
	for (auto b : entries) {
		event *ev = new network_arrival(TIME_START, b);
		cal->put(ev);
	}
}

void network_simulator::run() {

	extern double Trslen;
	extern double Runlen;
	extern int NRUNmin;

	double clock = TIME_START;
	event *ev;

	while (clock < Trslen) {
		ev = cal->get();
		ev->body();
		clock = ev->time;
		delete ev;
	}
	clear_stats();
	clear_counters();

	int current_run_number = 1;
	while (current_run_number <= NRUNmin) {
		while (clock < (current_run_number * Runlen + Trslen)) {
			ev = cal->get();
			ev->body();
			clock = ev->time;
			delete (ev);
		}
		update_stats();
		clear_counters();
		print_trace(current_run_number);
		current_run_number++;
	}
}

void network_simulator::results() {
	extern double Trslen;
	extern double Runlen;
	extern int NRUNmin;

	fprintf(fpout, "*********************************************\n");
	fprintf(fpout, "           SIMULATION RESULTS                \n");
	fprintf(fpout, "*********************************************\n\n");
	fprintf(fpout, "Input parameters:\n");
	fprintf(fpout, "Transient length (s)         %5.3f\n", Trslen);
	fprintf(fpout, "Run length (s)               %5.3f\n", Runlen);
	fprintf(fpout, "Number of runs               %5d\n", NRUNmin);
	fprintf(fpout, "Packet inter-arrival time	 %5.3f\n", inter);
	fprintf(fpout, "Average packet length		 %5ld\n", L);
	fprintf(fpout, "Results:\n");
	fprintf(fpout, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n",
			delay->mean(), delay->confidence(.95), delay->confpercerr(.95));
	fprintf(fpout, "Average Transfer Time        %4.6f   +/- %.2e  p:%3.2f\n",
			transfer_time->mean(), transfer_time->confidence(.95),
			transfer_time->confpercerr(.95));
	//TODO: add theoretical results
}

void network_simulator::print_trace(int n) {
	fprintf(fptrc, "*********************************************\n");
	fprintf(fptrc, "                 TRACE RUN %d                \n", n);
	fprintf(fptrc, "*********************************************\n\n");

	fprintf(fptrc, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n",
			delay->mean(), delay->confidence(.95), delay->confpercerr(.95));
	fprintf(fptrc, "Average Transfer Time        %4.6f   +/- %.2e  p:%3.2f\n",
			transfer_time->mean(), transfer_time->confidence(.95),
			transfer_time->confpercerr(.95));

	fflush(fptrc);
}

void network_simulator::clear_counters() {
	auto buffers = network->get_buffers();
	for (auto b : buffers) {
		b->tot_delay = 0;
		b->tot_packs = 0;
	}

	network->tot_packets = 0;
	network->tot_transfer = 0;
}

void network_simulator::clear_stats() {
	delay->reset();
	transfer_time->reset();
	execution_time->reset();
}

void network_simulator::update_stats() {
	auto buffers = network->get_buffers();
	for (auto b : buffers) {
		if (b->tot_packs > 0)
			*delay += b->tot_delay / b->tot_packs;
	}

	auto exit_connected_buffers = network->get_exit_connected_buffers();
	if (network->tot_packets > 0)
		*transfer_time += network->tot_transfer / network->tot_packets;
}

