#include "MaxwellSolver.h"

MaxwellSolver::MaxwellSolver(int number, double kFreq, double length, double permativity, int eigs, int convNum) 
	: n(number), m(number*number), k(kFreq), matrix(2 * (number*number), 2 * (number*number)), perm(permativity*permativity), numEigs(eigs), nConv(convNum)

{
	deltaX = (double)length;
	deltaY = (double)length;
	std::cout << "Delta x, y: "  << deltaX << " " << deltaY << std::endl;
	//deltaX = 1.0;
	//deltaY = 1.0;
}

int MaxwellSolver::index(int i, int j)

{
	return i + j * n;
}

void MaxwellSolver::condenseThread(SparseM &m1, std::vector<Triplet> &returnVec, int lowI, int lowJ)

{
	for (int l = 0; l < m1.outerSize(); l++)

	{
		//std::cout << m1.outerSize() << std::endl;
		for (Eigen::SparseMatrix<double>::InnerIterator it(m1, l); it; ++it)

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
	std::thread tr2([&]() {condenseThread(m2, v2, m, 0); });
	std::thread tr3([&]() {condenseThread(m3, v3, 0, m); });
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
		double val1 = (double)(data[3 * (i)] + data[3 * (i) + 1] + data[3 * (i) + 2]) / (3.0);
		double val2 = (scale - 1.0)*val1/255.0 + 1.0;
		//if (val2 < 1.0) std::cout << val2 << std::endl;
		geometry.push_back(val1);
		perms.push_back(val2*val2);
		//std::cout << val2 << std::endl;
		//if (val2 != 1.0) std::cout << val2 << std::endl;
	}
	//Need to make a check for sizes!
	delete data;
	std::cout << filename << " geometry imported!" << std::endl;
}

Vector3 MaxwellSolver::getPermComponent(int i, int j)

{
	double rx, ry, rz;
	int superI = index(i, j);

	if (i == 0 || j == 0 || i == n || j == n)

	{
		rx = perms[superI];
		ry = perms[superI];
		rz = perms[superI];
	}

	else

	{
		//rx = (perms[superI] + perms[index(i, j - 1)]) / 2;
		//ry = (perms[superI] + perms[index(i - 1, j)]) / 2;
		//rz = (perms[superI] + perms[index(i - 1, j - 1)] + perms[index(i, j - 1)] + perms[index(i - 1, j)]) / 4;
		rx = perms[superI];
		ry = perms[superI];
		rz = perms[superI];
	}

	return Vector3(rx, ry, rz);
}

void MaxwellSolver::buildPerm()

{
	/*
	double val = 1.0;
	//Default Case
	//Set Perms
	
	for (int j = 0; j < n; j++)

	{
		for (int i = 0; i < n; i++)

		{
			int superI = index(i, j);
			Vector3 permVec(1.0, 1.0, 1.0);

			if (i > 59 && i < 99 && j > 59 && j < 99)

			{
				permVec = Vector3(perm, perm, perm);
			}

			coeffsPermX.push_back(Triplet(superI, superI, permVec.x));
			coeffsPermY.push_back(Triplet(superI, superI, permVec.y));

			coeffsPermZInverse.push_back(Triplet(superI, superI, 1.0 / permVec.z));
		}
	}
	
	*/

	for (int i = 0; i < n; i++)

	{
		for (int j = 0; j < n; j++)

		{
			int superI = index(i, j);
			Vector3 permVec = getPermComponent(i, j);

			//std::cout << rx << " " << ry << " " << rz << std::endl;

			coeffsPermX.push_back(Triplet(superI, superI, permVec.x));
			coeffsPermY.push_back(Triplet(superI, superI, permVec.y));

			coeffsPermZInverse.push_back(Triplet(superI, superI, 1.0 / permVec.z));
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
			/*
			insertCoeff(coeffsUx, superI, index(i, j),  (-1.0) / deltaX);
			insertCoeff(coeffsUx, superI, index(i + 1, j),  + (1.0) / deltaX);

			insertCoeff(coeffsUy, superI, index(i, j),  (-1.0) / deltaY);
			insertCoeff(coeffsUy, superI, index(i + n, j),  + (1.0) / deltaY);
			*/

			insertCoeff(coeffsUx, superI, superI, -1.0);
			insertCoeff(coeffsUx, superI + 1, superI, 1.0);

			insertCoeff(coeffsUy, superI, superI, -1.0);
			insertCoeff(coeffsUy, superI + n, superI, 1.0);

			insertCoeff(identity, superI, superI, 1.0);
			
			
		}
	}
	std::cout << "Done in "  << c.elapsed() << " ms" << std::endl;
}

