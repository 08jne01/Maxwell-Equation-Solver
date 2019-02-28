#include "Program.h"

int main()

{
	std::string filename;

	int width = 1000;

	//std::cout << Eigen::nbThreads() << std::endl;

	std::cout << "Config file (blank for default):";
	if (std::cin.peek() == '\n')

	{
		filename = "Resources/config.ini";
	}

	else

	{
		std::string file;
		std::cin >> file;
		filename = "Resources/" + file;
	}

	sf::err().rdbuf(NULL);
	Program p(width, width, filename);

	return p.mainLoop();
}