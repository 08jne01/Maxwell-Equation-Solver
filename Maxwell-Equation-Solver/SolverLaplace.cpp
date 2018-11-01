#include "SolverLaplace.h"

SolverLaplace::SolverLaplace(int n): size(n), m(n*n), matrix(m, m), b(m)

{
	b.setZero(); //Sets b to all zeros
}

int SolverLaplace::index(int i, int j)

{
	return i + j * size;
}

void SolverLaplace::insertCoeff(int superI, int i, int j, double w, double source)

{
	
	if (i == size) //Check for boundary

	{
		b(superI) -= w*50.0; //Set boundary;
	}

	else if (i == -1)

	{
		b(superI) -= w*0.0;
	}

	else if (j == size)

	{
		b(superI) -= w*1000.0;
	}

	else if (j == -1)

	{
		b(superI) -= w*2000.0;
	}

	else

	{
		b(superI) -= source;
		coeffs.push_back(triplet(superI, index(i, j), w)); //Add triplet to coefficient vector
	}
}

void SolverLaplace::buildProblem()

{
	for (int j = 0; j < size; j++)

	{
		for (int i = 0; i < size; i++)

		{
			int superI = index(i, j);
			double h = 100.0;
			//double source = f((double)i / h, 100.0, 10.00, 0) * f((double)j / h, 0.4, -200, 0.0001);
			double source = 0;
			insertCoeff(superI, i + 1, j, 1.0, source); //Surrounding points for finite difference method
			insertCoeff(superI, i - 1, j, 1.0, source);
			insertCoeff(superI, i, j + 1, 1.0, source);
			insertCoeff(superI, i, j - 1, 1.0, source); //
			//Current Point
			insertCoeff(superI, i, j, -4.0, source);
		}
	}
}

void SolverLaplace::solveProblem() //Solves

{
	matrix.setFromTriplets(coeffs.begin(), coeffs.end());
	Eigen::SimplicialCholesky<sparseMatrix> chol(matrix);
	x = chol.solve(b);
}

void SolverLaplace::saveProblem() //Saves to values

{
	for (int j = 0; j < size; j++)

	{
		std::vector<double> buffer;
		for (int i = 0; i < size; i++)

		{
			buffer.push_back(x[index(i, j)]);
		}
		values.push_back(buffer);
	}
}

void SolverLaplace::writeProblem() //Writes to file

{
	std::ofstream file;
	file.open("Function.dat");
	for (int i = 0; i < values.size(); i++)

	{
		for (int j = 0; j < values[i].size(); j++)

		{
			file << i << "," << j << "," << values[i][j] << std::endl;
		}
	}
	file.close();
}

void SolverLaplace::normalise()

{
	double max = 0.0;

	for (int i = 0; i < m; i++)

	{
		if (x[i] > max)

		{
			max = x[i];
		}
	}

	std::cout << "Max: " << max << std::endl;

	for (int i = 0; i < size; i++)

	{
		std::vector<double> buffer;
		for (int j = 0; j < size; j++)

		{
			buffer.push_back(x[index(i, j)] / max);
		}
		valuesNormalised.push_back(buffer);
	}
}

double SolverLaplace::f(double x, double a, double b, double c)

{
	return a * sin(x*b)*sin(x*b)*exp(c*x);
}