void MaxwellSolver::buildMatrix()

{
	std::cout << "Building matricies..." << std::endl;
	Clock c;
	SparseM Pxx(m, m), Pxy(m, m), Pyx(m, m), Pyy(m, m), //Kill me
			Vx(m, m), Vy(m, m), erx(m, m),
			ery(m, m), erzI(m, m), I(m, m);

	Ux.resize(m, m);
	Uy.resize(m, m);
	//Boundary
	Ux.setFromTriplets(coeffsUx.begin(), coeffsUx.end());
	Uy.setFromTriplets(coeffsUy.begin(), coeffsUy.end());

	SparseM Ux_temp = Ux / deltaX;
	SparseM Uy_temp = Uy / deltaY;

	Ux = Ux_temp;
	Uy = Uy_temp;


	Vx = -Ux_temp.transpose();
	Vy = -Uy_temp.transpose();

	//Permativities
	erx.setFromTriplets(coeffsPermX.begin(), coeffsPermX.end());
	ery.setFromTriplets(coeffsPermY.begin(), coeffsPermY.end());
	//Inverse Permativities
	erzI.setFromTriplets(coeffsPermZInverse.begin(), coeffsPermZInverse.end());
	//Identity Matrix
	I.setFromTriplets(identity.begin(), identity.end());

	double kSqr = k * k;
	double kSqrI = 1.0 / kSqr;

	Pxx = -kSqrI * Ux*erzI*Vy*Vx*Uy + (kSqr*I + Ux * erzI*Vx)*(erx + kSqrI * Vy*Uy);
	Pyy = -kSqrI * Uy*erzI*Vx*Vy*Ux + (kSqr*I + Uy * erzI*Vy)*(ery + kSqrI * Vx*Ux);
	Pxy = Ux * erzI*Vy*(ery + kSqrI * Vx*Ux) - kSqrI * (kSqr*I + Ux * erzI*Vx)*Vy*Ux;
	Pyx = Uy * erzI*Vx*(erx + kSqrI * Vy*Uy) - kSqrI * (kSqr*I + Uy * erzI*Vy)*Vx*Uy; //erx - > ery in phils code

	//Pxx.prune(deltaX / k);
	//Pyy.prune(deltaX / k);
	//Pxy.prune(deltaX / k);
	//Pyx.prune(deltaX / k);
	std::cout << "Done in " << c.elapsed() << " ms" << std::endl;

	condense(Pxx, Pxy, Pyx, Pyy, matrix);

	//matrix.pruned();
	//matrix.makeCompressed();
}

int MaxwellSolver::findModes()

