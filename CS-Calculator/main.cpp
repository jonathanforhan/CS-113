#include <iostream>
#include <cassert>
#include "Calculator.hpp"

using namespace calc;

void test();

int main()
{
#ifndef NDEBUG
	test();
#endif
	const std::vector<std::string> examples = {
		"(1 / 10) ^ 2 + 5",
		"[(1+4) / 10] ^2 ^2",
		"100 % 48 + 5 / .5"
	};
	
	Calculator calculator;
	std::string expr;
	double result;
	unsigned i = 0;

	std::cout << "Enter an Expression for the Calculator\nType 'help' for help\nType 'exit' to quit\n" << std::endl;

	while (1)
	{
		std::cout << "> ";
		std::getline(std::cin, expr);

		if (expr == "exit")
		{
			break;
		}
		else if (expr == "help")
		{
			std::cout << "\nEnter an equation, for example: " + examples[i] + "\n" << std::endl;
			i = ++i % examples.size();
			continue;
		}

		if (!(calculator.evaluate(expr, result)))
		{
			std::cout << "Invalid User Input. Try Again\n" << std::endl;
			continue;
		}

		std::cout << "------------" << std::endl;
		std::cout << "  " << result << "\n" << std::endl;
	}

}

void test()
{
	Calculator c;
	double r;
	bool b;

	b = c.evaluate("2.34 / (-.234)", r);
	assert(b && r == -10);

	b = c.evaluate("2.5 * 2", r);
	assert(b && r == 5);

	b = c.evaluate("-.5 ^ 3 * 8", r);
	assert(b && r == -1);

	b = c.evaluate("2+ { 10^2} - (24 / 6)", r);
	assert(b && r == 98);

	b = c.evaluate("2^(5%3)/[1+4-3]*3", r);
	assert(b && r == 6);

	b = c.evaluate("2 * -2", r);
	assert(b && r == -4);

	b = c.evaluate("-2^(5%3)/[1+4-3]*-3", r);
	assert(b && r == -6);

	b = c.evaluate("5 / 2", r);
	assert(b && r == 2.5);

	b = c.evaluate("(1+3))", r);
	assert(!b);

	b = c.evaluate("-..34 + 1", r);
	assert(!b);

	b = c.evaluate("5 * 0.3-", r);
	assert(!b);

	b = c.evaluate(") 1 + 2 (", r);
	assert(!b);

	b = c.evaluate("1 / 0", r);
	assert(!b);
}