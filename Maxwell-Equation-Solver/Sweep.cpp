#include "Sweep.h"
/*
Sweep::Sweep(int type, FileHandler& fHandler): fileHandler(fHandler)

{

}

double Sweep::overlap(Field& field1, int mode1, Field& field2, int mode2, MaxwellSolver& solver)

{
	double xSum12 = 0.0;
	double xSum21 = 0.0;
	double xSum11 = 0.0;
	double xSum22 = 0.0;
	double sum12 = 0.0;
	double sum21 = 0.0;
	double sum11 = 0.0;
	double sum22 = 0.0;


	int size = sqrt(field1.Ex.innerSize());


	for (int i = 0; i < size; i += 1)

	{
		for (int j = 0; j < size; j += 1)

		{

			int pos = solver.index(i, j);

			double Ex1 = field1.Ex.col(mode1)[pos];
			double Ey1 = field1.Ey.col(mode1)[pos];
			double Hx1 = field1.Hx.col(mode1)[pos];
			double Hy1 = field1.Hy.col(mode1)[pos];

			double Ex2 = field2.Ex.col(mode2)[pos];
			double Ey2 = field2.Ey.col(mode2)[pos];
			double Hx2 = field2.Hx.col(mode2)[pos];
			double Hy2 = field2.Hy.col(mode2)[pos];

			xSum12 += (Ex1*Hy2 - Hx2 * Ey1);
			xSum21 += (Ex2*Hy1 - Hx1 * Ey2);
			xSum11 += (Ex1*Hy1 - Hx1 * Ey1);
			xSum22 += (Ex2*Hy2 - Hx2 * Ey2);
		}
		sum12 += xSum12;
		sum21 += xSum21;
		sum11 += xSum11;
		sum22 += xSum22;
	}

	return abs(sum12*sum21)/abs(sum22*sum11);
}

void Sweep::wavelengthTrace(double startWave, double endWave, int steps)

{
	double waveStep = (endWave - startWave) / (double)steps;
	Clock clock;
	std::cout << "Initialising Sweep..." << std::endl;
	MaxwellSolver solver(fileHandler.config);
	fileHandler.getGeometry(solver.perms, drawGeometry);
	
	

	int prevMode = 0;
	double prevEig;
	int start = 0;
	Field prevField, curField;

	for (int i = 0; i < steps; i++)

	{
		double wavelength = waveStep * i + startWave;
		solver.setWavelength(wavelength);
		solver.buildBoundaries();
		solver.buildMatrix();

		int success;
		//if (start != 0) success = solver.findModes(prevEig);
		//else success = solver.findModes(pow(0.99999999 * 2.*PI / wavelength, 2.));
		success = solver.findModes(pow(0.99999999 * 2.*PI / wavelength, 2.));

		if (success == EXIT_SUCCESS)

		{
			curField = solver.constructField();
			std::cout << "========================" << std::endl;
			std::cout << "Success for Wavelength: " << wavelength << std::endl;

			if (start == 0)

			{
				prevField = curField;
				start = 1;
			}

			else

			{
				double prevOverlap, curOverlap;
				int closestMode;

				for (int i = 0; i < curField.Ex.outerSize(); i++)

				{
					curOverlap = overlap(prevField, prevMode, curField, i, solver);
					
					if (curOverlap > prevOverlap)

					{
						closestMode = i;
						prevOverlap = curOverlap;
					}
					
					
				}
				std::cout << "Overlap: " << prevOverlap << std::endl;
				prevMode = closestMode;
				prevField = curField;

				
			}
			std::cout << "Best mode: " << prevMode << std::endl;
			prevEig = abs(prevField.eigenValues[prevMode]);
			//std::cout << prevEig << std::endl;
			double neffVal = sqrt(abs(prevField.eigenValues[prevMode])) / prevField.k;
			std::cout << std::setprecision(10) << "neff: " << neffVal << std::endl;
			neff.push_back(Vector2(wavelength, neffVal));
			std::cout << "Progress: " << (double)i / (double)steps << std::endl;
			std::cout << "========================" << std::endl;
		}
	}

	std::cout << "All done in " << clock.elapsed() << " ms" << std::endl;
}

void Sweep::outputData(std::string filename)

{
	std::ofstream file(filename);
	
	if (file.is_open())

	{
		file << "wavelength, neff" << std::endl;
		for (int i = 0; i < neff.size(); i++)

		{
			file << neff[i].x << "," << std::setprecision(10) << neff[i].y << std::endl;
		}

		file.close();
		std::cout << "Successfully written data to file!" << std::endl;
	}

	else

	{
		std::cout << "ERROR COULD NOT WRITE TO FILE!" << std::endl;
	}
	
}

void Sweep::geometryTrace(std::vector<Vector2>& neff)

{

}
*/