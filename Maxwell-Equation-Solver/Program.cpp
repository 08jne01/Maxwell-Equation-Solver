#include "Program.h"
//General program class
Program::Program(int width, int height): w(width), h(height), window(sf::VideoMode(width,height), "Maxwell Equation Solver")

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

	calculate(200);
	int mode = 0;

	while (window.isOpen())

	{
		while (window.pollEvent(events))

		{
			if (events.type == sf::Event::EventType::Closed)

			{
				window.close();
			}
		}

		if (sfClock.getElapsedTime().asSeconds() > 1.0)

		{
			setMode(mode % 10);
			std::cout << "Current Mode: " << mode % 10 << std::endl;
			mode++;
			sfClock.restart();
		}

		window.clear(sf::Color::Black);

		//Draw Here
		draw();

		window.display();

	}

	return EXIT_SUCCESS;
}

void Program::draw()

{
	window.draw(points);
}

void Program::calculate(int size)

{
	Clock c;
	MaxwellSolver max(size, 1000000);
	max.buildPerm();
	max.buildPotCoeffs();
	max.buildMatrix();
	max.findModes();
	std::cout << "=====================" << std::endl << "All done in " << c.elapsed() << " ms" << std::endl;

	vectors = max.eigenVectors;
	
}

void Program::setMode(int mode)

{
	points.clear();
	Eigen::VectorXd vec = vectors.col(mode);
	std::vector<double> values;
	for (int i = 0; i < vec.size() / 2; i++)

	{
		values.push_back(sqrt(vec[i]*vec[i]));
	}

	std::vector<double> normal;

	normalise(values, normal);

	for (int i = 0; i < w; i++)

	{
		for (int j = 0; j < w; j++)

		{
			double val = getValue(normal, sqrt(vec.size() / 2), i, j, w, w);
			//std::cout << val << std::endl;
			points.append(sf::Vertex(sf::Vector2f(i, j), sf::Color(255 * val, 255 * val, 255 * val)));
		}
	}
}

double Program::interpolate(double d1, double d2, double w)

{
	return d1 + w * (d2 - d1);
	//double w2 = (1 - cos(w*PI)) / 2.0;
	//return (d1 * (1 - w2) + d2 * w2);
}

double Program::getValue(std::vector<double> &gridPoints, int sideLength, int x, int y, int w, int h)

{
	double xVal = ((double)x / (double)w)*(double)sideLength;
	double yVal = ((double)y / (double)h)*(double)sideLength;

	int xVal0 = (int)xVal;
	int xVal1 = xVal0 + 1;
	int yVal0 = (int)yVal;
	int yVal1 = yVal0 + 1;

	double sX = xVal - double(xVal0);
	double sY = yVal - double(yVal0);

	double ix0, ix1, n0, n1;

	n0 = gridPoints[xVal0 + yVal0 * sideLength];
	n1 = gridPoints[xVal1 + yVal0 * sideLength];

	ix0 = interpolate(n0, n1, sX);

	n0 = gridPoints[xVal0 + yVal1 * sideLength];
	n1 = gridPoints[xVal1 + yVal1 * sideLength];

	ix1 = interpolate(n0, n1, sX);

	return interpolate(ix0, ix1, sY);

}

void Program::normalise(std::vector<double> &vec, std::vector<double> &normalisedVals)

{
	double max = 0.0;

	for (int i = 0; i < vec.size(); i++)

	{
		if (vec[i] > max)

		{
			max = vec[i];
		}
	}

	for (int i = 0; i < vec.size(); i++)

	{
		double val = vec[i] / max;

		if (val > 1.0)

		{
			val = 1.0;
		}

		else if (val < 0.0)

		{
			val = 0.0;
		}

		normalisedVals.push_back(val);
	}
}