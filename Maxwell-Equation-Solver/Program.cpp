#include "Program.h"
//General program class
Program::Program(std::string filename, int sweep, int changeneff, int neff): fileHandler(filename), willSweep(sweep)

{
	eigs = fileHandler.config.numModes;
	points.setPrimitiveType(sf::Points);

	w = fileHandler.config.screenSize;
	h = fileHandler.config.screenSize;

	if (changeneff == 1)

	{
		fileHandler.config.neffGuess = neff;
	}

	std::cout << "=====================" << std::endl;
	std::cout << "Starting Values" << std::endl;
	std::cout << "=====================" << std::endl;
	//std::cout << "Iterations: " << fileHandler.config.points << std::endl;
	std::cout << "Number of Modes: " << fileHandler.config.numModes << std::endl;
	std::cout << "Wavelength: " << fileHandler.config.wavelength << std::endl;
	std::cout << "Width of window in pixels: " << w << std::endl;
	std::cout << "Max Index Red: " << fileHandler.config.maxIndexRed << std::endl;
	std::cout << "Max Index Green: " << fileHandler.config.maxIndexGreen << std::endl;
	std::cout << "Max Index Blue: " << fileHandler.config.maxIndexBlue << std::endl;
	std::cout << "File input name: " << fileHandler.config.fiber << std::endl;
	std::cout << "=====================" << std::endl;
}

int Program::mainLoop()

{
	sf::Event events;
	sf::Clock sfClock;

	if (fileHandler.error)

	{
		return EXIT_FAILURE;
	}

	if (willSweep == 1)

	{
		Sweep sweep(fileHandler.config.sweepType, fileHandler);
		sweep.wavelengthTrace(fileHandler.config.sweepStart, fileHandler.config.sweepEnd, fileHandler.config.sweepPoints);
		sweep.outputData(fileHandler.config.sweepFilename);
		//system("PAUSE");
	}

	else

	{
		//Calculate based on inital settings
		std::vector<double> localGeometry;
		if (calculate(localGeometry) != EXIT_SUCCESS)

		{
			return EXIT_FAILURE;
		}

		FieldViewer fieldViewer(fileHandler, field, localGeometry);
		fieldViewer.mainLoop();
	}
	
	return EXIT_SUCCESS;
}

void Program::draw()

{
	window.draw(points);
	if (gOn == 1) window.draw(geometry);
}

int Program::calculate(std::vector<double>& localGeometry)

{
	Clock c;
	//Intialise maxwell solver
	
	std::vector<double> perms;
	fileHandler.loadGeometry();
	fileHandler.getGeometry(perms);
	fileHandler.getDrawGeometry(localGeometry);
	MaxwellSolver max(fileHandler.config);
	max.setPerms(perms);
	max.buildBoundaries();
	max.buildMatrix();

	double k = 2.0*PI / fileHandler.config.wavelength;

	if (max.findModes(pow(k*fileHandler.config.neffGuess, 2.)) == EXIT_SUCCESS)

	{
		field = max.constructField();
		std::cout << "=====================" << std::endl << "All done in " << c.elapsed() << " ms" << std::endl << "=====================" << std::endl;
		return EXIT_SUCCESS;
	}

	else

	{
		return EXIT_FAILURE;
	}
}