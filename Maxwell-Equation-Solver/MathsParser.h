#pragma once
#include <string>
#include <sstream>
#include <vector>
	//Operators: +, -, /, *, ^, sqrt

#define OPERATOR_PLUS 0
#define OPERATOR_MINUS 1
#define OPERATOR_DIVIDE 2
#define OPERATOR_MULTIPLY 3
#define OPERATOR_POWER 4
#define OPERATOR_SQUAREROOT 5

#define ERROR_NON_NUMERIC 1
#define ERROR_MISSING_CLOSING_BRACKET 2
#define ERROR_EXTRA_CLOSING_BRACKET 3
#define ERROR_NUM_VARS_NOT_EQUAL_NUM_ARGS 4
#define ERROR_VARIABLE_NOT_FOUND_IN_FUNCTION 5

class MathsParser

{
public:
	MathsParser()

	{

	}

	std::string getErrorMessage()

	{
		switch (currentError)

		{
		case 0:
			return "No error";
		case ERROR_NON_NUMERIC:
			return "Non numeric or operator in expression";
		case ERROR_MISSING_CLOSING_BRACKET:
			return "Missing ) bracket";
		case ERROR_EXTRA_CLOSING_BRACKET:
			return "Extra ) bracket";
		case ERROR_NUM_VARS_NOT_EQUAL_NUM_ARGS:
			return "The number of variables does not equal the number of arguments given";
		case ERROR_VARIABLE_NOT_FOUND_IN_FUNCTION:
			return "Variable not found in function";
		}
	}

	void reverseString(std::string& s)

	{
		std::string newString = "";

		for (int i = s.length() - 1; i >= 0; i--)

		{
			newString += s[i];
		}

		s = newString;
	}

	template<class T> T evaluate(const std::string s)

	{
		bool shouldSplit = false;

		int operatorType = -1;

		std::string expr = s;

		bool found = false;

		int rounds = 0;
		int splitPosition = -1;

		while (!found && rounds < 5)

		{
			int removeBracket = 0;
			for (int i = 0, brackets = 0; i < expr.length(); i++)

			{
				if (expr[i] == '(') brackets++;
				else if (expr[i] == ')') brackets--;

				if (expr[i] == '-' && !brackets) found = true, splitPosition = i, operatorType = OPERATOR_MINUS;
				else if (expr[i] == '+' && !brackets && rounds == 1) found = true, splitPosition = i, operatorType = OPERATOR_PLUS;
				else if (expr[i] == '*' && !brackets && rounds == 2) found = true, splitPosition = i, operatorType = OPERATOR_MULTIPLY;
				else if (expr[i] == '/' && !brackets && rounds == 3) found = true, splitPosition = i, operatorType = OPERATOR_DIVIDE;
				else if (expr[i] == '^' && !brackets && rounds == 4) found = true, splitPosition = i, operatorType = OPERATOR_POWER;
			}
			rounds++;
			if (rounds > 4 && !found)

			{
				for (int j = 0, brackets = 0; j < expr.length(); j++)

				{
					if (expr[j] == '(')

					{
						if (brackets == 0)

						{
							expr.erase(expr.begin() + j);
							j--;
						}
						brackets++;
					}

					else if (expr[j] == ')')

					{
						brackets--;
						if (brackets == 0)

						{
							expr.erase(expr.begin() + j);
							rounds = 0;
							j--;
						}
					}
				}
			}
		}

		if (splitPosition != -1) shouldSplit = true;
		

		std::string leftString;
		std::string rightString;

		T left;
		T right;
		T onlyValue;

		if (shouldSplit)

		{
			leftString = expr.substr(0, splitPosition);
			rightString = expr.substr(splitPosition+1, expr.length() - 1);

			//std::cout << leftString << "   " << rightString << std::endl;

			left = evaluate<T>(leftString);
			right = evaluate<T>(rightString);
		}

		else

		{
			
			if (isNumeric(expr, 0))

			{
				std::stringstream os(expr);
				os >> onlyValue;
			}

			else

			{
				std::cout << "Error in parsing!" << std::endl;
				onlyValue = 0;
			}
			/*std::stringstream osL(leftString);
			std::stringstream osR(rightString);

			osL >> left;
			osR >> right;*/
		}

		T result = 10.0;

		//std::cout << leftString << "   " << rightString << std::endl;

		switch (operatorType)

		{
		case OPERATOR_PLUS:
			result = left + right;
			break;
		case OPERATOR_MINUS:
			result = left - right;
			break;
		case OPERATOR_DIVIDE:
			result = left / right;
			break;
		case OPERATOR_MULTIPLY:
			result = left * right;
			break;
		case OPERATOR_POWER:
			result = pow(left, right);
			break;
		default:
			result = onlyValue;
			break;
		}
		return result;
	}

	bool isNumeric(std::string s, int position)

	{
		int pos = position;
		bool hasDecimal = false;

		if (s.length() == 0) return false;

		for (int i = 0; i < s.length(); i++)

		{
			if (s[i] == '.')

			{
				if (hasDecimal) return false;
				else hasDecimal = true;
			}

			else if (!(s[i] == '0' || s[i] == '1' || s[i] == '2' || s[i] == '3' || s[i] == '4' || s[i] == '5' || s[i] == '6' || s[i] == '7' || s[i] == '8' || s[i] == '9'))

			{
				currentError = ERROR_NON_NUMERIC;
				return false;
			}
		}

		return true;
	}

	template<class T> T calculateExpression(std::string s)

	{
		
		for (int i = 0; i < s.length(); i++)

		{
			if (s[i] == ' ' || s[i] == '\r' || s[i] == '\n') s.erase(s.begin() + i), i--;
		}
		int bracket = 0;
		for (int i = 0; i < s.length(); i++)

		{
			if (s[i] == '(') bracket++;
			else if (s[i] == ')') bracket--;
		}

		if (bracket > 0) currentError = ERROR_MISSING_CLOSING_BRACKET;
		else if (bracket < 0) currentError = ERROR_EXTRA_CLOSING_BRACKET;

		T result;

		if (!currentError) result = evaluate<double>(s);

		if (currentError)
		{
			std::cout << getErrorMessage() << std::endl;
			return 0;
		}

		else

		{
			return result;
		}
	}

	void setFunction(const std::string s, std::vector<std::string> variables_)

	{
		function = s;
		variables = variables_;
	}

	template<class T> T callFunction(std::vector<T> x)

	{
		if (x.size() != variables.size())

		{
			currentError = ERROR_NUM_VARS_NOT_EQUAL_NUM_ARGS;
		}

		std::string expr = function;

		for (int i = 0; i < variables.size(); i++)

		{
			int replacedAll = 0;
			int numberReplaced = 0;
			while (!replacedAll)

			{
				size_t pos = expr.find(variables[i]);
				if (pos != std::string::npos)

				{
					expr.replace(pos, variables[i].length(), std::to_string(x[i]));
				}

				else if (numberReplaced == 0)

				{
					currentError = ERROR_VARIABLE_NOT_FOUND_IN_FUNCTION;
					break;
				}

				else

				{
					replacedAll = 1;
				}

				numberReplaced++;
			}
		}

		//std::string expr = function;
		//std::npos = expr.find()
		//expr.replace(expr.find("lambda"), 6, std::to_string(x));
		return calculateExpression<T>(expr);
	}

	void resetErrors()

	{
		currentError = 0;
	}

private:

	std::vector<std::string> variables;

	const std::string allowedChars = "0123456789.-+/*^";
	std::string function;
	int currentError = 0;
};