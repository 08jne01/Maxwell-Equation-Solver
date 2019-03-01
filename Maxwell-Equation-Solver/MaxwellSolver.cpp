#include "MaxwellSolver.h"

MaxwellSolver::MaxwellSolver(Config conf): config(conf)

{
	n = config.points;
	m = n * n;
	k = 2.0*PI / config.wavelength;
	matrix.resize(2 * m, 2 * m);
	perm = config.maxIndex*config.maxIndex;
	numEigs = config.numModes;
	nConv = config.convergance;
	deltaX = config.sizeOfStructure / config.numPointStructure;
	deltaY = deltaX;
}

MaxwellSolver::~MaxwellSolver()

{
	//Clear up just in case
	eigenVals.resize(0);
	eigenVectors.resize(0, 0);
	perms.clear();
	coeffsUx.clear();
	coeffsUy.clear();
	coeffsPermX.clear();
	coeffsPermY.clear();
	coeffsPermZInverse.clear();
	matrix.resize(0, 0);
}

void MaxwellSolver::setWavelength(double wavelength)

{
	k = 2.0*PI / wavelength;
}

int MaxwellSolver::index(int i, int j)

{
	return i + j * n;
}

void MaxwellSolver::condenseThread(SparseM &m1, std::vector<Triplet> &returnVec, int lowI, int lowJ)

{	//Iterates through each value and puts it into a new triplet vector based on position and lowJ and lowI
	//It seems like this would be slow but Eigen3 has a special iterator for this.
	for (int l = 0; l < m1.outerSize(); l++)

	{
		for (Eigen::SparseMatrix<Complex>::InnerIterator it(m1, l); it; ++it)

		{
			int i = it.col() + lowJ;
			int j = it.row() + lowI;
			Complex val = it.value();
			if (i < 2*m && i > -1 && j < 2*m && j > -1) returnVec.push_back(Triplet(j, i, val));
		}
	}
}

void MaxwellSolver::condense(SparseM &m1, SparseM &m2, SparseM &m3, SparseM &m4, SparseM &returnMatrix)

{
	if (config.timers == 1) std::cout << "Condensing matricies..." << std::endl;
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
	//Join threads simulatenously
	tr1.join();
	tr2.join();
	tr3.join();
	tr4.join();
	//Reserve space for final vector
	v.reserve(v1.size() + v2.size() + v3.size() + v4.size());
	//Insert vectors
	v.insert(v.end(), v1.begin(), v1.end());
	v.insert(v.end(), v2.begin(), v2.end());
	v.insert(v.end(), v3.begin(), v3.end());
	v.insert(v.end(), v4.begin(), v4.end());
	//Set P (matrix) from triplets
	returnMatrix.setFromTriplets(v.begin(), v.end());
	if (config.timers == 1) std::cout << "Done in " << c.elapsed() << " ms" << std::endl;
}

void MaxwellSolver::insertCoeff(std::vector<Triplet> &matrixCoeffs, int superI, int superJ, Complex val)

{
	if (superI < m && superJ < m && superI > -1 && superJ > -1)

	{
		//Insert coefficient if it is in bounds
		matrixCoeffs.push_back(Triplet(superI, superJ, val));
	}
}

Vector3 MaxwellSolver::getPermComponent(int i, int j)

{
	double rx, ry, rz;
	int superI = index(i, j);
	//Check if it is on the boundary
	if (i == 0 || j == 0 || i == n || j == n)

	{
		rx = perms[superI];
		ry = perms[superI];
		rz = perms[superI];
	}

	else

	{	//Averages across 2 points (stair case approximation)
		rx = (perms[superI] + perms[index(i, j - 1)]) / 2;
		ry = (perms[superI] + perms[index(i - 1, j)]) / 2;
		rz = (perms[superI] + perms[index(i - 1, j - 1)] + perms[index(i, j - 1)] + perms[index(i - 1, j)]) / 4;
		//rx = perms[superI];
		//ry = perms[superI];
		//rz = perms[superI];
	}

	return Vector3(rx, ry, rz);
}

MaxwellSolver::Complex MaxwellSolver::sVal(double x, double l, double aMax, double sigmaMax)

{
	double a = 1. + aMax * pow(x / l, 2.5);
	double sigma = sigmaMax * pow(sin(PI*x / (2.*l)), 2.);
	return Complex(a, a*sigma);
}

void MaxwellSolver::buildBoundaries()

