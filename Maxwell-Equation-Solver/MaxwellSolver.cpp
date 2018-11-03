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

	std::thread tr1([&]() {condenseThread(m1, v1, 0, 0);});
	std::thread tr2([&]() {condenseThread(m2, v2, 0, m); });
	std::thread tr3([&]() {condenseThread(m3, v3, m, 0); });
	std::thread tr4([&]() {condenseThread(m4, v4, m, m); });
	

	//condenseThread(m1, v, 0, 0);
	//condenseThread(m2, v, 0, m);
	//condenseThread(m3, v, m, 0);
	//condenseThread(m4, v, m, m);

	/*
	for (int i = 0; i < 2 * m; i++)

	{
		for (int j = 0; j < 2 * m; j++)

		{
			if (i > -1 && i < m && j > -1 && j < m)

			{
				double val = m1.coeff(i, j);
				if (val != 0.0) v.push_back(Triplet(i, j, val));
			}

			else if (i > m - 1 && i < 2 * m && j > -1 && j < m)

			{
				double val = m2.coeff(i - m, j);
				if (val != 0.0) v.push_back(Triplet(i, j, val));
			}

			else if (i > -1 && i < m && j > m - 1 && j < 2 * m)

			{
				double val = m3.coeff(i, j - m);
				if (val != 0.0) v.push_back(Triplet(i, j, val));
			}

			else if (i > m - 1 && i < 2 * m && j > m - 1 && j < 2 * m)

			{
				double val = m4.coeff(i - m, j - m);
				if (val != 0.0) v.push_back(Triplet(i, j, val));
			}
		}
	}
	*/
	

	
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

void MaxwellSolver::buildPerm()

{

	double val = 1.0;

	std::vector<double> perms;

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

	for (int i = 0; i < m; i++)

	{
		val = perms[i];

		coeffsPermX.push_back(Triplet(i, i, val));
		coeffsPermY.push_back(Triplet(i, i, val));
		coeffsPermZ.push_back(Triplet(i, i, val));

		coeffsPermXInverse.push_back(Triplet(i, i, 1.0 / val));
		coeffsPermYInverse.push_back(Triplet(i, i, 1.0 / val));
		coeffsPermZInverse.push_back(Triplet(i, i, 1.0 / val));
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

			/*
			//Ux

			if (superI % n != 0)

			{
				insertCoeff(coeffsUx, superI, index(i + 1, j), 1.0*(0.5 / deltaX));
			}
				
			else

			{
				insertCoeff(coeffsUx, superI, index(i + 1, j), 1.0*(0.5 / deltaX));
			}
				
				
			insertCoeff(coeffsUx, superI, index(i, j), -1.0*(0.5 / deltaX));
			//insertCoeff(coeffsUx, superI, index(i, j), -2.0*(0.5 / deltaY));
			//Uy
			if (superI != m - 1)

			{
				insertCoeff(coeffsUy, superI, index(i, j), -1.0*(0.5 / deltaY));
			}
				
			else

			{
				insertCoeff(coeffsUy, superI, index(i, j), 1.0*(0.5 / deltaY));
			}

			
			
			//insertCoeff(coeffsUy, superI, index(i, j), -2.0*(0.5 / deltaY));
			//Vx
			if (superI % n != 0)

			{
				insertCoeff(coeffsUy, superI, index(i + n, j), 1.0*(0.5 / deltaY));
				insertCoeff(coeffsVx, superI, index(i, j + 1), -1.0*(0.5 / deltaX));
				insertCoeff(coeffsVy, superI, index(i, j + n), -1.0*(0.5 / deltaY));
			}
			
			else

			{
				insertCoeff(coeffsUy, superI, index(i + n, j), 1.0*(0.5 / deltaY));
				insertCoeff(coeffsVx, superI, index(i, j + 1), -1.0*(0.5 / deltaX));
				insertCoeff(coeffsVy, superI, index(i, j + n), -1.0*(0.5 / deltaY));
			}
				
				
			insertCoeff(coeffsVx, superI, index(i, j), 1.0*(0.5 / deltaX));
			//insertCoeff(coeffsVx, superI, index(i, j), -2.0*(0.5 / deltaX));
			//Vy
			
			insertCoeff(coeffsVy, superI, index(i, j), 1.0*(0.5 / deltaY));
			//insertCoeff(coeffsVy, superI, index(i, j), -2.0*(0.5 / deltaY));
			//Identity Matrix
			*/

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
			/* //COPY
			//Ux
			insertCoeff(coeffsUx, superI, index(i + 1, j), 1.0*(1.0 / deltaX));
			insertCoeff(coeffsUx, superI, index(i, j), -1.0*(1.0 / deltaX));
			//Uy
			insertCoeff(coeffsUy, superI, index(i + (m / 2) + 1, j), 1.0*(1.0 / deltaY));
			double l;
			if (superI == m - 1)

			{
				l = 1.0;
			}
			else
			{
				l = -1.0;
			}
			insertCoeff(coeffsUy, superI, index(i, j), l*(1.0 / deltaY));
			//Vx
			insertCoeff(coeffsVx, superI, index(i, j + 1), -1.0*(1.0 / deltaX));
			insertCoeff(coeffsVx, superI, index(i, j), 1.0*(1.0 / deltaX));
			//Vy
			insertCoeff(coeffsVy, superI, index(i, j + (m / 2) + 1), -1.0*(1.0 / deltaY));
			insertCoeff(coeffsVy, superI, index(i, j), 1.0*(1.0 / deltaY));
			//Identity Matrix
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
	Vx.setFromTriplets(coeffsVx.begin(), coeffsVx.end());
	Vy.setFromTriplets(coeffsVy.begin(), coeffsVy.end());
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

void MaxwellSolver::findModes()

{
	std::cout << "Finding modes..." << std::endl;
	Clock c;
	Spectra::SparseGenMatProd<double> op(matrix);
	int nev = numEigs;
	Spectra::GenEigsSolver<double, Spectra::LARGEST_MAGN, Spectra::SparseGenMatProd<double>> eigs(&op, nev, nev + 2 + n/10);
	eigs.init();
	int nconv = eigs.compute();

	if (eigs.info() == Spectra::SUCCESSFUL)

	{
		eigenVals = eigs.eigenvalues().real();
		eigenVectors = eigs.eigenvectors().real();
	}

	std::cout << "Done in " << c.elapsed() << " ms" << std::endl;
	std::cout << std::setprecision(14) << eigenVals << std::endl;

	//std::cout << eigenVectors.col(0).size() << std::endl;
	
}