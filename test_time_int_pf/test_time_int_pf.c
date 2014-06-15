/*
 ============================================================================================
 Name        : test_time_int_pf.c
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

 Description: Measure costs of page faults and interrupts.
 Run instructions: compile with "gcc test_time_int_pf.c -o test -lrt && ./test"
 ============================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>
#include <cpuid.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

unsigned long long search_in_file(char *f, char *str, int find_numeric); // Search for a string str in the file fname
unsigned long long find_num_in_str(char *str); // Find a numeric in a string
int get_time_ns(struct timespec *timeStruct); // Get timestamp in nanoseconds
unsigned long long calculate_time_ns(struct timespec start, struct timespec end); // Calculate elapsed time, between start and end
struct proc_stats get_page_fault_file(); // Get a file with formation on recorded page faults
int read_stat(char * filename, int pid, struct proc_stats *s); // Read in contents of a stat file
unsigned long get_page_fault(struct proc_stats statsData, int choice); // Get a number of recorded page faults
unsigned long long average_time(unsigned long long *time, int timesRun); // Calculate average time of running experiment

#define BILLION  1000000000L // Definition of a billion

 /*
 * Struct for information from the stat file.
 * Based on code from Stephen Brown.
 */
struct proc_stats {
    int pid;			// %d
    char comm[256];		// %s
    char state;			// %c
    int ppid;			// %d
    int pgrp;			// %d
    int session;		// %d
    int tty_nr;			// %d
    int tpgid;			// %d
    unsigned long flags;	// %lu
    unsigned long long minflt;	// %lu
    unsigned long long cminflt;	// %lu
    unsigned long long majflt;	// %lu
    unsigned long long cmajflt;	// %lu
    unsigned long utime;	// %lu
    unsigned long stime; 	// %lu
    long cutime;		// %ld
    long cstime;		// %ld
    long priority;		// %ld
    long nice;			// %ld
    long num_threads;		// %ld
    long itrealvalue;		// %ld
    unsigned long starttime;	// %lu
    unsigned long vsize;	// %lu
    long rss;			// %ld
    unsigned long rlim;		// %lu
    unsigned long startcode;	// %lu
    unsigned long endcode;	// %lu
    unsigned long startstack;	// %lu
    unsigned long kstkesp;	// %lu
    unsigned long kstkeip;	// %lu
    unsigned long signal;	// %lu
    unsigned long blocked;	// %lu
    unsigned long sigignore;	// %lu
    unsigned long sigcatch;	// %lu
    unsigned long wchan;	// %lu
    unsigned long nswap;	// %lu
    unsigned long cnswap;	// %lu
    int exit_signal;		// %d
    int processor;		// %d
    unsigned long rt_priority;	// %lu
    unsigned long policy;	// %lu
    unsigned long long delayacct_blkio_ticks;	// %llu
};

/*
 * The main entry point of the programme.
 */
int main(int argc, const char ** argv) {
	int k;
	long sum; // SUm integers to avoid compiler optimisation
	struct timespec start, stop;
	int run = 10;

	// Record time run times
	unsigned long long *time = malloc(sizeof(unsigned long long) * run);// Record times of experiments in the run.
	if (time == NULL) {
		printf("Error with allocating space for the array\n");
		exit(1);
	}

	// Caculate the average duration of an interrupt
	printf("INTERRUPTS\ntime,num\n");
	int i = 0;
	for (i = 0; i < run; ++i) {
		unsigned long long interruptsBefore;
		unsigned long long interruptsAfter;

		// Warmup
		interruptsAfter = search_in_file("/proc/interrupts", "LOC:", 1);
		interruptsBefore = interruptsAfter;

		while (interruptsBefore == interruptsAfter)
			interruptsBefore = search_in_file("/proc/interrupts", "LOC:", 1);

		// Create interrupts
		do {
			get_time_ns(&start);// Record time before causing the interrupt
			interruptsAfter = search_in_file("/proc/interrupts", "LOC:", 1);
		} while (interruptsAfter - interruptsBefore != 1);

		get_time_ns(&stop); // Record time after causing the interrupt

		int numInterrupts = interruptsAfter - interruptsBefore;// How many interrupts occurred
		unsigned long long timeWithInterupts = calculate_time_ns(start, stop);// Record time with interrupts

		printf("%d,%llu\n", timeWithInterupts, numInterrupts);
		time[i] = timeWithInterupts / numInterrupts;// Record time difference over a number of interrupts
	}
	printf("1 interrupt takes (average from %d runs): %llu\n", run, average_time(time, run));

	// Caculate the average duration of a page fault
	printf("\nPAGE FAULTS\ntime,num\n");
	for (i = 0; i < run; ++i) {
		unsigned long long pfBefore;
		unsigned long long pfAfter;

		// Warmup
		struct proc_stats stat_file = get_page_fault_file();
		pfAfter = get_page_fault(stat_file, 1);
		pfBefore = pfAfter;

		while (pfBefore == pfAfter) {
			stat_file = get_page_fault_file();
			pfBefore = get_page_fault(stat_file, 1);
		}

		get_time_ns(&start);// Record time before causing a page fault

		// Create page faults
		do {
			stat_file = get_page_fault_file();
			pfAfter= get_page_fault(stat_file, 1);
		}while (pfAfter - pfBefore == 0);

		get_time_ns(&stop); // Record time after causing a page fault

		int numPf = pfAfter - pfBefore; // How many minor page faults occurred
		unsigned long long timeWithPf = calculate_time_ns(start, stop); // Record time with page faults

		printf("%d,%llu\n", timeWithPf, numPf);
		time[i] = timeWithPf / numPf; // Record time difference over a number of page faults
	}
	printf("1 page fault takes (average from %d runs): %llu\n", run, average_time(time, run));

	free(time);
	return sum;
}

