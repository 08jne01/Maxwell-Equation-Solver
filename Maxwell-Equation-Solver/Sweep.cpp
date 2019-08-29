#include "Sweep.h"

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


	//int size = sqrt(field1.Ex.innerSize());


	for (int i = 0; i < solver.nx; i += 1)

	{
		for (int j = 0; j < solver.ny; j += 1)

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

	return std::abs(sum12*sum21)/std::abs(sum22*sum11);
}

double Sweep::lerp(double d1, double d2, double w)

{
	return d1 + w * (d2 - d1);
}

double Sweep::getIndex(double wavelength, int material)

{
	//
	int type = 0;

	std::vector<std::vector<double>> indexProfile;

	switch (material)

	{
	case RED_MATERIAL:
		type = redProfileType;
		if (!type) indexProfile = indexProfileRed;
		break;
	case GREEN_MATERIAL:
		type = greenProfileType;
		if (!type) indexProfile = indexProfileGreen;
		break;
	case BLUE_MATERIAL:
		type = blueProfileType;
		if (!type) indexProfile = indexProfileBlue;
		break;
	}

	if (type == 1)

	{
		switch (material)

		{
		case RED_MATERIAL:

		{
			double value = redParser.callFunction<double>({ wavelength });
			std::cout << value << std::endl;
			return value;
		}
			
		case GREEN_MATERIAL:
			return greenParser.callFunction<double>({ wavelength });
		case BLUE_MATERIAL:
			return blueParser.callFunction<double>({ wavelength });
		}
	}

	else if (type == -1)

	{
		switch (material)

		{
		case RED_MATERIAL:
			return fileHandler.config.maxIndexRed;
		case GREEN_MATERIAL:
			return fileHandler.config.maxIndexGreen;
		case BLUE_MATERIAL:
			return fileHandler.config.maxIndexBlue;
		}
	}

	else

	{
		wavelength *= fileHandler.config.profileMultiplier;

		double minWavelength, maxWavelength;
		minWavelength = indexProfile[0][0];
		maxWavelength = indexProfile[indexProfile.size() - 1][0];

		if (wavelength < minWavelength)

		{
			double n = indexProfile[0][1];
			std::cout << "Warning wavelength selected outside of index profile! Index: " << n << std::endl;
			return n;
		}

		if (wavelength > maxWavelength)

		{
			double n = indexProfile[indexProfile.size() - 1][1];
			std::cout << "Warning wavelength selected outside of index profile! Index: " << n << std::endl;
			return n;
		}

		int minIndex;
		for (int i = 0; i < indexProfile.size(); i++)

		{
			double diff = (wavelength - indexProfile[i][0]);
			if (diff <= 0)

			{
				minIndex = i - 1;
				break;
			}
		}

		double weight = wavelength - indexProfile[minIndex][0];
		double maxIndex = lerp(indexProfile[minIndex][1], indexProfile[minIndex + 1][1], weight);
		std::cout << "Using Max Index: " << maxIndex << std::endl;
		return maxIndex;
	}
}

void Sweep::wavelengthTrace(double startWave, double endWave, int steps)

