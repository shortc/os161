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


typedef struct whale_guts{
    struct cv *cvs[NMATING];
    struct lock *lock;
    int ready_males; 
    int ready_females; 
    int male_indices[NMATING];
    int female_indices[NMATING];
    int male_index;
    int female_index;
    int matchmaker_index;
} whale_guts;

static
void
male(void *p, unsigned long which)
{

    
	kprintf("hi\n");
    whale_guts *guts = (whale_guts *)p;

	kprintf("hi\n");
    lock_acquire(guts->lock);
	kprintf(">>> male whale #%ld starting\n", which);


	guts->ready_males++;
    guts->male_indices[guts->male_index] = which;
	guts->male_index++;

	cv_signal(guts->cvs[2], guts->lock);
	cv_wait(guts->cvs[0], guts->lock);
	kprintf("*** male whale #%ld mating\n", which);
	guts->ready_males--;

	kprintf("<<< male whale #%ld exiting\n", which);
	lock_release(guts->lock);

}

static
void
female(void *p, unsigned long which)
{
   
	kprintf("hi fe\n");
    whale_guts *guts = (whale_guts *)p;

	kprintf("hi fe\n");

    lock_acquire(guts->lock);
	kprintf(">>> female whale #%ld starting\n", which);


	guts->ready_females++;
	guts->female_indices[guts->female_index] = which;
	guts->female_index++;

	cv_signal(guts->cvs[2], guts->lock);
	cv_wait(guts->cvs[1], guts->lock);
	kprintf("*** female whale #%ld mating\n", which);
	guts->ready_females--;

	kprintf("<<< female whale #%ld exiting\n", which);
	lock_release(guts->lock);

}

static
void
matchmaker(void *p, unsigned long which)
{

	kprintf("hi mm\n");
    whale_guts *guts = (whale_guts *)p;
	kprintf("hi mm\n");
	
    kprintf(">>> matchmaker whale #%ld starting\n", which);
	
	lock_acquire(guts->lock);


	while(guts->ready_males == 0 || guts->ready_females == 0) {
		cv_wait(guts->cvs[2], guts->lock);
	}
	

	guts->matchmaker_index++;
	cv_signal(guts->cvs[0], guts->lock);
	cv_signal(guts->cvs[1], guts->lock);
	
	kprintf("*** matchmaker whale #%ld helping #%d and #%d\n", which, guts->male_indices[guts->matchmaker_index], guts->female_indices[guts->matchmaker_index]);

	kprintf("!!! Mating done!\n");
	
	kprintf("<<< matchmaker whale #%ld exiting\n", which);
	lock_release(guts->lock);

}


// Change this function as necessary
int
whalemating(int nargs, char **args)
{

	int i, j, err=0;


    whale_guts *guts = (whale_guts *) kmalloc(sizeof(whale_guts));


	
	kprintf("nargs is %d\n", nargs);

	guts->cvs[0] = cv_create("male");
	//guts->male_cv = cv_create("male");
    kprintf("male cv created\n");
	guts->cvs[1] = cv_create("female");
	guts->cvs[2] = cv_create("matchmaker");

	guts->lock = lock_create("crazy");
	
	guts->ready_males = 0;
	guts->ready_females = 0;
	
	guts->male_index = 0;
	guts->female_index = 0;
	guts->matchmaker_index = 0;
	
    kprintf("male cv created\n");
	(void)nargs;
	(void)args;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < NMATING; j++) {
			switch(i) {
			    case 0:
				
				err = thread_fork("Male Whale Thread",
						  NULL, male, (void *)&guts, j);
				break;
			    case 1:
				err = thread_fork("Female Whale Thread",
						  NULL, female, (void *)&guts, j);
				break;
			    case 2:
				err = thread_fork("Matchmaker Whale Thread",
						  NULL, matchmaker, (void *)&guts, j);
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
