/*
 ============================================================================================
 Name        : conf.h
 Author      : Pavlo Bazilinskyy
 Version     : 0.1
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

 Description : Configuration of the experiment
 ============================================================================================
 */

#ifndef CONF_H_
#define CONF_H_

// Configure experiment
#define MAX_POWER 24
#define SET_HIGHEST_PRIORITY
#define WARM_CACHE
#define WARM_STRINGS_WITH_FILES
#define START_AFTER_TIMER_TICK
#define PROCESS_AFFINITY PIN_TO_ONE_CPU
#define PIN_TO_CPU 0
#define TIMES_RUN_EXPERIMENT 10
// Every time reads a file it generates 1
#define ALLOWED_INTERRUPTS 2
#define ALLOWED_CONTEXT_SWITCHES 0
#define ALLOWED_PAGEFAULTS_MINOR 5
#define ALLOWED_PAGEFAULTS_MAJOR 0

// Output
#define DEBUG
//#define DETAILED_DEBUG
#define SHOW_RESULTS
//#define OUTPUT_TO_FILE // Output measurements into a CSV file

#define BILLION  1000000000L
#define LONG_TO_ADD 1l
#define BIG_BUFFER_SIZE (8*1024) // For storying files as strings
#ifdef __APPLE__
	#define CSV_FILE_CLEAN "test_1_i7_clean.csv"
	#define CSV_FILE_DIRTY "test_1_i7_dirty.csv"
#else
	#define CSV_FILE_CLEAN "test_1_xeon_clean.csv"
	#define CSV_FILE_DIRTY "test_1_xeon_dirty.csv"
#endif
#endif /* CONF_H_ */