{
	if (config.timers == 1) std::cout << "Building Boundary..." << std::endl;
	Clock c;

	Complex sx, sy;

	double pmlSize = 10.;

	for (int j = 0; j < n; j++)

	{
		for (int i = 0; i < n; i++)

		{
			int superI = index(i, j);

			if (j < pmlSize)

			{
				sx = sVal(-(double)j + pmlSize - 1., pmlSize, 0.1, 1.);
				//std::cout << "left " << sx << std::endl;
				//sx = Complex(1.0, 0.0);
			}

			else if (j > n - pmlSize - 1.)

			{
				sx = sVal((double)j - (double)n + pmlSize, pmlSize, 0.1, 1.);
				//std::cout << "right " << sx << std::endl;
			}

			else

			{
				sx = Complex(1.0, 0.0);
			}

			if (i < pmlSize)

			{
				sy = sVal(-(double)i + pmlSize, pmlSize, 0.1, 1.);
				
			}

			else if (i > n - pmlSize - 1.)

			{
				sy = sVal((double)i - (double)n + pmlSize, pmlSize, 0.1, 1.);
			}

			else

			{
				sy = Complex(1.0, 0.0);
			}

			//sx = Complex(1.0, 0.0);
			//sy = Complex(1.0, 0.0);

			//Set Permativities
			Vector3 permVec = getPermComponent(i, j);

			coeffsPermX.push_back(Triplet(superI, superI, permVec.x*pow(sy/sx, 2.0)));
			coeffsPermY.push_back(Triplet(superI, superI, permVec.y*pow(sx/sy, 2.)));
			coeffsPermZInverse.push_back(Triplet(superI, superI, 1. / (permVec.z*pow(sx*sy, 2.))));

			//coeffsPermHX.push_back(Triplet(superI, superI, sy / sx));
			//coeffsPermHY.push_back(Triplet(superI, superI, sx / sy));
			//coeffsPermHZInverse.push_back(Triplet(superI, superI, 1. / (sx*sy)));

			//Set Window Boundary conditions
			

			/*

			if (j == 0)

			{
				//std::cout << "Test" << std::endl;
				//insertCoeff(coeffsUx, superI, superI, -1.0);
				//insertCoeff(coeffsUx, superI, superI + 1, 1.0);
				//Symetric
				insertCoeff(coeffsUxSym, superI, superI, -1.0);
				insertCoeff(coeffsUxSym, superI, superI + 1, 1.0);
			}

			else if ((superI + 1) % n != 0)

			{
				//insertCoeff(coeffsUx, superI, superI, -1.0);
				//insertCoeff(coeffsUx, superI, superI + 1, 1.0);
				//Symetric
				insertCoeff(coeffsUxSym, superI, superI, -1.0);
				insertCoeff(coeffsUxSym, superI, superI + 1, 1.0);
			}

			if (j == 0)

			{
				//insertCoeff(coeffsUy, superI, superI, -1.0);
				//insertCoeff(coeffsUy, superI, superI + n, 1.0);
				//Symetric
				insertCoeff(coeffsUySym, superI, superI, -1.0);
				insertCoeff(coeffsUySym, superI, superI + n, 1.0);
			}

			else

			{
				//insertCoeff(coeffsUy, superI, superI, -1.0);
				//insertCoeff(coeffsUy, superI, superI + n, 1.0);
				//Symetric
				insertCoeff(coeffsUySym, superI, superI, -1.0);
				insertCoeff(coeffsUySym, superI, superI + n, 1.0);
			}

			
			
			
			if ((superI + 1) % n != 0)

			{
				insertCoeff(coeffsUx, superI, superI, -1.0);
				insertCoeff(coeffsUx, superI, superI + 1, 1.0);
				
			}

			*/

			insertCoeff(coeffsUx, superI, superI, Complex(-1.0, 0.0));
			if ((superI + 1) % n != 0) insertCoeff(coeffsUx, superI, superI + 1, Complex(1.0, 0.0));
			insertCoeff(coeffsUy, superI, superI, Complex(-1.0,0.0));
			insertCoeff(coeffsUy, superI, superI+n, Complex(1.0, 0.0));
			

			//if (j != 0) insertCoeff(coeffsIdentity, superI, superI, 1.0);
			
		}
	}
	if (config.timers == 1) std::cout << "Done in "  << c.elapsed() << " ms" << std::endl;
}

void MaxwellSolver::buildMatrix()

