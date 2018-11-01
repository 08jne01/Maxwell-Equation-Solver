#include "Header.h"
#include "Program.h"
//1 file for each class if possible
int main()

{
	sf::err().rdbuf(NULL);
	Program p(200, 200);

	return p.mainLoop();
}