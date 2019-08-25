#pragma once
#include <ctime>

class Clock

{
public:
	Clock();
	void reset();
	double elapsed() const; //Returns elapsed time in miliseconds
private:
	double start;
};