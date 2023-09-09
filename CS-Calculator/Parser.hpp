#pragma once
#include <string>
#include <vector>
#include "Tokens.hpp"

namespace calc {

class Parser
{
public:
	Parser() = delete;

	/* Make a token list from using the shunting yard algorithm */
	/* https://en.wikipedia.org/wiki/Shunting_yard_algorithm */
	static std::vector<Token> try_parse(const std::string &expr);

	static op_t to_op(char c);
	static char to_char(op_t t);

private:
	/* Logic for order of operations PEMDAS
	 * | ^ | 3 |
	 * | % | 2 |
	 * | * | 2 |
	 * | / | 2 |
	 * | + | 1 |
	 * | - | 1 |
	 */
	static int get_precedence(op_t token);

	/* ensure proper input syntax */
	static void throw_if_invalid(const std::string &expr);
};

} // calc
