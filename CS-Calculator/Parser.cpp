#include "Parser.hpp"
#include <stdexcept>

namespace calc {

std::vector<Token> Parser::try_parse(const std::string& expr)
{
	try {
		throw_if_invalid(expr);
	} catch (const std::exception& e) {
		throw e; // explicitly rethrow
	}

	std::vector<Token> res_stack;	// result stack
	std::vector<op_t> op_stack;		// operation stack
	bool prev_numeric   = false;	// if last val was a number
	bool prev_op		= false;	// if last val was an arithmetic operation, used for negative determination
	bool negative		= false;	// tracks negative vs minus sign
	int floating_point	= 0;		// tracks floating points placement
	int last_precedence = 0;		// order of operations

	for (const char c : expr)
	{
		if (c == ' ')
			continue;

		if (isdigit(c))
		{
			if (prev_numeric)
			{
				assert(res_stack.back().numeric);
				if (!floating_point)
				{
					res_stack.back().val = res_stack.back().val * 10 + static_cast<double>(c - '0');
				}
				else
				{
					// fix bug, adding to a negative doesn't produce larger answer
					res_stack.back().val = res_stack.back().val >= 0
						? res_stack.back().val + static_cast<double>(c - '0') / pow(10, floating_point)
						: res_stack.back().val - static_cast<double>(c - '0') / pow(10, floating_point);

					floating_point++;
				}
			}
			else
			{
				res_stack.emplace_back(static_cast<double>(c - '0'));
			}

			if (negative && res_stack.back().val != 0)
			{
				res_stack.back().val *= -1;
				negative = false;
			}

			prev_numeric = true;
			prev_op = false;

			continue;
		}
		else if (c == '.')
		{
			if (floating_point)
				throw std::runtime_error("Two decimal points in one number");

			if (!prev_numeric)
				res_stack.emplace_back(0.0);

			floating_point++;
			prev_numeric = true;
			prev_op = false;
			continue;
		}
		else if (c == '-' && (prev_op || res_stack.empty()))
		{
			negative = true;
			prev_numeric = prev_op = false;
			continue;
		}

		if (floating_point == 1) /* must be digit if fp == 1 */
			throw std::runtime_error("Unterminated decimal");

		floating_point = 0;
		prev_numeric = prev_op = negative = false;

		switch (op_t op = Op::to_op(c))
		{
			op_t pair;
		case Op::eLBracket:
		case Op::eLParen:
		case Op::eLBrace:
			op_stack.push_back(op);
			prev_op = true;
			break;
		case Op::eRBracket:
		case Op::eRParen:
		case Op::eRBrace:
			pair = Op::get_pair(op);
			while (!op_stack.empty() && op_stack.back() != pair)
			{
				res_stack.emplace_back(op_stack.back());
				op_stack.pop_back();
			}
			op_stack.pop_back();
			break;
		case Op::eExp:
		case Op::eMod:
		case Op::eMul:
		case Op::eDiv:
		case Op::eAdd:
		case Op::eSub:
			if (!op_stack.empty() && get_precedence(op) <= last_precedence)
			{
				res_stack.emplace_back(op_stack.back());
				op_stack.pop_back();
			}
			op_stack.push_back(op);
			prev_op = true;
			break;
		default:
			throw std::runtime_error("Invalid Character as input: " + c);
		}

		last_precedence = !op_stack.empty() ? get_precedence(op_stack.back()) : 0;
	}

	for (auto op = op_stack.rbegin(); op != op_stack.rend(); ++op)
	{
		res_stack.emplace_back(*op);
	}

	int stack = 0;
	for (const auto &token : res_stack)
	{
		token.numeric ? stack++ : stack--;
	}

	if (stack != 1)
		throw std::runtime_error("Invalid combination of numbers and operations");

	return res_stack;
}

int Parser::get_precedence(op_t token)
{
	switch (token)
	{
	case Op::eExp:
		return 3;
	case Op::eMod:
	case Op::eMul:
	case Op::eDiv:
		return 2;
	case Op::eAdd:
	case Op::eSub:
		return 1;
	default:
		return 0;
	}
}

void Parser::throw_if_invalid(const std::string& expr)
{
	if (expr.empty())
		throw std::runtime_error("Empty string");

	int bracket = 0, paren = 0, brace = 0;
	for (const char c : expr)
	{
		if (c != ' ' && c != '.' && !isdigit(c))
		{
			switch(Op::to_op(c) /* may throw */)
			{
				case Op::eLBracket:
					bracket++;
					break;
				case Op::eLParen:
					paren++;
					break;
				case Op::eLBrace:
					brace++;
					break;
				case Op::eRBracket:
					bracket--;
					break;
				case Op::eRParen:
					paren--;
					break;
				case Op::eRBrace:
					brace--;
					break;
				default:
					break;
			}
		}
	}

	if ((bracket | paren | brace) != 0)
		throw std::runtime_error("Invalid Bracket Structure");
}

} // calc
