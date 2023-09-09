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
	static std::vector<token_t> try_parse(const std::string &expr);

	static token_t to_token(char c);
	static char to_char(token_t t);

private:
	/* Logic for order of operations PEMDAS
	 * | ^ | 3 |
	 * | % | 2 |
	 * | * | 2 |
	 * | / | 2 |
	 * | + | 1 |
	 * | - | 1 |
	 */
	static int get_precedence(token_t token);

	/* ensure proper input syntax */
	static void throw_if_invalid(const std::string &expr);
};

} // calc
