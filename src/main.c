/* -*- C++ -*- */
#include <stdio.h>
#include "global.h"
#include "easyio.h"
#include "simulator.h"
#include "queue.h"
#include "network_simulator.h"

int main(int argc, char *argv[]) {
	simulator *sim;

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("**********************************************************\n\n");
	printf("              		SimQueue \n\n");
	printf("**********************************************************\n\n");

	printf("\nSIMULATORS:\n");
	printf(" 1 - M/M/1 QUEUE SIMULATION\n");
	printf(" 2 - NETWORK SIMULATION\n");
	int choice = read_digit(" Your choice", 2, 1, 2);

	switch (choice) {
	case 1:
		sim = new queue(argc, argv);
		break;
	case 2:
	default:
		sim = new network_simulator(argc, argv);
		break;
	}

	sim->init();
	sim->run();
	sim->results();
	delete sim;

	return EXIT_SUCCESS;
}
