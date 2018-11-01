#pragma once
#include "Header.h"
//Class for PDE solver. We can split this later into other classes if it gets too large.
class SolverLaplace

{
public:

	SolverLaplace(int n);

	typedef Eigen::Triplet<double> triplet; //short-hand
	typedef Eigen::SparseMatrix<double> sparseMatrix; //short-hand

	void buildProblem(); //Setup
	void solveProblem(); //Solve
	void insertCoeff(int superI, int i, int j, double w, double source); //insert coefficient to matrix based on super index
	void saveProblem(); //Saves to variable
	void writeProblem(); //Writes to file
	void normalise();
	int index(int i, int j); //Converts i, j  to super index.
	double f(double x, double a, double b, double c); //Function to set sources

	std::vector<std::vector<double>> values; //Output values
	std::vector<std::vector<double>> valuesNormalised; //Normalised Values

private:
	int size, m; //number of grid points, number of grid points squared
	std::vector<triplet> coeffs; //coefficients for matrix
	sparseMatrix matrix;
	Eigen::VectorXd b; //matrix . x = b
	Eigen::VectorXd x; //Solving for x
};

//Super indexs go through all the points, with one number. Not to be confused with the x, y position in the grid.
//The matrix will have superindex superI and superJ for y and x respectively.
//Then when refering to position of the grid use i, j, except in unrelated loops.