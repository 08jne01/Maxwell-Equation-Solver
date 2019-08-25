#include "FileHandler.h"
//FileHandler class definitions
FileHandler::FileHandler(const std::string filename): error(0)

{
	readConfig(filename);
}

void FileHandler::readConfig(const std::string filename)

{
	std::ifstream file;
	file.open(filename);
	std::string line;
	std::string data;
	std::vector<std::stringstream> conf;

	if (!file.is_open())

	{
		std::cout << "Failed to retrive config!" << std::endl;
		std::string error;
		//char arr[20];
		//strerror_s(arr, errno);

		//Change this
		char* arr = strerror(errno);

		std::cout << "ERROR: " << arr << std::endl;
		error = 1;
	}

	while (getline(file, line))

	{
		std::stringstream os(line);

		if (line.find("#") != std::string::npos)

		{
			//Do nothing
		}

		else

		{
			conf.push_back(std::stringstream(line));
		}
	}

	if (conf.size() != 24)

	{
		std::cout << "Error Reading Config File!" << std::endl;
		error = 1;
	}

	else

	{
		std::stringstream os;

		conf[0] >> config.points;
		conf[1] >> config.numModes;
		conf[2] >> config.sizeOfStructure;
		conf[3] >> config.numPointStructure;
		conf[4] >> config.wavelength;
		conf[5] >> config.maxIndexRed;
		conf[6] >> config.maxIndexGreen;
		conf[7] >> config.maxIndexBlue;
		conf[8] >> config.neffGuess;
		conf[9] >> config.convergance;
		conf[10] >> config.fiber;
		conf[11] >> config.timers;
		conf[12] >> config.screenSize;
		conf[13] >> config.colorMapFileName;
		conf[14] >> config.sweepType;
		conf[15] >> config.overlapConfidence;
		conf[16] >> config.sweepStart;
		conf[17] >> config.sweepEnd;
		conf[18] >> config.sweepPoints;
		conf[19] >> config.initMode;
		conf[20] >> config.sweepFilename;
		conf[21] >> config.profileOn;
		conf[22] >> config.indexProfile;
		conf[23] >> config.profileMultiplier;
	}
}

void FileHandler::getGeometry(std::vector<double>& geometry, std::vector<double>& drawGeometry)

{
	//Allocate memory for filename and convert filename to char array so that string can be used
	char *filenameCharArray = (char*)malloc(sizeof(char) * (config.fiber.size() + 1));
	//strcpy_s(filenameCharArray, config.fiber.size() + 1, config.fiber.c_str());

	strcpy(filenameCharArray, config.fiber.c_str());

	//Open file and create write buffer
	//FILE *in;
	FILE *in = fopen(filenameCharArray, "rb");
	//fopen_s(&in, filenameCharArray, "rb");
	unsigned char info[54];
	//fread_s(info, 54 * sizeof(unsigned char), sizeof(unsigned char), 54, in);
	size_t result = fread(info, 54 * sizeof(unsigned char), sizeof(unsigned char), in);
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];
	config.pointsX = width;
	config.pointsY = height;
	int size = 3 * width * height;
	unsigned char* data = new unsigned char[size];
	result = fread(data, size * sizeof(unsigned char), sizeof(unsigned char), in);
	//fread_s(data, size * sizeof(unsigned char), sizeof(unsigned char), size, in);
	fclose(in);
	free(filenameCharArray);

	for (int i = 0; i < width*height; i++)

	{
		
		double b = data[3 * (i)];
		double g = data[3 * (i) + 1];
		double r = data[3 * (i) + 2];

		//std::cout << r << std::endl;

		double avg = (r + g + b) / (3.0);
		//double val2 = (config.maxIndex - 1.0)*val1 / 255.0 + 1.0;
		double refractiveIndex = indexAlgorithm(r, g, b);
		drawGeometry.push_back(std::max(r, std::max(g, b)));
		geometry.push_back(refractiveIndex*refractiveIndex);
	}
	//Need to make a check for sizes!
	delete data;
	std::cout << config.fiber << " geometry imported!" << std::endl;
}

double FileHandler::indexAlgorithm(double r, double g, double b)

{
	double refR = (config.maxIndexRed - 1.0)* r / 255.0 + 1.0;
	double refG = (config.maxIndexGreen - 1.0)* g / 255.0 + 1.0;
	double refB = (config.maxIndexBlue - 1.0)* b / 255.0 + 1.0;

	//std::cout << refR << " " << refG << " " << refB << std::endl;

	return std::max(std::max(refG, refB), refR);
}

void FileHandler::readCSV(const std::string filename, std::vector<std::vector<double>>& dataVector, int columns, int ignoreLine) const

{
	std::ifstream in(filename);
	std::string line;
	std::string data;

	std::vector<std::vector<double>> tempVec;

	if (!in.is_open())

	{
		std::cout << "Failed to open data!" << std::endl;
		//char arr[20];
		char* arr = strerror(errno);
		std::cout << "Error: " << arr << std::endl;
		return;
	}
	while (in.good())

	{
		std::vector<double> buffer;

		for (int i = 0; i < columns; i++)

		{
			if (i < columns - 1) getline(in, data, ',');

			else getline(in, data, '\n');
			std::stringstream os(data);
			double temp;
			os >> temp;
			buffer.push_back(temp);
			//std::cout << data << std::endl;
		}

		if (ignoreLine != 1) tempVec.push_back(buffer);
		ignoreLine = 0;
	}

	tempVec.pop_back();

	dataVector = tempVec;
}