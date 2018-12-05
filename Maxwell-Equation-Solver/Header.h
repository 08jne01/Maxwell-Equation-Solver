#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <SFML/Graphics.hpp>
#include <Eigen/Sparse>
#include <Eigen/SparseCore>
#include <Eigen/Core>
#include <Spectra/SymEigsSolver.h>
#include <Spectra/GenEigsSolver.h>
#include <Spectra/GenEigsRealShiftSolver.h>
#include <Spectra/MatOp/SparseGenRealShiftSolve.h>
#include <Spectra/SymEigsShiftSolver.h>
#include <Spectra/GenEigsComplexShiftSolver.h>
#include <Spectra/MatOp/SparseGenMatProd.h>
#include <complex>
#include <ctime>
#include <iomanip>
#include <Windows.h>

#define PI 3.14159

#define FIELD_HX 0
#define FIELD_HY 1
#define FIELD_HZ 2
#define FIELD_EX 3
#define FIELD_EY 4
#define FIELD_EZ 5

//General header for includes and global scope definitions