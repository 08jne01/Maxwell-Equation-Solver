#include "Program.h"
//General program class
Program::Program(int width, int height, int iters, int eigVals, double kVal, double length, double permativity, int convNum, std::string f): 
	w(width), h(height), window(sf::VideoMode(width,height), "Maxwell Equation Solver"), its(iters), eigs(eigVals), k(kVal), l(length), perm(permativity),
	conv(convNum), displayField(3), modeSet(0), filename(f)

{
	points.setPrimitiveType(sf::Points);
}

int Program::mainLoop()

{
	sf::Event events;
	sf::Clock sfClock;

	if (!window.isOpen())

	{
		return EXIT_FAILURE;
	}

	if (calculate(its) != EXIT_SUCCESS)

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

			keyCallBack(events);
		}

		if (modeSet != 1)

		{
			setMode(mode);
			std::cout << "Current Mode: " << (mode)+1 << std::endl << "=====================" << std::endl;
			modeSet = 1;
		}

		window.clear(sf::Color::White);

		//Draw Here
		draw();

		window.display();

	}

	return EXIT_SUCCESS;
}

void Program::draw()

{
	window.draw(points);
	if (gOn == 1) window.draw(geometry);
}

int Program::calculate(int size)

{
	Clock c;

	//double k = 1000000.0;

	MaxwellSolver max(size, k, l, perm, eigs, conv);
	max.buildGeometry(perm, filename);

	for (int i = 0; i < w; i++)

	{
		for (int j = 0; j < w; j++)

		{
			double val = getValue(max.geometry, sqrt(max.geometry.size()), i, j, w, w);
			geometry.append(sf::Vertex(sf::Vector2f(i, j), sf::Color(val, val, val, 200)));
		}
	}
	gOn = 0;

	max.buildPerm();
	max.buildPotCoeffs();
	max.buildMatrix();
	

	if (max.findModes() == EXIT_SUCCESS)

	{
		max.findField();
		
		fieldComponents.push_back(max.Hx);
		fieldComponents.push_back(max.Hy);
		fieldComponents.push_back(max.Hz);
		fieldComponents.push_back(max.Ex);
		fieldComponents.push_back(max.Ey);
		fieldComponents.push_back(max.Ez);

		eigenValues = max.eigenVals;
		std::cout << "=====================" << std::endl << "All done in " << c.elapsed() << " ms" << std::endl << "=====================" << std::endl;
		return EXIT_SUCCESS;
	}

	else

	{
		system("PAUSE");
		return EXIT_FAILURE;
	}
	
	
	
}

void Program::setMode(int mode)

{
	points.clear();
	Eigen::VectorXd vec;

	switch (displayField)

	{
		case FIELD_EX: 
		
		{
			vec = fieldComponents[displayField].col(mode);
			std::cout << "Displaying Ex Component" << std::endl;
			break;
		}
		
		case FIELD_EY:

		{
			vec = fieldComponents[displayField].col(mode);
			std::cout << "Displaying Ey Component" << std::endl;
			break;
		}

		case FIELD_EZ:

		{
			vec = fieldComponents[displayField].col(mode);
			std::cout << "Displaying Ez Component" << std::endl;
			break;
		}

		case FIELD_HX:

		{
			vec = fieldComponents[displayField].col(mode);
			std::cout << "Displaying Hx Component" << std::endl;
			break;
		}

		case FIELD_HY:

		{
			vec = fieldComponents[displayField].col(mode);
			std::cout << "Displaying Hy Component" << std::endl;
			break;
		}

		case FIELD_HZ:

		{
			vec = fieldComponents[displayField].col(mode);
			std::cout << "Displaying Hz Component" << std::endl;
			break;
		}
		
	}

	std::cout << "Eigen Value: " << eigenValues[mode] << std::endl;
	std::cout << "neff: " << sqrt(abs(eigenValues[mode]))/k << std::endl;
	std::vector<double> values;
	for (int i = 0; i < vec.size(); i++)

	{
		
		double num = vec[i];
		//re = num.real();
		//im = num.imag();


		//std::cout << num << std::endl;
		values.push_back(num);
	}

	std::vector<double> normal;

	normalise(values, normal);

	for (int i = 0; i < w; i++)

	{
		for (int j = 0; j < w; j++)

		{
			double val = getValue(normal, sqrt(vec.size()), i, j, w, w);
			double colorR, colorB, colorG;
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

			//std::cout << val << std::endl;
			//std::cout << colorR << std::endl;
			points.append(sf::Vertex(sf::Vector2f(i,j), sf::Color(colorR, colorG, colorB)));
		}
	}
}

