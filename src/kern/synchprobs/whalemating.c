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

typedef struct {
    struct cv *mmstarting_cv;
    struct cv *mm_cv;
    struct cv *m_cv;
    struct cv *f_cv;

    // Used two bc if one then order might be off
    struct cv *fmating_cv;
    struct cv *mmating_cv;

    struct lock *lock;

    int is_first;
    int num_mating;

    int male_indices[NMATING];
    int female_indices[NMATING];
    int male_index;
    int female_index;
    int matchmaker_index;
} whale_guts;

static struct semaphore *sem;

static
void
male(void *p, unsigned long which)
{
    whale_guts *guts = (whale_guts *)p;

    lock_acquire(guts->lock);
	kprintf(">>> male whale #%ld starting\n", which);

    guts->male_indices[guts->male_index] = which;
	guts->male_index++;

	cv_signal(guts->mm_cv, guts->lock);
	cv_wait(guts->m_cv, guts->lock);

	kprintf("*** male whale #%ld mating\n", which);

    guts->num_mating++;

    cv_signal(guts->mm_cv, guts->lock);
    cv_wait(guts->mmating_cv, guts->lock);

	kprintf("<<< male whale #%ld exiting\n", which);
	lock_release(guts->lock);

    V(sem);
}

static
void
female(void *p, unsigned long which)
{
    whale_guts *guts = (whale_guts *)p;

    lock_acquire(guts->lock);
	kprintf(">>> female whale #%ld starting\n", which);

	guts->female_indices[guts->female_index] = which;
	guts->female_index++;

	cv_signal(guts->mm_cv, guts->lock);
	cv_wait(guts->f_cv, guts->lock);

	kprintf("*** female whale #%ld mating\n", which);

    guts->num_mating++;

    cv_signal(guts->mm_cv, guts->lock);
	cv_wait(guts->fmating_cv, guts->lock);

	kprintf("<<< female whale #%ld exiting\n", which);
	lock_release(guts->lock);

    V(sem);
}

static
void
matchmaker(void *p, unsigned long which)
{
    whale_guts *guts = (whale_guts *)p;

    lock_acquire(guts->lock);
    kprintf(">>> matchmaker whale #%ld starting\n", which);

    if(guts->is_first == 1) {
        guts->is_first = 0;
    }
    else {
        cv_wait(guts->mmstarting_cv, guts->lock);
    }

	while(guts->m_cv->num_sl_threads == 0 || guts->f_cv->num_sl_threads == 0) {
		cv_wait(guts->mm_cv, guts->lock);
	}

	cv_signal(guts->m_cv, guts->lock);
	cv_signal(guts->f_cv, guts->lock);

    while (guts->num_mating < 2) {
        cv_wait(guts->mm_cv, guts->lock);
    }

    guts->num_mating--;
    guts->num_mating--;

    cv_signal(guts->mmating_cv, guts->lock);
	cv_signal(guts->fmating_cv, guts->lock);

    kprintf("*** matchmaker whale #%ld helping #%d and #%d\n", which, guts->male_indices[guts->matchmaker_index], guts->female_indices[guts->matchmaker_index]);
	kprintf("!!! Mating done!\n");

    guts->matchmaker_index++;

    if(guts->mmstarting_cv->num_sl_threads > 0) {
        cv_signal(guts->mmstarting_cv, guts->lock);
    }

	kprintf("<<< matchmaker whale #%ld exiting\n", which);
	lock_release(guts->lock);

    V(sem);
}


// Change this function as necessary
int
whalemating(int nargs, char **args)
{
    
    struct male_cv = cv_create("male_cv");
    struct female_cv = cv_create("female_cv");
    struct mm_cv = cv_create("mm_cv");
    struct whale_lock = lock_create("whale_lock");
    int male_count = 0;
    int female_count = 0;
    int mm_count = 0;

	int i, j, err=0;
    (void)nargs;
    (void)args;

    whale_guts guts;

	kprintf("nargs is %d\n", nargs);

	guts.m_cv = cv_create("male");
	guts.f_cv = cv_create("female");
	guts.mm_cv = cv_create("matchmaker");
    guts.mmstarting_cv = cv_create("matchmaker starting line");
    guts.mmating_cv = cv_create("male_mating");
    guts.fmating_cv = cv_create("female_mating");

	guts.lock = lock_create("whale_lock");

    guts.is_first = 1;
	guts.num_mating = 0;
	guts.male_index = 0;
	guts.female_index = 0;
	guts.matchmaker_index = 0;

    sem = sem_create("sem", 0);

	for (i = 0; i < 3; i++) {
		for (j = 0; j < NMATING; j++) {
			switch(i) {
			    case 0:

				err = thread_fork("Male Whale Thread",
						  NULL, male, &guts, j);
				break;
			    case 1:
				err = thread_fork("Female Whale Thread",
						  NULL, female, &guts, j);
				break;
			    case 2:
				err = thread_fork("Matchmaker Whale Thread",
						  NULL, matchmaker, &guts, j);
				break;
			}
			if (err) {
				panic("whalemating: thread_fork failed: %s)\n",
				      strerror(err));
			}
		}
	}

    /*
     * Semaphore is used to make sure that the main function does not end until
     * all threads have finished
     */
    for (i = 0; i < (NMATING * 3); i++) {
        P(sem);
    }

	return 0;
}
