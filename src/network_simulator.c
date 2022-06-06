/* -*- C++ -*- */
/*******************************************************
  C5 Project extension - NETWORK_SIMULATOR.C
 *******************************************************/
#include <stdio.h>
#include <filesystem>
#include <ctime>

#include "global.h"
#include "network_simulator.h"
#include "rand.h"
#include "buffer.h"
#include "event.h"
#include "calendar.h"
#include "easyio.h"
#include "heap_calendar.h"
#include "network_graph.h"

extern calendar *cal;		// events calendar
extern double inter;
extern double Trslen;
extern double Runlen;
extern int NRUNmin;

long L; // mean length of a packet
network_graph *network; // the network to simulate
double TIME_START = 0.0;

network_simulator::network_simulator(int argc, char *argv[]) :
		simulator(argc, argv) {
	cal = new heap_calendar();

	network = NULL;

	transfer_time = new Sstat();
	execution_time = new Sstat();

	traffic_model = service_model = 0;
	gamma = 0;
	run_sim_time = 0.0;
}

network_simulator::~network_simulator() {
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
	gamma = read_double(" Offered traffic per node (pck/s)", 1, 0.01, 1000);
	inter = 1 / gamma;
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
	auto filename = read_string(" Network topology file path",
			"./topology.txt");
	network = network_graph::parse(filename);
}

void network_simulator::init() {
	input();
	// add an arrival event for each of the ingress buffers
	auto ingress = network->get_ingress_buffers();
	for (auto b : ingress) {
		event *ev = new network_arrival(TIME_START, b);
		cal->put(ev);
	}
}

void network_simulator::run() {

	extern double Trslen;
	extern double Runlen;
	extern int NRUNmin;

	double clock_time = TIME_START;
	event *ev;

	while (clock_time < Trslen) {
		ev = cal->get();
		ev->body();
		clock_time = ev->time;
		delete ev;
	}
	clear_stats();
	clear_counters();

	int current_run_number = 1;
	while (current_run_number <= NRUNmin) {
		clock_t begin = clock();

		while (clock_time < (current_run_number * Runlen + Trslen)) {
			ev = cal->get();
			ev->body();
			clock_time = ev->time;
			delete ev;
		}

		clock_t end = clock();
		run_sim_time = 1000 * (double) (end - begin) / CLOCKS_PER_SEC;

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
	fprintf(fpout, "Transient length (s)        %12.3f\n", Trslen);
	fprintf(fpout, "Run length (s)              %12.3f\n", Runlen);
	fprintf(fpout, "Number of runs              %12d\n", NRUNmin);
	fprintf(fpout, "Offered traffic per node    %12.3f\n", gamma);
	fprintf(fpout, "Average packet length       %12ld\n", L);
	fprintf(fpout, "Results:\n");
	fprintf(fpout, "Average Transfer Time       %12.6f   +/- %.2e  p:%6.2f\n",
			transfer_time->mean(), transfer_time->confidence(.95),
			transfer_time->confpercerr(.95));
	fprintf(fpout, "Average Run Time (ms)       %12.6f   +/- %.2e  p:%6.2f\n",
				execution_time->mean(), execution_time->confidence(.95),
				execution_time->confpercerr(.95));


	// generate the .dat file for the matlab script
	std::filesystem::create_directory("./analysis");
	network->serialize("./analysis/q_matrix.dat", "./analysis/node_info.dat");
	std::filesystem::path script_path = std::filesystem::path("qos.m");

	fprintf(fpout,
			"\nTheoretical results can be computed using the MATLAB script at: \n%s\n",
			std::filesystem::absolute(script_path).c_str());
}

void network_simulator::print_trace(int n) {
	fprintf(fptrc, "*********************************************\n");
	fprintf(fptrc, "                 TRACE RUN %d                \n", n);
	fprintf(fptrc, "*********************************************\n\n");

	fprintf(fptrc, "Average Transfer Time       %12.6f   +/- %.2e  p:%6.2f\n",
			transfer_time->mean(), transfer_time->confidence(.95),
			transfer_time->confpercerr(.95));
	fprintf(fptrc, "Run Simulation Time (ms)    %12.6f\n", run_sim_time);
	fprintf(fptrc, "\n");
	fflush(fptrc);
}

void network_simulator::clear_counters() {
	/*auto buffers = network->get_buffers();
	for (auto b : buffers) {
		b->tot_delay = 0;
		b->tot_packs = 0;
	}*/

	network->tot_packets = 0;
	network->tot_transfer = 0;
}

void network_simulator::clear_stats() {
	transfer_time->reset();
	execution_time->reset();
}

void network_simulator::update_stats() {
	if (network->tot_packets > 0)
		*transfer_time += network->tot_transfer / network->tot_packets;

	*execution_time += run_sim_time;
}

