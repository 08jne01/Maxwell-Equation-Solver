#include "MaxwellSolver.h"

MaxwellSolver::MaxwellSolver(int number, double kFreq, double length, double permativity, int eigs) 
	: n(number), m(number*number), k(kFreq), matrix(2 * (number*number), 2 * (number*number)), perm(permativity), numEigs(eigs)

{
	deltaX = length / (double)number;
	deltaY = length / (double)number;
}

int MaxwellSolver::index(int i, int j)

{
	return i + j * n;
}

void MaxwellSolver::condenseThread(SparseM &m1, std::vector<Triplet> &returnVec, int lowI, int lowJ)

{
	for (int k = 0; k < m1.outerSize(); k++)

	{
		//std::cout << m1.outerSize() << std::endl;
		for (Eigen::SparseMatrix<double>::InnerIterator it(m1, k); it; ++it)

		{
			//if (i > lowI - 1 && i < highI && j < lowJ - 1 && j > highJ)
			//{
			int i = it.col() + lowI;
			int j = it.row() + lowJ;
			//std::cout << lowI << std::endl;
			double val = it.value();
			if (i < 2*m && i > -1 && j < 2*m && j > -1) returnVec.push_back(Triplet(i, j, val));
			//}
		}
	}
}

void MaxwellSolver::condense(SparseM &m1, SparseM &m2, SparseM &m3, SparseM &m4, SparseM &returnMatrix)

{
	std::cout << "Condensing matricies..." << std::endl;
	Clock c;
	std::vector<Triplet> v;

	std::vector<Triplet> v1;
	std::vector<Triplet> v2;
	std::vector<Triplet> v3;
	std::vector<Triplet> v4;
	//Parallised
	std::thread tr1([&]() {condenseThread(m1, v1, 0, 0);});
	std::thread tr2([&]() {condenseThread(m2, v2, 0, m); });
	std::thread tr3([&]() {condenseThread(m3, v3, m, 0); });
	std::thread tr4([&]() {condenseThread(m4, v4, m, m); });
	
	tr1.join();
	tr2.join();
	tr3.join();
	tr4.join();

	v.reserve(v1.size() + v2.size() + v3.size() + v4.size());
	v.insert(v.end(), v1.begin(), v1.end());
	v.insert(v.end(), v2.begin(), v2.end());
	v.insert(v.end(), v3.begin(), v3.end());
	v.insert(v.end(), v4.begin(), v4.end());
	

	returnMatrix.setFromTriplets(v.begin(), v.end());
	std::cout << "Done in " << c.elapsed() << " ms" << std::endl;
}

void MaxwellSolver::insertCoeff(std::vector<Triplet> &matrixCoeffs, int superI, int superJ, double val)

{
	if (superI < m && superJ < m && superI > -1 && superJ > -1)

	{
		matrixCoeffs.push_back(Triplet(superI, superJ, val));
	}

	else if (superJ > m - 1 || superJ < 0)

	{
		matrixCoeffs.push_back(Triplet(superI, superI, 1.0));
	}
}

void MaxwellSolver::buildGeometry(double scale, std::string filename)

{
	//Allocate memory for filename and convert filename to char array so that string can be used
	char *filenameCharArray = (char*)malloc(sizeof(char) * (filename.size() + 1));
	strcpy_s(filenameCharArray, filename.size() + 1, filename.c_str());
	//Open file and create write buffer
	FILE *in;
	fopen_s(&in, filenameCharArray, "rb");
	unsigned char info[54];
	fread_s(info, 54 * sizeof(unsigned char), sizeof(unsigned char), 54, in);
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];
	int size = 3 * width * height;
	unsigned char* data = new unsigned char[size];
	fread_s(data, size * sizeof(unsigned char), sizeof(unsigned char), size, in);
	fclose(in);
	free(filenameCharArray);

	for (int i = 0; i < width*height; i++)

	{
		double val = scale*(double)(data[3 * (i)] + data[3 * (i)+1] + data[3 * (i)+2]) / (3.0*255) + 1.0;
		perms.push_back(val);
		//if (val != 1.0) std::cout << val << std::endl;
	}
	//Need to make a check for sizes!
	delete data;
	std::cout << filename << " geometry imported!" << std::endl;
}

