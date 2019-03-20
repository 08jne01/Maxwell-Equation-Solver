#include "Program.h"
//General program class
Program::Program(int width, int height, std::string filename, int sweep, int changeneff, int neff): 
	w(width), h(height), displayField(0), modeSet(0), gOn(0), fileHandler(filename), willSweep(sweep)

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
	std::cout << "Iterations: " << fileHandler.config.points << std::endl;
	std::cout << "Number of Modes: " << fileHandler.config.numModes << std::endl;
	std::cout << "Wavelength: " << fileHandler.config.wavelength << std::endl;
	std::cout << "Size of window in pixels: " << width << std::endl;
	std::cout << "Max Index: " << fileHandler.config.maxIndex << std::endl;
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
	}

	else

	{
		//Calculate based on inital settings
		if (calculate() != EXIT_SUCCESS)

		{
			return EXIT_FAILURE;
		}
		double ratio = (double)fileHandler.config.pointsY / (double)fileHandler.config.pointsX;
		
		h = (int)((double)w * ratio);
		std::cout << w << " " << h << std::endl;
		//h = w;
		//h = w;
		//Create the window after calculation
		window.create(sf::VideoMode(w, h), "FDFD Maxwell Solver");

		if (!window.isOpen())

		{
			return EXIT_FAILURE;
		}

		while (window.isOpen())

		{
			while (window.pollEvent(events))

			{
				if (events.type == sf::Event::EventType::Closed)

				{
					window.close();
				}
				//Callback for user commands
				keyCallBack(events);
			}

			if (modeSet != 1)

			{
				setMode(mode);
				std::cout << "Current Mode: " << (mode)+1 << std::endl << "=====================" << std::endl;
				modeSet = 1;
			}

			window.clear(sf::Color::Black);

			//Draw Here
			draw();

			window.display();

		}
	}
	return EXIT_SUCCESS;
}

void Program::draw()

{
	window.draw(points);
	if (gOn == 1) window.draw(geometry);
}

int Program::calculate()

