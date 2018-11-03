#pragma once
#include "Header.h"
#include "Clock.h"

class MaxwellSolver

{
public:

	MaxwellSolver(int number, double kFreq, double length, double permativity, int eigs); //Size of one side of grid

	typedef Eigen::Triplet<double> Triplet;
	typedef Eigen::SparseMatrix<double> SparseM;

	int index(int i, int j);
	void condense(SparseM &m1, SparseM &m2, SparseM &m3, SparseM &m4, SparseM &returnMatrix);
	void condenseThread(SparseM &m1, std::vector<Triplet> &returnVec, int lowI, int lowJ);
	void insertCoeff(std::vector<Triplet> &matrixCoeffs, int superI, int superJ, double val);
	void buildPerm();
	void buildPotCoeffs();
	void buildMatrix();
	void findModes();

	Eigen::VectorXd eigenVals;
	Eigen::MatrixXd eigenVectors;

private:

	std::vector<Triplet> coeffsUx;
	std::vector<Triplet> coeffsUy;
	std::vector<Triplet> coeffsVx;
	std::vector<Triplet> coeffsVy;
	std::vector<Triplet> coeffsPermX;
	std::vector<Triplet> coeffsPermY;
	std::vector<Triplet> coeffsPermZ;
	std::vector<Triplet> coeffsPermXInverse;
	std::vector<Triplet> coeffsPermYInverse;
	std::vector<Triplet> coeffsPermZInverse;
	std::vector<Triplet> identity;

	SparseM matrix;
	
	int n, m, numEigs;
	double k, deltaX, deltaY, perm;
};