#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "Config.h"

//Class for handling file input and output
class FileHandler

{
public:

	FileHandler(const std::string filename);
	void readConfig(const std::string filename);
	void getGeometry(std::vector<double>& geometry, std::vector<double>& drawGeometry);
	double indexAlgorithm(double r, double g, double b);
	void readCSV(const std::string filename, std::vector<std::vector<double>>& dateVector, int columns, int ignoreLine = 0) const;
	Config config;
	int error;
private:
	
};