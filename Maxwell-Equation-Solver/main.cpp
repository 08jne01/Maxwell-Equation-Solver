#include "Header.h"
#include "Program.h"
//1 file for each class if possible
int main()

{
	double k, length, perm;
	int iters, numEigs, isDefault, width;

	std::cout << "Default? (1 for yes): ";
	std::cin >> isDefault;

	if (isDefault == 1)

	{
		iters = 40;
		numEigs = 60;
		length = 40.0;
		k = 16 * PI*length;
		width = 900;
		perm = 5.0;
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
	}


	sf::err().rdbuf(NULL);
	Program p(width, width, iters, numEigs, k, length, perm);

	return p.mainLoop();

}