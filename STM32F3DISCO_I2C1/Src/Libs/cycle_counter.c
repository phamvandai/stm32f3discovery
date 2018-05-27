#include "cycle_counter.h"

volatile unsigned int *DWT_CYCCNT = (unsigned int *)0xE0001004;		//address of the register
volatile unsigned int *DWT_CONTROL = (unsigned int *)0xE0001000;	//address of the register
volatile unsigned int *SCB_DEMCR = (unsigned int *)0xE000EDFC;		//address of the register


void reset_timer()
{
    *SCB_DEMCR   = *SCB_DEMCR | 0x01000000;
    *DWT_CYCCNT  = 0; // reset the counter
    *DWT_CONTROL = 0;
}

void start_timer()
{
    *DWT_CONTROL = *DWT_CONTROL | 1 ; // enable the counter
}

void stop_timer()
{
    *DWT_CONTROL = *DWT_CONTROL | 0 ; // disable the counter
}

unsigned int getCycles()
{
    return *DWT_CYCCNT;
}
