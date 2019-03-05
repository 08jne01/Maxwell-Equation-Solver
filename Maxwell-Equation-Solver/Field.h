#pragma once
#include <Eigen/SparseCore>
#include <string>

#define FIELD_EX 0
#define FIELD_EY 1
#define FIELD_EZ 2
#define FIELD_HX 3
#define FIELD_HY 4
#define FIELD_HZ 5

class Field

{
public:
	typedef Eigen::MatrixXd mat;
	typedef Eigen::VectorXd vec;

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

	struct UndefinedField : public std::exception

	{
		const char *what() const throw()

		{
			return "Exception: No field with that index!";
		}
	};

private:
};