{
	if (config.timers == 1) std::cout << "Building matricies..." << std::endl;
	Clock c;
	SparseM Pxx(m, m), Pxy(m, m), Pyx(m, m), Pyy(m, m), //Kill me
			Vx(m, m), Vy(m, m), erx(m, m),
			ery(m, m), erzI(m, m), urx(m,m), ury(m,m), urzI(m,m), I(m, m), I_sym(m,m);
	//Resize boundary matrices
	Ux.resize(m, m);
	Uy.resize(m, m);
	//Ux_sym.resize(m, m);
	//Uy_sym.resize(m, m);
	//Boundary
	Ux.setFromTriplets(coeffsUx.begin(), coeffsUx.end());
	Uy.setFromTriplets(coeffsUy.begin(), coeffsUy.end());
	//Ux_sym.setFromTriplets(coeffsUxSym.begin(), coeffsUxSym.end());
	//Uy_sym.setFromTriplets(coeffsUySym.begin(), coeffsUySym.end());
	//Clear coeffs from memory
	coeffsUx.clear();
	coeffsUy.clear();
	//coeffsUxSym.clear();
	//coeffsUySym.clear();

	//Vx.setFromTriplets(coeffsVx.begin(), coeffsVx.end());
	//Vy.setFromTriplets(coeffsVy.begin(), coeffsVy.end());
	
	//Set grid spacing
	SparseM Ux_temp = Ux / deltaX;
	SparseM Uy_temp = Uy / deltaY;
	Ux = Ux_temp;
	Uy = Uy_temp;


	//Setup Vx and Vy
	Vx = -Ux_temp.transpose();
	Vy = -Uy_temp.transpose();

	//Ux_temp = Ux_sym / deltaX;
	//Uy_temp = Uy_sym / deltaY;
	//Ux_sym = Ux_temp; 
	//Uy_sym = Uy_temp;

	//Ux = Ux_sym;
	//Uy = Uy_sym;
	

	//Permativities
	erx.setFromTriplets(coeffsPermX.begin(), coeffsPermX.end());
	ery.setFromTriplets(coeffsPermY.begin(), coeffsPermY.end());
	//Inverse Permativities
	erzI.setFromTriplets(coeffsPermZInverse.begin(), coeffsPermZInverse.end());
	//Identity Matrix
	//I.setIdentity();
	I.setFromTriplets(coeffsIdentity.begin(), coeffsIdentity.end());
	I.setIdentity();
	I_sym.setIdentity();

	double kSqr = k * k;

	Pxx = I * (-(Ux*erzI*Vy*Vx*Uy) / kSqr + (kSqr*I + Ux * erzI*Vx)*(erx + (Vy*Uy) / kSqr));
	Pyy = -(Uy*erzI*Vx*Vy*Ux) / kSqr + (kSqr*I_sym + Uy * erzI*Vy)*(ery + (Vx*Ux) / kSqr);
	Pxy = I * ((Ux * erzI*Vy)*(ery + (Vx*Ux) / kSqr) - ((kSqr*I + Ux * erzI*Vx)*(Vy*Ux)) / kSqr);
	Pyx = (Uy * erzI*Vx)*(erx + (Vy*Uy) / kSqr) - ((kSqr*I_sym + Uy * erzI*Vy)*(Vx*Uy)) / kSqr;


	//Pxx.prune(1.0 / k);
	//Pyy.prune(1.0 / k);
	//Pxy.prune(1.0 / k);
	//Pyx.prune(1.0 / k);

	//std::cout << Pxx << std::endl;
	//std::cout << Pxy << std::endl;
	//std::cout << Pyx << std::endl;
	//std::cout << Pyy << std::endl;

	if (config.timers == 1) std::cout << "Done in " << c.elapsed() << " ms" << std::endl;

	condense(Pxx, Pxy, Pyx, Pyy, matrix);
	//std::cout << kSqr << std::endl;
	
	//matrix.pruned();
	//matrix.makeCompressed();
}

int MaxwellSolver::findModes(double sigma)

