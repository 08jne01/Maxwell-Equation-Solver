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
	std::string colorMapFileName;
	int sweepType;
	double overlapConfidence;
	double sweepStart;
	double sweepEnd;
	int sweepPoints;
	int initMode;
	std::string sweepFilename;
	int profileOn;
	std::string indexProfile;
	double profileMultiplier;
	int pointsX;
	int pointsY;
};