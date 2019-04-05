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

static struct cv *cvs[NMATING];
static struct lock *locks[NMATING];
int *ready_males; 
int *ready_females; 
int *male_indices[NMATING];
int *female_indices[NMATING];
int *male_index;
int *female_index;
int *matchmaker_index;

static
void
male(void *p, unsigned long which)
{
	(void)p;

	lock_acquire(locks[0]);
	kprintf(">>> male whale #%ld starting\n", which);


	ready_males++;
	male_indices[male_index] = which;
	male_index++;

	cv_signal(cvs[2], locks[0]);
	cv_wait(cvs[0], locks[0]);
	kprintf("*** male whale #%ld mating\n", which);
	ready_males--;

	kprintf("<<< male whale #%ld exiting\n", which);
	lock_release(locks[0]);

}

static
void
female(void *p, unsigned long which)
{
	(void)p;

	lock_acquire(locks[0]);
	kprintf(">>> female whale #%ld starting\n", which);


	ready_females++;
	female_indices[female_index] = which;
	female_index++;

	cv_signal(cvs[2], locks[0]);
	cv_wait(cvs[1], locks[0]);
	kprintf("*** female whale #%ld mating\n", which);
	ready_females--;

	kprintf("<<< female whale #%ld exiting\n", which);
	lock_release(locks[0]);

}

static
void
matchmaker(void *p, unsigned long which)
{
	(void)p;
	kprintf(">>> matchmaker whale #%ld starting\n", which);
	
	lock_acquire(locks[0]);


	while(ready_males == 0 || ready_females == 0) {
		cv_wait(cvs[2], locks[0]);
	}
	

	matchmaker_index++;
	cv_signal(cvs[0], locks[0]);
	cv_signal(cvs[1], locks[0]);
	
	kprintf("*** matchmaker whale #%ld helping #%d and #%d\n", which, male_indices[matchmaker_index], female_indices[matchmaker_index]);

	kprintf("!!! Mating done!\n");
	
	kprintf("<<< matchmaker whale #%ld exiting\n", which);
	lock_release(locks[0]);

}


// Change this function as necessary
int
whalemating(int nargs, char **args)
{

	int i, j, err=0;
	

	
	kprintf("nargs is %d\n", nargs);

	cvs[0] = cv_create("male");
	cvs[1] = cv_create("female");
	cvs[2] = cv_create("matchmaker");

	locks[0] = lock_create("crazy");
	
	ready_males = 0;
	ready_females = 0;
	
	male_index = 0;
	female_index = 0;
	matchmaker_index = 0;
	
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
