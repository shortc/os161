/*
 * Copyright (c) 2001, 2002, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Driver code for whale mating problem
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <test.h>
#include <synch.h>

#define NMATING 10

static struct cv *testcvs[NMATING];
static struct lock *testlocks[NMATING];


static
void
male(void *p, unsigned long which)
{
	(void)p;
	kprintf("male whale #%ld starting\n", which);
	// Implement this function
}

static
void
female(void *p, unsigned long which)
{
	(void)p;
	lock_acquire(testlocks[1]);
	kprintf("female whale #%ld starting\n", which);

	// Implement this function
	lock_release(testlocks[1]);
}

static
void
matchmaker(void *p, unsigned long which)
{
	(void)p;
	
	lock_acquire(testlocks[0]);
	kprintf("matchmaker whale #%ld starting\n", which);

	// Implement this function


	lock_release(testlocks[0]);
}


// Change this function as necessary
int
whalemating(int nargs, char **args)
{

	int i, j, err=0;
	

	
	kprintf("nargs is %d\n", nargs);

	testcvs[0] = cv_create("wow");
	testlocks[0] = lock_create("crazy");

	testlocks[1] = lock_create("cray");
	
	
	(void)nargs;
	(void)args;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < NMATING; j++) {
			switch(i) {
			    case 0:
				
				err = thread_fork("Male Whale Thread",
						  NULL, male, NULL, j);
				break;
			    case 1:
				err = thread_fork("Female Whale Thread",
						  NULL, female, NULL, j);
				break;
			    case 2:
				err = thread_fork("Matchmaker Whale Thread",
						  NULL, matchmaker, NULL, j);
				break;
			}
			if (err) {
				panic("whalemating: thread_fork failed: %s)\n",
				      strerror(err));
			}
		}
	}

	return 0;
}
