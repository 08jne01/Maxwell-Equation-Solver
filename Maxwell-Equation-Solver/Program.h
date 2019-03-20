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
	
	Program(int width, int height, std::string filename, int sweep, int changeneff, int neff); //Only need 1 constructor since we will call this once
	int mainLoop(); //Mainloop for program
	void draw(); //Draw Call
	int calculate(); //Calculate
	void setMode(int mode);
	void writeFields();
	void normalise(Eigen::VectorXd& vec, std::vector<double>& normalisedVals);
	void keyCallBack(sf::Event events);
	double interpolate(double d1, double d2, double w);
	double getValue(std::vector<double>& gridPoints, int sideLengthX, int sideLengthY, int x, int y, int w, int h); //

private:
	
	sf::RenderWindow window; //Sfml window
	sf::VertexArray points, geometry; //Array of points
	int w, h, eigs, displayField, mode, modeSet, gOn, willSweep; //Width, height
	std::vector<Eigen::MatrixXd> fieldComponents;
	Field field;
	Eigen::VectorXd eigenValues;
	FileHandler fileHandler;
};