#pragma once
#include <vector>
#include <string>
#include "Tokens.hpp"

// C++14 noise
#if __cplusplus >= 201703L
#	define _CALC_NODISCARD [[nodiscard]]
#elif defined(_MSC_VER) && _MSC_VER >= 1700 
#	define _CALC_NODISCARD _Check_return_
#elif defined(__GNUC__) || defined(__clang__)
#	define _CALC_NODISCARD __attribute__((warn_unused_result))
#else
#	define _CALC_NODISCARD
#endif

namespace calc {

struct CalculatorOpts
{
	enum
	{
		eNil		= 0,
		ePrintRPN 	= 1,	// Print Reverse Polish Notation during parsing
		eDebug 	    = 2,	// Debug Printing
	};
};

class Calculator
{
public:
	explicit Calculator();
	explicit Calculator(unsigned opts);

	_CALC_NODISCARD bool evaluate(const std::string& expr, double& result);

private:
	unsigned _opts;
	std::vector<Token> _tokens;
};

} // calc