{
	if (config.timers == 1) std::cout << "Finding modes..." << std::endl;
	Clock c;
	int nev = numEigs;
	//Initial Guess for eigen solver

	if (sigma < -0.9 && sigma > -1.1)

	{
		sigma = k * k * perm*1.0;
	}
	SparseGenComplexShiftSolve<double> op(matrix);
	//Spectra::SparseGenRealShiftSolve<double> op(matrix); //n/10
	//Spectra::GenEigsRealShiftSolver<double, Spectra::LARGEST_MAGN, Spectra::SparseGenRealShiftSolve<double>> eigs(&op, nev, 2*nev + 1 + n/10, sigma);
	Spectra::GenEigsComplexShiftSolver<double, Spectra::LARGEST_MAGN, SparseGenComplexShiftSolve<double>> eigs(&op, nev, 2 * nev + 1 + n / 10, sigma, 0.0);
	//Initalise eigen solver and compute
	eigs.init();
	int nconv = eigs.compute();

	//Check if successful
	if (eigs.info() == Spectra::SUCCESSFUL)

	{
		std::cout << "Successful!" << std::endl;
		eigenVals = eigs.eigenvalues();
		eigenVectors = eigs.eigenvectors();
		if (config.timers == 1) std::cout << "Done in " << c.elapsed() << " ms" << std::endl;
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
}

Field MaxwellSolver::constructField()

{
	Clock c;
	typedef Eigen::SparseMatrix<std::complex<double>> SparseMComplex;
	typedef Eigen::Triplet<std::complex<double>> TripletComplex;

	if (config.timers == 1) std::cout << "Finding remaining field components..." << std::endl;
	//Get sizes
	int inner, outer;
	inner = eigenVectors.innerSize() / 2.0;
	outer = eigenVectors.outerSize();

	Field field;

	//Set Eigen Values;
	field.setEigenValues(eigenVals);
	field.k = k;
	field.dx = deltaX;
	field.dy = deltaY;

	//Resize fields
	field.Ex.resize(inner, outer);
	field.Ey.resize(inner, outer);
	field.Ez.resize(inner, outer);
	field.Hx.resize(inner, outer);
	field.Hy.resize(inner, outer);
	field.Hz.resize(inner, outer);

	//Split output into Ex field and Ey field
	for (int i = 0; i < eigenVectors.outerSize(); i++)

	{
		field.Ex.col(i) = eigenVectors.col(i).head(inner);
		field.Ey.col(i) = eigenVectors.col(i).tail(inner);

		//field.Ex.col(i).normalize();
		//field.Ey.col(i).normalize();
	}

	//Remake Matrices for operations
	SparseM erzI(m, m), erx(m, m), ery(m, m), UxCom(m, m), UyCom(m, m), Vx(m, m), Vy(m, m);
	erzI.setFromTriplets(coeffsPermZInverse.begin(), coeffsPermZInverse.end());
	erx.setFromTriplets(coeffsPermX.begin(), coeffsPermX.end());
	ery.setFromTriplets(coeffsPermY.begin(), coeffsPermY.end());
	Vx = -Ux.transpose();
	Vy = -Uy.transpose();

	//Clear vectors used for last time
	coeffsPermZInverse.clear();
	coeffsPermX.clear();
	coeffsPermY.clear();

	double k0 = k;

	Complex im = Complex(0.0, 1.0);

	//Construct field components
	for (int i = 0; i < outer; i++)

	{
		field.Hz.col(i) = (- im/ k0)*(-Uy *(field.Ex.col(i)) + Ux * (field.Ey.col(i)));
		field.Hx.col(i) = (- im/ sqrt(abs(eigenVals[i])))*(Vx*field.Hz.col(i) - im*k0 * ery*field.Ey.col(i));
		field.Hy.col(i) = (- im/ sqrt(abs(eigenVals[i])))*(im*k0*erx*field.Ex.col(i) + Vy * field.Hz.col(i));
		field.Ez.col(i) = (im / k0)*erzI*(-Vy * field.Hx.col(i) + Vx * field.Hy.col(i));

		//field.Hz.col(i).normalize();
		//field.Hx.col(i).normalize();
		//field.Hy.col(i).normalize();
		//field.Ez.col(i).normalize();
	}

	for (int i = 0; i < outer; i++)

	{
		field.Hz.col(i).normalize();
		field.Hx.col(i).normalize();
		field.Hy.col(i).normalize();
		field.Ez.col(i).normalize();
		field.Ex.col(i).normalize();
		field.Ey.col(i).normalize();
	}


	//Clear boundary matrices used for last time
	Ux.resize(0, 0);
	Uy.resize(0, 0);

	if (config.timers == 1) std::cout << "Done in " << c.elapsed() << " ms" << std::endl;

	return field;
	
}