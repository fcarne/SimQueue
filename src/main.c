/* -*- C++ -*- */
#include <stdio.h>
#include "global.h"
#include "easyio.h"
#include "simulator.h"
#include "queue.h"
#include "network_simulator.h"
#include "calendar_benchmark.h"

int main(int argc, char *argv[]) {
	simulator *sim;

	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("**********************************************************\n\n");
	printf("              		SimQueue \n\n");
	printf("**********************************************************\n\n");

	printf("\nSIMULATORS:\n");
	printf(" 1 - M/M/1 QUEUE SIMULATION\n");
	printf(" 2 - NETWORK SIMULATION\n");
	printf(" 3 - CALENDAR BENCHMARK\n");

	int choice = read_digit(" Your choice", 2, 1, 3);

	switch (choice) {
	case 1:
		sim = new queue(argc, argv);
		break;
	case 2:
		sim = new network_simulator(argc, argv);
		break;
	case 3:
		break;
	}

	if (choice != 3) {
		sim->init();
		sim->run();
		sim->results();
		delete sim;
	} else {
		calendar_benchmark();
	}

	return EXIT_SUCCESS;
}

