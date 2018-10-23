#pragma once
#include "Header.h"
//Important to have class for main program to avoid global definition of variables which is in general bad.
class Program

{
public:
	
	Program(int width, int height); //Only need 1 constructor since we will call this once
	int mainLoop(); //Mainloop for program
	void draw(); //Draw Call
	void calculate(int size); //Calculate

private:
	
	sf::RenderWindow window; //Sfml window
	sf::VertexArray points; //Array of points
	int w, h; //Width, height 
};