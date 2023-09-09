#pragma once
#include <vector>
#include <string>
#include "Tokens.hpp"

namespace calc {

struct CalculatorOpts
{
	enum Bits
	{
		eNil		= 0,
		ePrintRPN 	= 1,	// Print Reverse Polish Notation during parsing
		eDebug 	    = 2,	// Debug Printing
	};
};

class Calculator
{
public:
	explicit Calculator(CalculatorOpts::Bits opts);

	bool evaluate(const std::string& expr, int64_t& result);

private:
	CalculatorOpts::Bits _opts;
	std::vector<token_t> _tokens;
};

} // calc
