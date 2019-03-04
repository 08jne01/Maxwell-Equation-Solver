#include "Program.h"

int main(int argc, char* argv[])

{
	std::string filename = "Resources/config.ini";

	int width = 1000;
	int willSweep = 0;

	//std::cout << Eigen::nbThreads() << std::endl;


	std::string configCommand = "-config";
	std::string sweepCommand = "-sweep";

	if (argc != 0)

	{
		for (int i = 0; i < argc; i++)

		{
			if (argv[i] == sweepCommand)

			{
				willSweep = 1;
			}

			if (argv[i] == configCommand)

			{
				std::cout << "Config file (blank for default):";
				if (std::cin.peek() != '\n')

				{
					std::string file;
					std::cin >> file;
					filename = "Resources/" + file;
				}
			}
		}
	}

	sf::err().rdbuf(NULL);
	Program p(width, width, filename, willSweep);
	p.mainLoop();
	system("PAUSE");
	return 0;
}