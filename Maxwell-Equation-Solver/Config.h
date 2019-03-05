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
	double neffGuess;
	int convergance;
	std::string fiber;
	int timers;
	int screenSize;
	int sweepType;
	double sweepStart;
	double sweepEnd;
	int sweepPoints;
	std::string sweepFilename;
};