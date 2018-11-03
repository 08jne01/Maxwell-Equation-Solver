#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <SFML/Graphics.hpp>
#include <Eigen/Sparse>
#include <Eigen/SparseCore>
#include <Eigen/Core>
#include <Spectra/SymEigsSolver.h>
#include <Spectra/GenEigsSolver.h>
#include <Spectra/MatOp/SparseGenMatProd.h>
#include <complex>
#include <ctime>
#include <iomanip>

#define PI 3.14159

//General header for includes and global scope definitions