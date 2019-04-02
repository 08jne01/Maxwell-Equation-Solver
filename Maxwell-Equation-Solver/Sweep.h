#pragma once
#include <vector>
#include "Vector2.h"
#include "FieldViewer.h"
#include "MaxwellSolver.h"

#define GEOMETRY 0
#define WAVELENGTH 1

class Sweep

{
public:
	Sweep(int type, FileHandler& fHandler);
	double overlap(Field& field1, int mode1, Field& field2, int mode2, MaxwellSolver& solver);
	void wavelengthTrace(double startWave, double endWave, int steps);
	void geometryTrace(std::vector<Vector2>& neff);
	void outputData(std::string filename);
	double getIndex(double wavelength);
	double lerp(double d1, double d2, double w);

private:
	FileHandler& fileHandler;
	int sweepType;
	std::vector<double> drawGeometry;
	std::vector<Vector2> neff;
	std::vector<std::vector<double>> indexProfile;
};