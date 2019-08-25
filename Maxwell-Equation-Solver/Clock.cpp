#include "Clock.h"

Clock::Clock()

{
	start = clock();
}

void Clock::reset()

{
	start = clock();
}

double Clock::elapsed() const

{
	return 1000*(clock() - start) / CLOCKS_PER_SEC;
}

