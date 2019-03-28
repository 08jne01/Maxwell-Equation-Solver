#include "FileHandler.h"
//FileHandler class definitions
FileHandler::FileHandler(std::string filename): error(0)

{
	readConfig(filename);
}

void FileHandler::readConfig(std::string filename)

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

	if (conf.size() != 18)

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
		conf[5] >> config.maxIndex;
		conf[6] >> config.neffGuess;
		conf[7] >> config.convergance;
		conf[8] >> config.fiber;
		conf[9] >> config.timers;
		conf[10] >> config.screenSize;
		conf[11] >> config.sweepType;
		conf[12] >> config.overlapConfidence;
		conf[13] >> config.sweepStart;
		conf[14] >> config.sweepEnd;
		conf[15] >> config.sweepPoints;
		conf[16] >> config.initMode;
		conf[17] >> config.sweepFilename;
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
	fread(info, 54 * sizeof(unsigned char), sizeof(unsigned char), in);
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];
	config.pointsX = width;
	config.pointsY = height;
	int size = 3 * width * height;
	unsigned char* data = new unsigned char[size];
	fread(data, size * sizeof(unsigned char), sizeof(unsigned char), in);
	//fread_s(data, size * sizeof(unsigned char), sizeof(unsigned char), size, in);
	fclose(in);
	free(filenameCharArray);

	for (int i = 0; i < width*height; i++)

	{
		double val1 = (double)(data[3 * (i)] + data[3 * (i)+1] + data[3 * (i)+2]) / (3.0);
		double val2 = (config.maxIndex - 1.0)*val1 / 255.0 + 1.0;
		drawGeometry.push_back(val1);
		geometry.push_back(val2*val2);
	}
	//Need to make a check for sizes!
	delete data;
	std::cout << config.fiber << " geometry imported!" << std::endl;
}