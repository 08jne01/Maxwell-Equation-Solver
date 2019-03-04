#pragma once
//#include "Header.h"
#include <iostream>
#include <thread>
#include <Eigen/Sparse>
#include <Spectra/GenEigsRealShiftSolver.h>
#include <Spectra/GenEigsComplexShiftSolver.h>
#include <Spectra/MatOp/SparseGenMatProd.h>
#include <Spectra/MatOp/SparseGenRealShiftSolve.h>
#include "Header.h"
#include "Field.h"
#include "Clock.h"
#include "Vector3.h"
#include "Config.h"

class MaxwellSolver

{
public:

	MaxwellSolver(Config conf); //Size of one side of grid
	~MaxwellSolver();

	typedef Eigen::Triplet<double> Triplet;
	typedef Eigen::SparseMatrix<double> SparseM;

	//Tools
	int index(int i, int j);
	void condense(SparseM &m1, SparseM &m2, SparseM &m3, SparseM &m4, SparseM &returnMatrix);
	void condenseThread(SparseM &m1, std::vector<Triplet> &returnVec, int lowI, int lowJ);
	void insertCoeff(std::vector<Triplet> &matrixCoeffs, int superI, int superJ, double val);
	Vector3 getPermComponent(int i, int j);

	//Initialisers
	void setWavelength(double wavelength);
	void buildBoundaries();
	void buildMatrix();
	int findModes(double sigma = -1.0);
	Field constructField();

	std::vector<double> perms;

	//
	//std::vector<double> geometry;

private:

	//Config
	Config config;
	//Coefficients
	std::vector<Triplet> coeffsUx;
	std::vector<Triplet> coeffsUy;
	std::vector<Triplet> coeffsUxSym;
	std::vector<Triplet> coeffsUySym;
	//std::vector<Triplet> coeffsVx;
	//std::vector<Triplet> coeffsVy;
	std::vector<Triplet> coeffsPermX;
	std::vector<Triplet> coeffsPermY;
	std::vector<Triplet> coeffsPermZInverse;
	std::vector<Triplet> coeffsIdentity;
	//Matrices
	SparseM matrix, Ux, Uy, Ux_sym, Uy_sym;
	//Results
	Eigen::VectorXd eigenVals;
	Eigen::MatrixXd eigenVectors;

	int n, m, numEigs, nConv;
	double k, deltaX, deltaY, perm;
};