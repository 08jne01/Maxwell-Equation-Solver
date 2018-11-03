#pragma once
#include "Header.h"
#include "MaxwellSolver.h"
#include "Clock.h"
//Important to have class for main program to avoid global definition of variables which is in general bad.
class Program

{
public:
	
	Program(int width, int height, int iters, int eigVals, double kVal, double length, double permativity); //Only need 1 constructor since we will call this once
	int mainLoop(); //Mainloop for program
	void draw(); //Draw Call
	void calculate(int size); //Calculate
	void setMode(int mode);
	void normalise(std::vector<double> &vec, std::vector<double> &normalisedVals);
	double interpolate(double d1, double d2, double w);
	double getValue(std::vector<double> &gridPoints, int sideLength, int x, int y, int w, int h); //

private:
	
	sf::RenderWindow window; //Sfml window
	sf::VertexArray points; //Array of points
	int w, h, its, eigs; //Width, height
	double k, l, perm;
	Eigen::MatrixXd vectors;
	Eigen::VectorXd eigenValues;
};