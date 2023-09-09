#include "Calculator.hpp"
#include "Parser.hpp"
#include <iostream>

namespace calc {

Calculator::Calculator(CalculatorOpts::Bits opts)
	  : _opts(opts),
    	_tokens({})
{}

bool Calculator::evaluate(const std::string& expr, int64_t& result)
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
			t & Token::MOD_BIT
				? std::cout << Parser::to_char(t) << ' '
				: std::cout << t << ' ';
		}

		std::cout << '\n';
	}

	std::vector<int64_t> stack;

	for (const auto token : _tokens)
	{
		if (!(token & Token::MOD_BIT))
		{
			stack.push_back(token);
		}
		else
		{
			// the two top numbers on our stack
			int64_t b = stack.back();
			stack.pop_back();
			int64_t a = stack.back();
			stack.pop_back();

			switch (token)
			{
				case Token::Exp:
					stack.push_back(static_cast<int64_t>(pow(a, b)));
					break;
				case Token::Mod:
					stack.push_back(a % b);
					break;
				case Token::Mul:
					stack.push_back(a * b);
					break;
				case Token::Div:
					stack.push_back(a / b);
					break;
				case Token::Add:
					stack.push_back(a + b);
					break;
				case Token::Sub:
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
