#include "timer.h"

/* Function to wait for some msec: the program will stop there */
void wait_msec(unsigned int n)
{
    register unsigned long f, t, r, expiredTime;

    // Get the current counter frequency (Hz)
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    
    // Read the current counter value
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    
    // Calculate expire value for counter
    expiredTime = t + ( (f/1000)*n )/1000;
    do {
    	asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
    } while(r < expiredTime);
}

/* Function to start a timer (set = 1) or wait for it to expire (set = 0) */
void set_wait_timer(int set, unsigned int msVal) {
    static unsigned long expiredTime = 0; //declare static to keep value
    register unsigned long r, f, t;
    
    if (set) { /* SET TIMER */
        // Get the current counter frequency (Hz)
        asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));

        // Read the current counter
        asm volatile ("mrs %0, cntpct_el0" : "=r"(t));

        // Calculate expired time:
        expiredTime = t + ( (f/1000)*msVal )/1000;
    } 
    else { /* WAIT FOR TIMER TO EXPIRE */
        do {
            asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
        } while(r < expiredTime);
    }
}