double Program::interpolate(double d1, double d2, double w)

{
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

double Program::getValue(std::vector<double> &gridPoints, int sideLength, int x, int y, int w, int h)

{
	double xVal = ((double)x / (double)w)*(double)sideLength;
	double yVal = ((double)y / (double)h)*(double)sideLength;

	int xVal0 = (int)xVal;
	int xVal1 = xVal0 + 1;
	int yVal0 = (int)yVal;
	int yVal1 = yVal0 + 1;

double sX = pow(xVal - double(xVal0), 1);
double sY = pow(yVal - double(yVal0), 1);
/*
double s00 = sqrt(pow(xVal - (double)xVal0, 2) + pow(yVal - (double)yVal0, 2));
double s01 = sqrt(pow(xVal - (double)xVal0, 2) + pow(yVal - (double)yVal1, 2));
double s10 = sqrt(pow(xVal - (double)xVal1, 2) + pow(yVal - (double)yVal0, 2));
double s11 = sqrt(pow(xVal - (double)xVal1, 2) + pow(yVal - (double)yVal1, 2));

double v00 = s00 * gridPoints[xVal0 + yVal0 * sideLength];
double v01 = s01 * gridPoints[xVal0 + yVal1 * sideLength];
double v10 = s10 * gridPoints[xVal1+ yVal0 * sideLength];
double v11 = s11 * gridPoints[xVal1 + yVal1 * sideLength];

return (v00 + v01 + v10 + v11) / 4.0;
*/
double ix0, ix1, n0, n1;
n0 = gridPoints[xVal0 + yVal0 * sideLength];
n1 = gridPoints[xVal1 + yVal0 * sideLength];

ix0 = interpolate(n0, n1, sX);

n0 = gridPoints[xVal0 + yVal1 * sideLength];
n1 = gridPoints[xVal1 + yVal1 * sideLength];

ix1 = interpolate(n0, n1, sX);
/*
if (xVal == xVal0 && yVal == yVal0)

{
	return gridPoints[xVal0 + yVal0 * sideLength];
}

else

{
	return 0.0;
}
*/
return interpolate(ix0, ix1, sY);
}

void Program::normalise(std::vector<double> &vec, std::vector<double> &normalisedVals)

{
	double max = -10.0;
	double min = 1.0;

	for (int i = 0; i < vec.size(); i++)

	{
		if (abs(vec[i]) > max)

		{
			max = abs(vec[i]);
		}
	}

	for (int i = 0; i < vec.size(); i++)

	{
		if (abs(vec[i]) < min)

		{
			min = abs(vec[i]);
		}
	}

	for (int i = 0; i < vec.size(); i++)

	{
		double val = 1 * vec[i] / max;

		if (val > 1.0)

		{
			//val = 1.0;
		}

		else if (val < -1.0)

		{
			//val = -1.0;
		}
		normalisedVals.push_back(val);
	}

}

void Program::writeFields()

{
	std::ofstream file;
	std::stringstream os;
	std::string s = "Field_Components_Mode_";
	std::string end = ".dat";
	os << s << mode+1 << end;
	file.open(os.str());

	int size = sqrt(fieldComponents[0].col(0).size());

	file << "x,y,Hx,Hy,Hz,Ex,Ey,Ez" << std::endl;

	for (int i = 0; i < size; i++)

	{
		for (int j = 0; j < size; j++)

		{
			file << i << "," << j << ","
				<< fieldComponents[0].col(mode)[i + size * j] << ","
				<< fieldComponents[1].col(mode)[i + size * j] << ","
				<< fieldComponents[2].col(mode)[i + size * j] << ","
				<< fieldComponents[3].col(mode)[i + size * j] << ","
				<< fieldComponents[4].col(mode)[i + size * j] << ","
				<< fieldComponents[5].col(mode)[i + size * j] << std::endl;
		}
	}
}

void Program::keyCallBack(sf::Event events)

{
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

			case sf::Keyboard::Enter:

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