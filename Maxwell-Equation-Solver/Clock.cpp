#include "Clock.h"

Clock::Clock()

{
	start = clock();
}

void Clock::reset()

{
	start = clock();
}

double Clock::elapsed()

{
	return clock() - start;
}