void MaxwellSolver::buildPerm()

{

	double val = 1.0;
	//Default Case
	if (perms.size() <= 1)

	{
		for (int i = 0; i < n; i++)

		{
			for (int j = 0; j < n; j++)

			{
				if (i > n / 4 && i < 3 * n / 4 && j > n / 4 && j < 3 * n / 4)

				{
					//perms.push_back(1.00000001);
					perms.push_back(perm);
				}

				else

				{
					perms.push_back(1.0);
				}
			}
		}
	}
	//Set Perms
	for (int i = 0; i < n; i++)

	{
		for (int j = 0; j < n; j++)

		{
			double rx, ry, rz;

			int superI = index(i, j);
			
			if (i == 0 || j == 0 || i == n || j == n)

			{
				rx = perms[index(i, j)];
				ry = perms[index(i, j)];
				rz = perms[index(i, j)];
			}

			else

			{
				rx = (perms[index(i, j)] + perms[index(i,j - 1)]) / 2;
				ry = (perms[index(i, j)] + perms[index(i - 1, j)]) / 2;
				rz = (perms[index(i, j)] + perms[index(i - 1, j - 1)] + perms[index(i, j - 1)] + perms[index(i - 1, j)]) / 4;
			}

			//std::cout << rx << " " << ry << " " << rz << std::endl;

			coeffsPermX.push_back(Triplet(superI, superI, rx));
			coeffsPermY.push_back(Triplet(superI, superI, ry));
			coeffsPermZ.push_back(Triplet(superI, superI, rz));

			coeffsPermXInverse.push_back(Triplet(superI, superI, 1.0 / rx));
			coeffsPermYInverse.push_back(Triplet(superI, superI, 1.0 / ry));
			coeffsPermZInverse.push_back(Triplet(superI, superI, 1.0 / rz));
		}
		
	}
}

void MaxwellSolver::buildPotCoeffs()

{
	std::cout << "Building Boundary..." << std::endl;
	Clock c;
	for (int j = 0; j < n; j++)

	{
		for (int i = 0; i < n; i++)

		{
			int superI = index(i, j);
			insertCoeff(coeffsUx, superI, index(i, j), -1.0 / deltaX);
			insertCoeff(coeffsUx, superI, index(i + 1, j), 1.0 / deltaX);

			insertCoeff(coeffsUy, superI, index(i, j), -1.0 / deltaY);
			insertCoeff(coeffsUy, superI, index(i + n, j), 1.0 / deltaY);

			insertCoeff(identity, superI, superI, 1.0);
			/*
			//Ux
			insertCoeff(coeffsUx, superI, index(i + 1, j), 1.0*(1.0 / deltaX));
			insertCoeff(coeffsUx, superI, index(i - 1, j), -1.0*(1.0 / deltaX));
			//Uy
			insertCoeff(coeffsUy, superI, index(i, j + 1), 1.0*(1.0 / deltaY));
			double l;
			if (superI == m - 1)

			{
				l = 1.0;
			}
			else
			{
				l = -1.0;
			}
			insertCoeff(coeffsUy, superI, index(i, j - 1), l*(1.0 / deltaY));
			//Vx
			insertCoeff(coeffsVx, superI, index(i + 1, j), -1.0*(1.0 / deltaX));
			insertCoeff(coeffsVx, superI, index(i - 1, j), 1.0*(1.0 / deltaX));
			//Vy
			insertCoeff(coeffsVy, superI, index(i, j + 1), -1.0*(1.0 / deltaY));
			insertCoeff(coeffsVy, superI, index(i, j - 1), 1.0*(1.0 / deltaY));
			//Identity Matrix
			insertCoeff(identity, superI, superI, 1.0);
			insertCoeff(identity, superI, superI, 1.0);
			*/
		}
	}
	std::cout << "Done in "  << c.elapsed() << " ms" << std::endl;
}

void MaxwellSolver::buildMatrix()