/*
 * Search for substring *f in string *str.
 * find_numeric: find a numeric value in the result.
 */
unsigned long long search_in_file(char *f, char *str, int find_numeric) {
	FILE *fp;			// File pointer.
	char temp[512];		// Buffer for storing contents of a file.

	// Open file
	if ((fp = fopen(f, "r")) == NULL) {
		printf("Error: unable to open %s: %s\n", f, strerror(errno));
		return (-1);
	}

	// Search for str and extract numeric
	while (fgets(temp, 512, fp) != NULL) {
		if ((strstr(temp, str)) != NULL) {
			if (find_numeric) {
				unsigned long long number = find_num_in_str(temp);
				if (fp) {
					fclose(fp);
				}
				return number;
			} else {
				printf("A match found\n.");
				printf("\n%s\n", temp);
			}
		}
	}

	//Close the file if still open
	if (fp) {
		fclose(fp);
	}
	return (0);
}

/*
 * Find a numeric in a string
 */
unsigned long long find_num_in_str(char *str) {
	unsigned long long num = -1;	   // Support up to ull.
	sscanf(str, "%*[^0-9]%llu", &num); // Use regular expression.
	return num;
}

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
 * Read information from the /proc/stat file.
 */
struct proc_stats get_page_fault_file() {
	struct proc_stats statsData; 	// Struct to hold data from the /proc/pid/stat file
	int self = getpid(); 			// Process ID

	char buf[256]; // Buffer for reading in content of the file

	// Read data from the stats file
	sprintf(buf, "/proc/%d/stat", self);
	read_stat(buf, self, &statsData);

	return statsData;
}


/*
 * Structure for reading in information from the /proc/pid/stat file.
 * Based on code from Stephen Brown.
 */
int read_stat(char * filename, int pid, struct proc_stats *s) {
#ifndef __APPLE__ // Does not work on Mac OS
	const char *format = "%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %lu %lu %ld %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %d %d %lu %lu %llu";
	FILE *fp;

	fp = fopen(filename,"r");
	if (fp) {
		if (42==fscanf(fp, format,
						&s->pid,
						s->comm,
						&s->state,
						&s->ppid,
						&s->pgrp,
						&s->session,
						&s->tty_nr,
						&s->tpgid,
						&s->flags,
						&s->minflt,
						&s->cminflt,
						&s->majflt,
						&s->cmajflt,
						&s->utime,
						&s->stime,
						&s->cutime,
						&s->cstime,
						&s->priority,
						&s->nice,
						&s->num_threads,
						&s->itrealvalue,
						&s->starttime,
						&s->vsize,
						&s->rss,
						&s->rlim,
						&s->startcode,
						&s->endcode,
						&s->startstack,
						&s->kstkesp,
						&s->kstkeip,
						&s->signal,
						&s->blocked,
						&s->sigignore,
						&s->sigcatch,
						&s->wchan,
						&s->nswap,
						&s->cnswap,
						&s->exit_signal,
						&s->processor,
						&s->rt_priority,
						&s->policy,
						&s->delayacct_blkio_ticks
				)) {
			if (fp) {
				fclose(fp);
			}
			return 1;
		} else {
			if (fp) {
				fclose(fp);
			}
			return 0;
		}
	} else {
		return 0;
	}
#else
	return -1;
#endif
}

/*
 * Read counts of page faults from a proc_stats struct.
 * choice: 1 - minor, 2 - major
 */
unsigned long get_page_fault(struct proc_stats statsData, int choice) {
#ifndef __APPLE__
	if (choice == 1) {
		return statsData.minflt;
	} else {
		return statsData.majflt;
	}

#else
	return -1;
#endif
}

/*
 * Calculate the average time of running an experiment.
 */
unsigned long long average_time(unsigned long long *time, int timesRun) {
	if (timesRun == 0) // avoid division
		return 0;
	// Loop through times of all runs of the experiment
	int i;
	unsigned long long avTime = 0;
	for (i = 0; i < timesRun; ++i) {
		avTime += time[i];
	}
	return avTime / timesRun;
}