#pragma once
#include <Eigen/SparseCore>
#include <string>
#include <Complex>

#define FIELD_EX 0
#define FIELD_EY 1
#define FIELD_EZ 2
#define FIELD_HX 3
#define FIELD_HY 4
#define FIELD_HZ 5

class Field

{
public:
	typedef std::complex<double> Complex;
	typedef Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic> mat;
	typedef Eigen::Matrix<Complex, Eigen::Dynamic, 1> vec;

	struct UndefinedField;

	Field();
	Field(mat ex, mat ey, mat ez, mat hx, mat hy, mat hz);
	mat getField(int field);
	std::string getFieldName(int field);
	void setEigenValues(vec values);

	mat Ex;
	mat Ey;
	mat Ez;
	mat Hx;
	mat Hy;
	mat Hz;

	vec eigenValues;

	double k;
	double dx;
	double dy;

	std::string fieldNames[6] = { "Ex", "Ey", "Ez", "Hx", "Hy", "Hz" };

private:
};