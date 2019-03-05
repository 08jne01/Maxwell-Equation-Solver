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
	Config config;
	int error;
private:
	
};