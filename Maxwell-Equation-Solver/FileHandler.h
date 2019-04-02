#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstring>
#include "Config.h"

//Class for handling file input and output
class FileHandler

{
public:

	FileHandler(std::string filename);
	void readConfig(std::string filename);
	void getGeometry(std::vector<double>& geometry, std::vector<double>& drawGeometry);
	void readCSV(std::string filename, std::vector<std::vector<double>>& dateVector, int columns, int ignoreLine = 0);
	Config config;
	int error;
private:
	
};