{
	double waveStep = (endWave - startWave) / ((double)steps - 1);
	Clock clock;
	std::cout << "Initialising Sweep..." << std::endl;
	std::vector<double> perms;
	fileHandler.loadGeometry();
	fileHandler.getDrawGeometry(drawGeometry);
	fileHandler.getGeometry(perms);
	MaxwellSolver solver(fileHandler.config);
	solver.setPerms(perms);
	
	//solver.setPerms(fileHandler.config.maxIndex);
	
	if (fileHandler.config.profileOn)

	{
		if (fileHandler.config.indexProfileRed[0] == '$')

		{
			if (fileHandler.config.indexProfileRed[1] == '$') redProfileType = -1;
			else

			{
				std::string function = fileHandler.config.indexProfileRed.substr(1);
				std::cout << "Using function " << function << " for red" << std::endl;
				redParser.setFunction(function, { "lambda" });
				redProfileType = 1;
			}
		}

		else fileHandler.readCSV(fileHandler.config.indexProfileRed, indexProfileRed, 2, 1), redProfileType = 0;

		if (fileHandler.config.indexProfileGreen[0] == '$')

		{
			if (fileHandler.config.indexProfileGreen[1] == '$') greenProfileType = -1;
			else

			{
				std::string function = fileHandler.config.indexProfileGreen.substr(1);
				std::cout << "Using function " << function << " for green" << std::endl;
				greenParser.setFunction(function, { "lambda" });
				greenProfileType = 1;
			}
			
		}
		else fileHandler.readCSV(fileHandler.config.indexProfileGreen, indexProfileGreen, 2, 1), greenProfileType = 0;

		if (fileHandler.config.indexProfileBlue[0] == '$')

		{
			if (fileHandler.config.indexProfileBlue[1] == '$') blueProfileType = -1;
			else

			{
				std::string function = fileHandler.config.indexProfileBlue.substr(1);
				std::cout << "Using function " << function << " for blue" << std::endl;
				blueParser.setFunction(function, { "lambda" });
				blueProfileType = 1;
			}
			
		}
		else fileHandler.readCSV(fileHandler.config.indexProfileBlue, indexProfileBlue, 2, 1), blueProfileType = 0;
	}
		

	int prevMode = fileHandler.config.initMode;
	double prevEig, prevGuess;
	int start = 0;
	Field prevField, curField;

	prevGuess = fileHandler.config.neffGuess;

	for (int i = 0; i < steps; i++)

	{
		double wavelength = waveStep * i + startWave;
		solver.setWavelength(wavelength);
		if (fileHandler.config.profileOn) fileHandler.rebuildGeometry(solver.perms, getIndex(wavelength,  RED_MATERIAL), getIndex(wavelength, GREEN_MATERIAL), getIndex(wavelength, BLUE_MATERIAL));
		else fileHandler.rebuildGeometry(solver.perms, fileHandler.config.maxIndexRed, fileHandler.config.maxIndexGreen, fileHandler.config.maxIndexBlue);
		//solver.setPerms();
		solver.buildBoundaries();
		solver.buildMatrix();

		int success;
		//if (start != 0) 

		//{
			//double neff = sqrt(prevEig) / prevField.k;
			//prevGuess += (neff - prevGuess) / 10000.0;
			//std::cout << neff << " " << prevGuess << std::endl;
			//success = solver.findModes(pow(prevGuess * 2.*PI / wavelength, 2.));
			
		//}

		//else success = solver.findModes(pow(fileHandler.config.neffGuess * 2.*PI / wavelength, 2.));
		
		success = solver.findModes(pow(fileHandler.config.neffGuess * 2.*PI / wavelength, 2.));

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
				double prevOverlap = 0.0;
				double curOverlap;
				int closestMode = -1;

				std::vector<double> overlapsDiff;
				//overlapsDiff.resize(curField.Ex.outerSize());
				for (int i = 0; i < curField.Ex.outerSize(); i++)

				{
					curOverlap = overlap(prevField, prevMode, curField, i, solver);
					//std::cout << curOverlap << std::endl;
					overlapsDiff.push_back(curOverlap);
					if (curOverlap > prevOverlap)

					{
						closestMode = i;
						prevOverlap = curOverlap;
					}
				}
				
				if (prevOverlap < fileHandler.config.overlapConfidence)

				{
					std::cout << "Overlap < "<< fileHandler.config.overlapConfidence << ", Select Mode Manually" << std::endl;
					for (int i = 0; i < curField.Ex.outerSize(); i++)

					{
						std::cout << "Mode: " << i << " neff: " << sqrt(std::abs(curField.eigenValues[i])) / curField.k << " Overlap: " << overlapsDiff[i] << std::endl;
					}
					int selectedMode, inputSuccess;
					inputSuccess = 0;
					FieldViewer fieldViewer(fileHandler, curField, drawGeometry, overlapsDiff);
					selectedMode = fieldViewer.mainLoop();
					/*
					while (!inputSuccess)

					{
						//std::cout << "Error in input try again!" << std::endl;
						std::cout << "Select mode: ";
						std::cin >> selectedMode;

						if (std::cin.fail())

						{
							std::cin.clear();
							std::cin.ignore(32767, '\n');
							std::cout << "Error in input please try again!" << std::endl;
						}

						else

						{
							std::cin.ignore(32767, '\n');
							inputSuccess = 1;
						}
					}
					*/

					closestMode = selectedMode;
					if (closestMode == -1) break;
					prevOverlap = overlapsDiff[selectedMode];
				}
				


				std::cout << "Overlap: " << prevOverlap << std::endl;
				prevMode = closestMode;
				prevField = curField;

				
			}
			std::cout << "Best mode: " << prevMode << std::endl;
			prevEig = std::abs(prevField.eigenValues[prevMode]);
			//std::cout << prevEig << std::endl;
			double neffVal = sqrt(std::abs(prevField.eigenValues[prevMode])) / prevField.k;
			std::cout << std::setprecision(10) << "neff: " << neffVal << std::endl;
			neff.push_back(Vector2(wavelength, neffVal));
			std::cout << "Progress: " << (((double)i + 1.) / (double)steps)*100. << " %" << std::endl;
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