#include "Header.h"
#include "Program.h"
//1 file for each class if possible
int main()

{
	double k, length, perm;
	int iters, numEigs, isDefault, width, conv;
	std::string filename;

	std::cout << "Default? (1 for yes): ";
	std::cin >> isDefault;

	//std::cout << Eigen::nbThreads() << std::endl;

	if (isDefault == 1)

	{
		iters = 160;
		numEigs = 40;
		length = 160.0e-6;
		k = 2.0*PI / (1.55e-6);
		width = 1000;
		perm = 2.09;
		conv = 10;
		filename = "Fibre.bmp";
	}

	else

	{
		std::cout << "Iters: ";
		std::cin >> iters;
		std::cout << "Num Eigs: ";
		std::cin >> numEigs;
		std::cout << "Length: ";
		std::cin >> length;
		std::cout << "k: ";
		std::cin >> k; 
		std::cout << "Perm: ";
		std::cin >> perm;
		std::cout << "Width: ";
		std::cin >> width;
		std::cout << "Convergance (10): ";
		std::cin >> conv;
		std::cout << "File-Name: ";
		std::cin >> filename;
	}

	std::cout << "=====================" << std::endl;
	std::cout << "Starting Values" << std::endl;
	std::cout << "=====================" << std::endl;
	std::cout << "Iterations: " << iters << std::endl;
	std::cout << "Number of Eigen Values: " << numEigs << std::endl;
	std::cout << "Width of window: " << length << std::endl;
	std::cout << "Wavenumber in freespace k0: " << k << std::endl;
	std::cout << "Size of window in pixels: " << width << std::endl;
	std::cout << "Max permativity: " << perm << std::endl;
	std::cout << "Convergence Value: " << conv << std::endl;
	std::cout << "File input name: " << filename << std::endl;
	std::cout << "=====================" << std::endl;

	sf::err().rdbuf(NULL);
	Program p(width, width, iters, numEigs, k, length, perm, conv, filename);

	return p.mainLoop();

}