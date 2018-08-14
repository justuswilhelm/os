// Functions and structs largely taken from
// http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html
#pragma once
#include <stdint.h>

#define TIMER_BASE_FREQ 1193180
#define TIMER_PORT_0 0x40
#define TIMER_PORT_1 0x41
#define TIMER_PORT_2 0x42
#define TIMER_PORT_3 0x43

void init_timer(uint32_t frequency);
