#include <iostream>
#include "Calculator.hpp"
#include <cassert>

using namespace calc;

void test();

int main()
{
#ifndef NDEBUG
	test();
#endif

	std::cout << "Enter Expression for Calculator\n";

	std::string expr;
	std::getline(std::cin, expr);
	
	Calculator calculator(CalculatorOpts::eNil);

	int64_t result;
	while (!(calculator.evaluate(expr, result)))
	{
		std::cout << "Invalid User Input. Try Again" << std::endl;
		std::getline(std::cin, expr);
	}

	std::cout << result << std::endl;
}

void test()
{
	Calculator c(CalculatorOpts::eNil);
	int64_t r;
	c.evaluate("2 ^ 8", r);
	assert(r == 256);

	c.evaluate("10 % 3", r);
	assert(r == 1);

	c.evaluate("3 * 3", r);
	assert(r == 9);

	c.evaluate("9 / 3", r);
	assert(r == 3);

	c.evaluate("9 + 3", r);
	assert(r == 12);

	c.evaluate("9 - 3", r);
	assert(r == 6);

	c.evaluate("2+ { 10^2} - (24 / 6)", r);
	assert(r == 98);
}