{
	Clock c;
	//Intialise maxwell solver
	std::vector<double> localGeometry;
	std::vector<double> perms;
	fileHandler.getGeometry(perms, localGeometry);
	MaxwellSolver max(fileHandler.config);
	max.perms = perms;
	
	//max.buildGeometry(perm, filename);
	//Put geometry into vector for rendering
	for (int i = 0; i < w; i++)

	{
		for (int j = 0; j < h; j++)

		{
			double val = getValue(localGeometry, fileHandler.config.pointsX, fileHandler.config.pointsY, i, j, w, h);
			if (val > 10) geometry.append(sf::Vertex(sf::Vector2f(i, (-j + h) % h), sf::Color(val, val, val, 100)));
		}
	}
	//Build boundaries and matrices
	max.buildBoundaries();
	max.buildMatrix();

	double k = 2.0*PI / fileHandler.config.wavelength;


	//Find the modes
	//pow(k*0.99999, 2.0)
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

void Program::setMode(int mode)

{
	//Sets the mode
	std::cout << "Displaying " << field.getFieldName(displayField) << " Component" << std::endl;
	std::cout << "Eigen Value: " << field.eigenValues[mode] << std::endl;
	
	std::cout << std::setprecision(10) << "neff: " << sqrt(std::abs((double)field.eigenValues[mode])) / (double)field.k << std::endl;
	points.clear();
	std::vector<double> normal;
	Eigen::VectorXd vec = field.getField(displayField).col(mode);
	normalise(vec, normal);
	//Sets color scheme
	for (int i = 0; i < w; i++)

	{
		for (int j = 0; j < h; j++)

		{
			double val = getValue(normal, fileHandler.config.pointsX, fileHandler.config.pointsY, i, j, w, h);
			int colorR, colorB, colorG;
			if (val < 0.0)

			{
				colorB = 255*(1 - pow((val + 1), 1));
				colorR = 0;
				colorG = 255*(1 - 1 * (-(val*val) + 1));
			}

			else

			{
				colorR = 255*(1 - pow(-(val - 1), 1));
				colorB = 0;
				colorG = 255*(1 - (-(val*val) + 1));
			}
			points.append(sf::Vertex(sf::Vector2f(i,(-j + h) % h), sf::Color(colorR, colorG, colorB)));
		}
	}
}

double Program::interpolate(double d1, double d2, double w)

{
	//Linearly interpolate (there is also bicubic and cosine interpolation, but linear seems best)
	return d1 + w * (d2 - d1);
	//double w2 = (1 - cos(w*PI)) / 2.0;
	//return (d1 * (1 - w2) + d2 * w2);
	/*
	double a = 2 * d1 - 2 * d2;
	double b = -3 * d1 + 3 * d2;
	double d = d1;

	return w*w*w*a + b * w*w + d;
	*/
}

double Program::getValue(std::vector<double> &gridPoints, int sideLengthX, int sideLengthY, int x, int y, int w, int h)

{
	//Interpolation algorithm (Similar to the last half of a perlin noise algorithm)
	double xVal = ((double)x / (double)w)*(double)sideLengthX;
	double yVal = ((double)y / (double)h)*(double)sideLengthY;

	int xVal0 = (int)xVal;
	int xVal1 = xVal0 + 1;
	int yVal0 = (int)yVal;
	int yVal1 = yVal0 + 1;

	double sX = pow(xVal - double(xVal0), 1);
	double sY = pow(yVal - double(yVal0), 1);
	double ix0, ix1, n0, n1;
	n0 = gridPoints[xVal0 + yVal0 * sideLengthX];
	n1 = gridPoints[xVal1 + yVal0 * sideLengthX];

	ix0 = interpolate(n0, n1, sX);

	n0 = gridPoints[xVal0 + yVal1 * sideLengthX];
	n1 = gridPoints[xVal1 + yVal1 * sideLengthX];

	ix1 = interpolate(n0, n1, sX);
	return interpolate(ix0, ix1, sY);
}

void Program::normalise(Eigen::VectorXd& vec, std::vector<double>& normalisedVals)

{
	//Normalising algorithm for rendering
	double max = -10.0;

	for (int i = 0; i < vec.size(); i++)

	{
		if (std::abs(vec[i]) > max)

		{
			max = std::abs(vec[i]);
		}
	}

	for (int i = 0; i < vec.size(); i++)

	{
		double val = 1 * vec[i] / max;

		if (val > 1.0)

		{
			val = 1.0;
		}

		else if (val < -1.0)

		{
			val = -1.0;
		}
		normalisedVals.push_back(val);
	}
}

void Program::writeFields()

{
	//Output fields to a file
	std::ofstream file;
	std::stringstream os;
	std::string s = "Output_Data/Field_Components_Mode_";
	std::string end = ".dat";
	os << s << mode+1 << end;
	file.open(os.str());

	int size = fileHandler.config.pointsX;

	file << "x,y,Ex,Ey,Ez,Hx,Hy,Hz" << std::endl;

	double dx = field.dx;
	double dy = field.dy;

	for (int i = 0; i < fileHandler.config.pointsY; i++)

	{
		for (int j = 0; j < fileHandler.config.pointsX; j++)

		{

			file << i*dx << "," << j*dy << ","
				<< field.Ex.col(mode)[j + size * i] << ","
				<< field.Ey.col(mode)[j + size * i] << ","
				<< field.Ez.col(mode)[j + size * i] << ","
				<< field.Hx.col(mode)[j + size * i] << ","
				<< field.Hy.col(mode)[j + size * i] << ","
				<< field.Hz.col(mode)[j + size * i] << std::endl;
		}
	}

	file.close();
}

void Program::keyCallBack(sf::Event events)

{	//Callbacks for commands
	if (events.type == sf::Event::EventType::KeyPressed)

	{
		switch (events.key.code)

		{
			case sf::Keyboard::Left:

			{
				displayField--;
				if (displayField < 0) displayField = 5;
				modeSet = 0;
				break;
			}

			case sf::Keyboard::Right:

			{
				displayField++;
				if (displayField > 5) displayField = 0;
				modeSet = 0;
				break;
			}

			case sf::Keyboard::Up:

			{
				mode++;
				if (mode > eigs - 1) mode = 0;
				modeSet = 0;
				break;
			}

			case sf::Keyboard::Down:

			{
				mode--;
				if (mode < 0) mode = eigs - 1;
				modeSet = 0;
				break;
			}

			case sf::Keyboard::Space:

			{
				if (gOn == 1) gOn = 0;
				else gOn = 1;
				break;
			}

			case sf::Keyboard::Return:

			{
				std::cout << "Outputing Current Fields..." << std::endl;
				writeFields();
				break;
			}

			case sf::Keyboard::Escape:

			{
				window.close();
				break;
			}
		}
	}	
}