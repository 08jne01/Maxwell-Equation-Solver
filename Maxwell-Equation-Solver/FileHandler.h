#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "Config.h"
#include "Color.h"

//Class for handling file input and output
class FileHandler

{
public:

	FileHandler(const std::string filename);
	void readConfig(const std::string filename);
	void rebuildGeometry(std::vector<double>& geometry, double maxRed, double maxBlue, double maxGreen);
	void getDrawGeometry(std::vector<double>& drawGeometry);
	void getGeometry(std::vector<double>& geometry);
	void loadGeometry();
	double indexAlgorithm(double r, double g, double b, double rMax = -1.0, double gMax = -1.0, double bMax = -1.0);
	void readCSV(const std::string filename, std::vector<std::vector<double>>& dateVector, int columns, int ignoreLine = 0) const;
	Config config;
	int error;

private:

	std::vector<Color> loadedGeometry;
};