{
	std::cout << "Finding modes..." << std::endl;
	Clock c;
	//Spectra::SparseGenMatProd<double> op(matrix);
	int nev = numEigs;
	double sigma = k * k * perm*1.0;
	Clock c2;
	Spectra::SparseGenRealShiftSolve<double> op(matrix);
	Spectra::GenEigsRealShiftSolver<double, Spectra::LARGEST_MAGN, Spectra::SparseGenRealShiftSolve<double>> eigs(&op, nev, 2*nev + 1, sigma);
	//Spectra::GenEigsSolver<double, Spectra::LARGEST_REAL, Spectra::SparseGenMatProd<double>> eigs(&op, nev, 2 * nev + nConv);
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

void MaxwellSolver::findField()

{
	Clock c;
	typedef Eigen::SparseMatrix<std::complex<double>> SparseMComplex;
	typedef Eigen::Triplet<std::complex<double>> TripletComplex;

	std::cout << "Finding remaining field components..." << std::endl;
	int inner, outer;
	inner = eigenVectors.innerSize();
	outer = eigenVectors.outerSize();

	Ex.resize(inner / 2, outer);
	Ey.resize(inner / 2, outer);
	Ez.resize(inner / 2, outer);

	Hx.resize(inner / 2, outer);
	Hy.resize(inner / 2, outer);
	Hz.resize(inner / 2, outer);

	for (int i = 0; i < eigenVectors.outerSize(); i++)

	{
		Ex.col(i) = eigenVectors.col(i).head(eigenVectors.innerSize() / 2);
		Ey.col(i) = eigenVectors.col(i).tail(eigenVectors.innerSize() / 2);
	}

	SparseM erzI(m, m), erx(m, m), ery(m, m), UxCom(m, m), UyCom(m, m), Vx(m, m), Vy(m, m);
	//std::vector<Triplet> coeffComplexPermInverseZ, coeffComplexPermX, coeffComplexPermY;

	/*
	for (int i = 0; i < coeffsPermX.size(); i++)

	{
		int l = coeffsPermZInverse[i].col();
		int p = coeffsPermZInverse[i].row();
		std::complex<double> c(coeffsPermZInverse[i].value(), 0);
		coeffComplexPermInverseZ.push_back(TripletComplex(l, p, c));
	}

	for (int i = 0; i < coeffsPermX.size(); i++)

	{
		int l = coeffsPermX[i].col();
		int p = coeffsPermX[i].row();
		std::complex<double> c(coeffsPermX[i].value(), 0);
		coeffComplexPermX.push_back(TripletComplex(l, p, c));
	}

	for (int i = 0; i < coeffsPermX.size(); i++)

	{
		int l = coeffsPermY[i].col();
		int p = coeffsPermY[i].row();
		std::complex<double> c(coeffsPermY[i].value(), 0);
		coeffComplexPermY.push_back(TripletComplex(l, p, c));
	}

	std::vector<TripletComplex> complexUxVec, complexUyVec;

	for (int l = 0; l < Ux.outerSize(); l++)

	{
		for (Eigen::SparseMatrix<double>::InnerIterator it(Ux, l); it; ++it)

		{
			int i = it.col();
			int j = it.row();
			std::complex<double> val(it.value(), 0);
			if (i < 2 * m && i > -1 && j < 2 * m && j > -1) complexUxVec.push_back(TripletComplex(i, j, val));
		}
	}
	for (int l = 0; l < Ux.outerSize(); l++)

	{
		for (Eigen::SparseMatrix<double>::InnerIterator it(Uy, l); it; ++it)

		{
			int i = it.col();
			int j = it.row();
			std::complex<double> val(it.value(), 0);
			if (i < 2 * m && i > -1 && j < 2 * m && j > -1) complexUyVec.push_back(TripletComplex(i, j, val));
		}
	}
	*/


	double invImp = 1.0 / (120.0 * PI);

	erzI.setFromTriplets(coeffsPermZInverse.begin(), coeffsPermZInverse.end());
	erx.setFromTriplets(coeffsPermX.begin(), coeffsPermX.end());
	ery.setFromTriplets(coeffsPermY.begin(), coeffsPermY.end());

	//UxCom.setFromTriplets(complexUxVec.begin(), complexUxVec.end());
	//UyCom.setFromTriplets(complexUyVec.begin(), complexUyVec.end());
	Vx = -Ux.transpose();
	Vy = -Uy.transpose();

	double k0 = k;


	for (int i = 0; i < Ex.outerSize(); i++)

	{
		Hz.col(i) = (-1 / k0)*invImp*(-Uy * (Ex.col(i)) + Ux * (Ey.col(i)));
	}

	for (int i = 0; i < Hz.col(0).size(); i++)

	{
		//std::cout << (Vx * Hz.col(0))[i] / (k0 * ery*Ey.col(0))[i] << std::endl;
	}

	for (int i = 0; i < Ex.outerSize(); i++)

	{
		Hx.col(i) = (-1 / sqrt(abs(eigenVals[i])))*(Vx*Hz.col(i) - k0*ery*Ey.col(i));
	}

	for (int i = 0; i < Ex.outerSize(); i++)

	{
		Hy.col(i) = (-1 / sqrt(abs(eigenVals[i])))*(k0*erx*Ex.col(i) + Vy*Hz.col(i));
	}

	for (int i = 0; i < Ex.outerSize(); i++)

	{
		Ez.col(i) = cos(PI / 2)*(1 / k0)*erzI*(-Vy*Hx.col(i) + Vx*Hy.col(i));
	}

	std::cout << "Done in " << c.elapsed() << " ms" << std::endl;
}