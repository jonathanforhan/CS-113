#pragma once
#include <vector>
#include <string>
#include "Tokens.hpp"

namespace calc {

enum class CalculatorOpts
{
	eNil,
	ePrintRPN,	// Print Reverse Polish Notation during parsing
};

class Calculator
{
public:
	explicit Calculator(CalculatorOpts opts);

	bool evaluate(const std::string& expr, int64_t& result);

private:
	CalculatorOpts _opts;
	std::vector<token_t> _tokens;
};

} // calc
