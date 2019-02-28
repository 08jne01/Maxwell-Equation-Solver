#pragma once

#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <Eigen/SparseLU>
#include <stdexcept>
#include <complex>

template <typename Scalar, int Flags = 0, typename StorageIndex = int>
class SparseGenComplexShiftSolve
{
private:
	typedef std::complex<Scalar> Complex;
	typedef Eigen::Matrix<Scalar, Eigen::Dynamic, 1> Vector;
	typedef Eigen::Map<const Vector> MapConstVec;
	typedef Eigen::Map<Vector> MapVec;
	typedef Eigen::SparseMatrix<Scalar> SparseMatrix;
	typedef const Eigen::Ref<const SparseMatrix> ConstGenericSparseMatrix;

	typedef Eigen::SparseMatrix<Complex> ComplexMatrix;
	typedef Eigen::Matrix<Complex, Eigen::Dynamic, 1> ComplexVector;

	const SparseMatrix& m_mat;
	const int m_n;
	Eigen::SparseLU<ComplexMatrix> m_solver;
	ComplexVector m_x_cache;

public:
	SparseGenComplexShiftSolve(const ConstGenericSparseMatrix& mat_) : m_mat(mat_), m_n(mat_.rows())
	{
		if (mat_.rows() != mat_.cols())
			throw std::invalid_argument("SparseGenRealShiftSolve: matrix must be square");
	}

	int rows() const { return m_n; }

	int cols() const { return m_n; }

	void set_shift(Scalar sigmar, Scalar sigmai)
	{
		ComplexMatrix I(m_n, m_n);
		I.setIdentity();

		m_solver.compute(m_mat.template cast<Complex>() - Complex(sigmar, sigmai) * I);
		m_x_cache.resize(m_n);
		m_x_cache.setZero();
	}

	// y_out = inv(A - sigma * I) * x_in
	void perform_op(const Scalar* x_in, Scalar* y_out)
	{
		m_x_cache.real() = MapConstVec(x_in, m_n);
		MapVec y(y_out, m_n);
		y.noalias() = m_solver.solve(m_x_cache).real();
	}
};