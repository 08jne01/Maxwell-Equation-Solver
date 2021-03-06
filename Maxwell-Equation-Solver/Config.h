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
	double maxIndexRed;
	double maxIndexGreen;
	double maxIndexBlue;
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
	std::string indexProfileRed;
	std::string indexProfileGreen;
	std::string indexProfileBlue;
	double profileMultiplier;
	int pointsX;
	int pointsY;
};