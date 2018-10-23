#include "Program.h"
#include "Solver.h"
//General program class
Program::Program(int width, int height): w(width), h(height), window(sf::VideoMode(width,height), "Maxwell Equation Solver")

{
	points.setPrimitiveType(sf::Points);
}

int Program::mainLoop()

{
	sf::Event events;

	if (!window.isOpen())

	{
		return EXIT_FAILURE;
	}

	calculate(w);

	while (window.isOpen())

	{
		while (window.pollEvent(events))

		{
			if (events.type == sf::Event::EventType::Closed)

			{
				window.close();
			}
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
	double clockStart = clock();
	double elapsed;
	Solver s(size);
	s.buildProblem();
	s.solveProblem();
	s.saveProblem();
	s.normalise();
	s.writeProblem();
	elapsed = clock() - clockStart;
	std::cout << "Done in " << elapsed << " ms" << std::endl;

	for (int i = 0; i < w; i++)

	{
		for (int j = 0; j < w; j++)

		{
			double val = s.valuesNormalised[i][j];
			points.append(sf::Vertex(sf::Vector2f(i,j), sf::Color(3*255.0*(val/4) + 255.0/4, 0,(1-val)*255*3 / 4.0)));
			//std::cout << 255 * val << std::endl;
		}
	}
}