/*
 ============================================================================================
 Name        : test_env.h
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

 Description : The header for the testing environment.
 Target		 : MacBook Air with i7 and Xeon 5130
 ============================================================================================
 */

#ifndef TEST_ENV_H_
#define TEST_ENV_H_

#include <stdio.h>
#include <stdlib.h>

// Custom includes
#include "hr_timer.h"
#include "conf.h"
#include "file_worker.h"

void test_interrupt_time (void); // Record how much time one interrupt takes on the testing system
int warm_strings_with_files(void); // Create two copies of each string used for storing files to fill in memory with this data.
long calculate_n(long n);

#endif /* TEST_ENV_H_ */
