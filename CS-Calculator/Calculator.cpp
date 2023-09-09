#include "Calculator.hpp"
#include "Parser.hpp"
#include <iostream>

namespace calc {

Calculator::Calculator()
	  : _opts(CalculatorOpts::eNil),
    	_tokens({})
{}

Calculator::Calculator(unsigned opts)
	  : _opts(opts),
    	_tokens({})
{}

_CALC_NODISCARD bool Calculator::evaluate(const std::string& expr, double& result)
{
	try {
		_tokens = Parser::try_parse(expr);
	} catch (const std::exception& e) {
		if (_opts & CalculatorOpts::eDebug)
			std::cerr << e.what() << std::endl;

		return false;
	}

	if (_opts & CalculatorOpts::ePrintRPN)
	{
		for (const auto &t : _tokens)
		{
			t.numeric
				? std::cout << Parser::to_char(t.op) << ' '
				: std::cout << t.val << ' ';
		}

		std::cout << '\n';
	}

	std::vector<double> stack;

	for (const auto &token : _tokens)
	{
		if (token.numeric)
		{
			stack.push_back(token.val);
		}
		else
		{
			// the two top numbers on our stack
			double b = stack.back();
			stack.pop_back();
			double a = stack.back();
			stack.pop_back();

			switch (token.op)
			{
				case Op::eExp:
					stack.push_back(pow(a, b));
					break;
				case Op::eMod:
					stack.push_back(fmod(a, b));
					break;
				case Op::eMul:
					stack.push_back(a * b);
					break;
				case Op::eDiv:
					stack.push_back(a / b);
					break;
				case Op::eAdd:
					stack.push_back(a + b);
					break;
				case Op::eSub:
					stack.push_back(a - b);
					break;
			}
		}
	}

	if (stack.empty())
		return false;

	result = stack.front();

	return true;
}

} // calc
