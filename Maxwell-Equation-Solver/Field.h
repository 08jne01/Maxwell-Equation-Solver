#pragma once
#include <Eigen/SparseCore>
#include <string>
#include <iostream>

#define FIELD_EX 0
#define FIELD_EY 1
#define FIELD_EZ 2
#define FIELD_HX 3
#define FIELD_HY 4
#define FIELD_HZ 5

#define FIELD_TE 6
#define FIELD_TM 7

class Field

{
public:
	typedef Eigen::MatrixXd mat;
	typedef Eigen::VectorXd vec;

	//Field();
	//Field(mat ex, mat ey, mat ez, mat hx, mat hy, mat hz);
	//mat getField(int field);
	//std::string getFieldName(int field);
	//void setEigenValues(vec values);

	mat Ex;
	mat Ey;
	mat Ez;
	mat Hx;
	mat Hy;
	mat Hz;

	mat TE;
	mat TM;

	vec eigenValues;

	double k;
	double dx;
	double dy;

	std::string fieldNames[8] = { "Ex", "Ey", "Ez", "Hx", "Hy", "Hz", "Transverse Electric", "Transverse Magnetic"};

	struct UndefinedField : public std::exception

	{
		const char *what() const throw()

		{
			return "Exception: No field with that index!";
		}
	};

	Field()

	{

	}

	Field(mat ex, mat ey, mat ez, mat hx, mat hy, mat hz):
		Ex(ex), Ey(ey), Ez(ez),
		Hx(hx), Hy(hy), Hz(hz)
	{

	}

	void makeTEAndTM()

	{
		TE = Ex;
		TM = Ex;
		std::cout << Ex.outerSize() << std::endl;
		std::cout << Ex.innerSize() << std::endl;
		for (int i = 0; i < Ex.outerSize(); i++)

		{
			for (int j = 0; j < Ex.innerSize(); j++)

			{
				//std::cout << i << " " << j << std::endl;
				TE.coeffRef(j,i) = sqrt(Ex.col(i)[j] * Ex.col(i)[j] + Ey.col(i)[j] * Ey.col(i)[j]);
				TM.coeffRef(j,i) = sqrt(Hx.col(i)[j] * Hx.col(i)[j] + Hy.col(i)[j] * Hy.col(i)[j]);

				//TE(j, i) = 1.0;
				//TM.coeffRef(i, j) = sqrt(Hx.col(i)[j] * Hx.col(i)[j] + Hy.col(i)[j] * Hy.col(i)[j]);
			}
		}
	}

	std::string getFieldName(int field) const

	{
		return fieldNames[field];
	}

	Field::mat getField(int field) const

	{
		switch (field)

		{
		case FIELD_EX:
			return Ex;

		case FIELD_EY:
			return Ey;

		case FIELD_EZ:
			return Ez;

		case FIELD_HX:
			return Hx;

		case FIELD_HY:
			return Hy;

		case FIELD_HZ:
			return Hz;
		case FIELD_TE:
			return TE;
		case FIELD_TM:
			return TM;
		default:
			throw UndefinedField();
		}
	}

	void setEigenValues(vec values)

	{
		eigenValues = values;
	}

private:
};