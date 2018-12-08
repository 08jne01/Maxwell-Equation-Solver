#pragma once
#include <string>

struct Config

{

	Config()

	{

	}

	int points;
	int numModes;
	double sizeOfStructure;
	double numPointStructure;
	double wavelength;
	double maxIndex;
	int convergance;
	std::string fiber;
};