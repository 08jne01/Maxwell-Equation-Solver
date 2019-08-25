#pragma once
//#include "Header.h"
#include <SFML/Graphics.hpp>
#include "FileHandler.h"
#include "Sweep.h"
#include "Clock.h"

//Important to have class for main program to avoid global definition of variables which is in general bad.
class Program

{
public:
	
	Program(std::string filename, int sweep, int changeneff, int neff); //Only need 1 constructor since we will call this once
	int mainLoop(); //Mainloop for program
	void draw(); //Draw Call
	int calculate(std::vector<double>& localGeometry); //Calculate

private:
	
	sf::RenderWindow window; //Sfml window
	sf::VertexArray points, geometry; //Array of points
	int w, h, eigs, displayField, mode, modeSet, gOn, willSweep; //Width, height
	Field field;
	FileHandler fileHandler;
	sf::Font font;
};