{
	std::cout << "Building matricies..." << std::endl;
	Clock c;
	SparseM Pxx(m, m), Pxy(m, m), Pyx(m, m), Pyy(m, m), //Kill me
			Ux(m, m), Uy(m, m), Vx(m, m), Vy(m, m), erx(m, m),
			ery(m, m), erz(m, m), erxI(m, m), eryI(m, m),
			erzI(m, m), I(m, m);
	//Boundary
	Ux.setFromTriplets(coeffsUx.begin(), coeffsUx.end());
	Uy.setFromTriplets(coeffsUy.begin(), coeffsUy.end());
	//Vx.setFromTriplets(coeffsVx.begin(), coeffsVx.end());
	//Vy.setFromTriplets(coeffsVy.begin(), coeffsVy.end());

	Vx = -Ux.transpose();
	Vy = -Uy.transpose();

	//Permativities
	erx.setFromTriplets(coeffsPermX.begin(), coeffsPermX.end());
	ery.setFromTriplets(coeffsPermY.begin(), coeffsPermY.end());
	erz.setFromTriplets(coeffsPermZ.begin(), coeffsPermZ.end());
	//Inverse Permativities
	erxI.setFromTriplets(coeffsPermXInverse.begin(), coeffsPermXInverse.end());
	eryI.setFromTriplets(coeffsPermYInverse.begin(), coeffsPermYInverse.end());
	erzI.setFromTriplets(coeffsPermZInverse.begin(), coeffsPermZInverse.end());
	//Identity Matrix
	I.setFromTriplets(identity.begin(), identity.end());

	double kSqr = k * k;
	double kSqrI = 1.0 / kSqr;

	Pxx = -kSqrI * Ux*erzI*Vy*Vx*Uy + (kSqr*I + Ux * erzI*Vx)*(erx + kSqrI * Vy*Uy);
	Pyy = -kSqrI * Uy*erzI*Vx*Vy*Ux + (kSqr*I + Uy * erzI*Vy)*(ery + kSqrI * Vx*Ux);
	Pxy = Ux * erzI*Vy*(ery + kSqrI * Vx*Ux) - kSqrI * (kSqr*I + Ux * erzI*Vx)*Vy*Ux;
	Pyx = Uy * erzI*Vx*(erx + kSqrI * Vy*Uy) - kSqrI * (kSqr*I + Uy * erzI*Vy)*Vx*Uy;

	Pxx.makeCompressed();
	Pyy.makeCompressed();
	Pxy.makeCompressed();
	Pyx.makeCompressed();
	std::cout << "Done in " << c.elapsed() << " ms" << std::endl;

	condense(Pxx, Pxy, Pyx, Pyy, matrix);

	matrix.pruned();
	matrix.makeCompressed();
}

int MaxwellSolver::findModes()

{
	std::cout << "Finding modes..." << std::endl;
	Clock c;
	Spectra::SparseGenMatProd<double> op(matrix);
	int nev = numEigs;
	Spectra::GenEigsSolver<double, Spectra::LARGEST_REAL, Spectra::SparseGenMatProd<double>> eigs(&op, nev, 2*nev + m/10);
	eigs.init();
	int nconv = eigs.compute();

	if (eigs.info() == Spectra::SUCCESSFUL)

	{
		std::cout << "Successful!" << std::endl;
		eigenVals = eigs.eigenvalues().real();
		eigenVectors = eigs.eigenvectors().real();
		std::cout << "Done in " << c.elapsed() << " ms" << std::endl;
		return EXIT_SUCCESS;
	}

	else if (eigs.info() == Spectra::NOT_CONVERGING)

	{
		std::cout << "NOT CONVERGING!" << std::endl;
		return EXIT_FAILURE;
	}

	else if (eigs.info() == Spectra::NUMERICAL_ISSUE)

	{
		std::cout << "NUMERICAL ISSUE!" << std::endl;
		return EXIT_FAILURE;
	}
	
	//std::cout << std::setprecision(14) << eigenVals << std::endl;

	//std::cout << eigenVectors.col(0).size() << std::endl;
	
}