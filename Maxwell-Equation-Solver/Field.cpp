#include "Field.h"

Field::Field()

{

}

Field::Field(mat ex, mat ey, mat ez, mat hx, mat hy, mat hz) :
	Ex(ex), Ey(ey), Ez(ez),
	Hx(hx), Hy(hy), Hz(hz)
{
	
}

std::string Field::getFieldName(int field)

{
	return fieldNames[field];
}

Field::mat Field::getField(int field)

{
	switch (field)

	{
		case FIELD_EX:

		{
			return Ex;
		}

		case FIELD_EY:

		{
			return Ey;
		}

		case FIELD_EZ:

		{
			return Ez;
		}

		case FIELD_HX:

		{
			return Hx;
		}

		case FIELD_HY:

		{
			return Hy;
		}

		case FIELD_HZ:

		{
			return Hz;
		}

		default:

		{
			throw UndefinedField();
		}
	}
}

void Field::setEigenValues(vec values)

{
	eigenValues = values;
}