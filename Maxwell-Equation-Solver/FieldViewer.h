#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "MaxwellSolver.h"
#include "Color.h"
#include "Field.h"

class FieldViewer

{
public:
	FieldViewer(const FileHandler& fileH, Field fieldInit, std::vector<double> drawGeometry);
	FieldViewer(const FileHandler& fileH, Field fieldInit, std::vector<double> drawGeometry, std::vector<double> overlaps_);
	int mainLoop();
	void draw();
	void setMode(int mode);
	void writeFields();
	void normalise(Eigen::VectorXd& vec, std::vector<double>& normalisedVals);
	void keyCallBack(sf::Event events);
	void makeGeometryPoints(std::vector<double>& drawGeometry);
	void getColorScheme();
	double interpolate(double d1, double d2, double w);
	double getValue(std::vector<double>& gridPoints, int sideLengthX, int sideLengthY, int x, int y, int w, int h);

	

private:
	int w, h, modeSet, displayField, gOn, mode, eigs, overlapOn, textOn;
	std::vector<double> overlaps;
	std::vector<Color> colorMap;
	Field field;
	sf::RenderWindow window;
	FileHandler fileHandler;
	sf::VertexArray points, geometry;
	std::string windowText;
};