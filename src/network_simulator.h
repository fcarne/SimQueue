/*******************************************************
 C5 Project extension - G/G/1 NETWORK SIMULATOR
 *******************************************************/
#ifndef _NETWORK_SIMULATOR_H
#define _NETWORK_SIMULATOR_H

#include "simulator.h"
#include "stat.h"

class network_simulator: public simulator {
	int traffic_model;
	double gamma;
	int service_model;
	// statistics
	double run_sim_time;
	Sstat *transfer_time;
	Sstat *execution_time;

	virtual void input(void);
public:
	network_simulator(int argc, char *argv[]);
	virtual ~network_simulator(void);

	virtual void init(void);
	virtual void run(void);
	virtual void results(void);
private:
	virtual void clear_counters(void);
	virtual void clear_stats(void);
	virtual void update_stats(void);
	virtual void print_trace(int Run);
};

#endif
