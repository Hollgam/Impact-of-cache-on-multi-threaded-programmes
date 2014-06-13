/*
 ============================================================================================
 Name        : test.h
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

 Description : The header for the main file test.c
 ============================================================================================
 */
#ifndef TEST_1_H_
#define TEST_1_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
// Custom includes
#include "hr_timer.h"
#include "conf.h"
#include "file_worker.h"
#include "experiments.h" // The actual experiments.
#include "test_env.h" // The testing environment.

// Process ID
#ifndef __APPLE__
	#include <sys/types.h>
	#include <unistd.h>
#endif

int main(int argc, char *argv[]);
void run_tests(int argc, char *argv[]); // Set up environment and run experiments.
int set_highest_process_priority(void); // Set priority of the current to be the highest
unsigned long long ** makeMatrixUnsignedLonglong(int x, int y);
void initaliseMatrixUnsignedLonglongWithZeros(unsigned long long ** matrix, int x, int y);

#endif /* TEST_1_H_ */
