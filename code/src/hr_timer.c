/*
 ============================================================================================
 Name        : hr_timer.c
 Author      : Pavlo Bazilinskyy
 Version     : 1.0
 Copyright   : Copyright (c) 2014, Pavlo Bazilinskyy <pavlo.bazilinskyy@gmail.com>
 School of Computer Science, National University of Ireland, Maynooth

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 Description : Cross-platform high-resolution timer for performance measurements
 ============================================================================================
 */

#include "hr_timer.h"

/*
 * Get time in nanoseconds.
 */
int get_time_ns(struct timespec *timeStruct) {
	// Use CLOCK_MONOTONIC as it is more reliable
	if (clock_gettime( CLOCK_MONOTONIC, timeStruct) == -1) {
		perror("clock gettime");
		return 0;
	}
	return 1; // Return success
}

/*
 * Get resolution of the used timer.
 */
int get_time_res(struct timespec *timeStruct) {
#ifndef __APPLE__
	if( clock_getres( CLOCK_MONOTONIC, timeStruct) == -1 ) {
		perror( "clock getres" );
		return 0;
	}
	return 1;
#else // Does not work on Mac OS
	return 0;
#endif
}

/*
 * Calculate a difference in nanoseconds between start and end.
 */
unsigned long long calculate_time_ns(struct timespec start, struct timespec end) {
	struct timespec temp;
	if ((end.tv_nsec - start.tv_nsec) < 0) { // Wrap
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = BILLION + end.tv_nsec - start.tv_nsec;
	} else { // Everything is fine
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return BILLION * temp.tv_sec + temp.tv_nsec; // Convert seconds into nanoseconds
}

/*
 * Calculate the average time of running an experiment.
 */
unsigned long long average_time(unsigned long long *time, int timesRun) {
	if (timesRun == 0) // Avoid division by zero
		return 0;
	// Loop through times of all runs of the experiment
	int i;
	unsigned long long avTime = 0;
	for (i = 0; i < timesRun; ++i) {
		avTime += time[i];
	}
	return avTime / timesRun;
}

/*
 *  Calculate the minimum time of running the experiment.
 */
unsigned long long min_time(unsigned long long *time, int timesRun) {
	if (timesRun == 0) // Avoid division by zero
		return 0;
	// Loop through times of all runs of the experiment
	int i;
	unsigned long long minTime = INT_MAX; // Initialise the min time to be the max value for integers
	for (i = 0; i < timesRun; ++i) {
		if (minTime > time[i])
			minTime = time[i];
	}

	return minTime;
}

/*
 * Use RDTSC to measure time at nanosecond accuracy (if it is not disabled).
 * CPUID == 1 - use CPUID; CPUID == 0 - do not use CPUID.
 */
unsigned long long rdtsc_old(int CPUID) {
	unsigned long a, b;
	unsigned long long temp;
	if (CPUID) // Use inline assembly code
		__asm__ __volatile__("CPUID\nrdtsc" : "=a" (a), "=d" (b):: "memory", "%ebx", "%ecx");
	else
		__asm__ __volatile__("rdtsc" : "=a" (a), "=d" (b):: "memory", "%ebx", "%ecx");
	temp = b;
	temp = (temp << 32) | a;
	return temp;
}

// Use RDTSC
#if TIMING == RDTSC
/*
 * Use RDTSC to measure time at nanosecond accuracy (if it is not disabled).
 */
	uint64_t rdtsc() {
		uint32_t lo, hi;

		#ifdef USE_RDTSCP
		asm volatile (
				"RDTSCP\n\t"/*read the clock*/
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t"
				"CPUID\n\t": "=r" (hi), "=r"
				(lo):: "%rax", "%rbx", "%rcx", "%rdx");
		return (uint64_t) hi << 32 | lo;

		#else
		asm volatile (
				"CPUID\n\t"/*serialize*/
				"RDTSC\n\t"/*read the clock*/
				"mov %%edx, %0\n\t"
				"mov %%eax, %1\n\t": "=r" (hi), "=r"
				(lo):: "%rax", "%rbx", "%rcx", "%rdx");
		#endif

	}
#endif

/*
 * Test the clock_gettime(3) function.
 */
void test_clock_gettime(void) {
	struct timespec start1, start2, start3;
	get_time_ns(&start1);
	get_time_ns(&start2);
	get_time_ns(&start3);

	// Make initial recordings
	while (calculate_time_ns(start1, start2) == 0ull) {
		get_time_ns(&start2);
	}
	get_time_ns(&start3);
	while (calculate_time_ns(start2, start3) == 0ull) {
		get_time_ns(&start3);
	}

	// Estimate the duration of a call to clock_gettime(3).
	printf("TIME. Time: %ld %ld %ld.", BILLION * start1.tv_sec + start1.tv_nsec,
		BILLION * start2.tv_sec + start2.tv_nsec,
		BILLION * start3.tv_sec + start3.tv_nsec);
	printf(" Diff: %llu %llu\n", calculate_time_ns(start1, start2), calculate_time_ns(start2, start3)); // Show the difference in time.
}

/*
 * Test the clock_getres(3) function.
 */
void test_clock_getres(void) {
	struct timespec startres1;
	get_time_res(&startres1);
	printf("RESOLUTION. time: %ld\n", BILLION * startres1.tv_sec + startres1.tv_nsec);
}

/*
 * Test the rdtsc instruction (old version).
 */
void test_rdtsc(void) {
	// With CPUID
	printf("TEST OF RDTSC with CPUID\n");

	unsigned long long t[32], prev;
	int i;
	for (i = 0; i < 32; i++)
		t[i] = rdtsc_old(1);

	prev = t[0];
	for (i = 1; i < 32; i++) {
		printf("%llu [%llu]\n", t[i], t[i] - prev);
		prev = t[i];
	}

	printf("Total=%llu\n", t[32 - 1] - t[0]);

	// Without CPUID
	printf("\nTEST OF RDTSC without CPUID\n");

	for (i = 0; i < 32; i++)
		t[i] = rdtsc_old(0);

	prev = t[0];
	for (i = 1; i < 32; i++) {
		printf("%llu [%llu]\n", t[i], t[i] - prev);
		prev = t[i];
	}

	printf("Total=%llu\n", t[32 - 1] - t[0]);
}
