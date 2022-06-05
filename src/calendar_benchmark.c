/* -*- C++ -*- */
/*******************************************************
 C5 Project extension - CALENDAR PUT/GET BENCHMARK
 *******************************************************/

#include "calendar_benchmark.h"

#include <ctime>
#include <typeinfo>

#include "easyio.h"
#include "stat.h"
#include "calendar.h"
#include "heap_calendar.h"
#include "linked_calendar.h"
#include "rand.h"
#include "event.h"

event* generate_event() {
	double t;
	GEN_UNIF(SEED, 0, 10000, t);
	return new event(t);
}

void calendar_benchmark() {
	Sstat *bench_put_time = new Sstat;
	Sstat *bench_get_time = new Sstat;

	calendar *cals[] = { new heap_calendar(), new linked_calendar() };

	printf("\nBENCHMARK PARAMETERS:\n");
	int Trslen = read_int(" Benchmark setup length (# of ops)", 1000, 100,
			100000);
	int Runlen = read_int(" Benchmark RUN length (# of ops)", 10000, 100,
			100000);
	int Nrun = read_int(" Number of RUNs", 5, 2, 100);

	printf("*********************************************\n");
	printf("           BENCHMARK RESULTS                \n");
	printf("*********************************************\n\n");
	printf("Input parameters:\n");
	printf("Setup length            	%12d\n", Trslen);
	printf("Run length                  %12d\n", Runlen);
	printf("Number of runs              %12d\n", Nrun);
	printf("Results:\n");

	for (calendar *cal : cals) {
		bench_put_time->reset();
		bench_get_time->reset();

		for (int i = 0; i < Trslen; i++) {
			cal->put(generate_event());
		}

		for (int i = 0; i < Nrun; i++) {
			clock_t begin = clock();
			for (int j = 0; j < Runlen; j++) {
				cal->put(generate_event());
			}
			clock_t end = clock();
			*bench_put_time += 1000 * (double) (end - begin) / CLOCKS_PER_SEC;
		}

		for (int i = 0; i < Nrun; i++) {
			clock_t begin = clock();
			for (int j = 0; j < Runlen; j++)
				cal->get();
			clock_t end = clock();
			*bench_get_time += 1000 * (double) (end - begin) / CLOCKS_PER_SEC;
		}

		printf(" # %s\n", typeid(*cal).name());
		printf(
				"    - Average put() Time (ms)       %12.6f   +/- %.2e  p:%6.2f\n",
				bench_put_time->mean(), bench_put_time->confidence(.95),
				bench_put_time->confpercerr(.95));
		printf(
				"    - Average get() Time (ms)       %12.6f   +/- %.2e  p:%6.2f\n",
				bench_get_time->mean(), bench_get_time->confidence(.95),
				bench_get_time->confpercerr(.95));

	}
}

