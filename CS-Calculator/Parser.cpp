#include "Parser.hpp"
#include <stdexcept>

namespace calc {

std::vector<token_t> Parser::try_parse(const std::string& expr)
{
	try {
		throw_if_invalid(expr);
	} catch (const std::exception& e) {
		throw e; // explicitly rethrow
	}

	std::vector<token_t> res_stack;	// result stack
	std::vector<token_t> op_stack;	// operation stack
	bool prev_numeric = false;		// if last val was a number
	int last_precedence = 0;		// order of operations

	for (const char c : expr)
	{
		if (c == ' ')
			continue;

		if (isdigit(c))
		{
			if (prev_numeric)
				res_stack.back() = res_stack.back() * 10 + static_cast<int64_t>(c - '0');
			else
				res_stack.push_back(static_cast<int64_t>(c - '0'));
			prev_numeric = true;
			continue;
		}
		else
		{
			prev_numeric = false;
		}

		token_t pair;

		switch (token_t token = to_token(c))
		{
		case Token::LBracket:
		case Token::LParen:
		case Token::LBrace:
			op_stack.push_back(token);
			break;
		case Token::RBracket:
		case Token::RParen:
		case Token::RBrace:
			pair = Token::get_pair(token);
			while (!op_stack.empty() && op_stack.back() != pair)
			{
				res_stack.push_back(op_stack.back());
				op_stack.pop_back();
			}
			op_stack.pop_back();
			break;
		case Token::Exp:
			// ^ is evaluated right-to-left so no precedence checking
			op_stack.push_back(token);
			break;
		case Token::Mod:
		case Token::Mul:
		case Token::Div:
		case Token::Add:
		case Token::Sub:
			if (!op_stack.empty() && get_precedence(token) <= last_precedence)
			{
				res_stack.push_back(op_stack.back());
				op_stack.pop_back();
			}
			op_stack.push_back(token);
			break;
		default:
			throw std::runtime_error("Invalid Character as input: " + c);
		}

		last_precedence = !op_stack.empty() ? get_precedence(op_stack.back()) : 0;
	}

	for (auto op = op_stack.rbegin(); op != op_stack.rend(); ++op)
	{
		res_stack.push_back(*op);
	}

	// if Debug build run this code, this should never happen and is only for debugging
#ifndef NDEBUG
	for (const auto t : res_stack)
	{
		switch (t)
		{
		case Token::LBracket:
		case Token::LParen:
		case Token::LBrace:
		case Token::RBracket:
		case Token::RParen:
		case Token::RBrace:
			throw std::runtime_error("Post-parse check failed");
		default:
			break;
		}
	}
#endif

	return res_stack;
}

token_t Parser::to_token(char c)
{
	switch (c)
	{
	case '[':
		return Token::LBracket;
	case '(':
		return Token::LParen;
	case '{':
		return Token::LBrace;
	case ']':
		return Token::RBracket;
	case ')':
		return Token::RParen;
	case '}':
		return Token::RBrace;
	case '^':
		return Token::Exp;
	case '%':
		return Token::Mod;
	case '*':
		return Token::Mul;
	case '/':
		return Token::Div;
	case '+':
		return Token::Add;
	case '-':
		return Token::Sub;
	default:
		throw std::runtime_error("Invalid Character as input: " + c);
	}
}

char Parser::to_char(token_t t)
{
	switch (t)
	{
	case Token::LBracket:
		return '[';
	case Token::LParen:
		return '(';
	case Token::LBrace:
		return '{';
	case Token::RBracket:
		return ']';
	case Token::RParen:
		return ')';
	case Token::RBrace:
		return '}';
	case Token::Exp:
		return '^';
	case Token::Mod:
		return '%';
	case Token::Mul:
		return '*';
	case Token::Div:
		return '/';
	case Token::Add:
		return '+';
	case Token::Sub:
		return '-';
	default:
		throw std::runtime_error("Invalid Token");
	}
}

int Parser::get_precedence(token_t token)
{
	switch (token)
	{
	case Token::Exp:
		return 3;
	case Token::Mod:
	case Token::Mul:
	case Token::Div:
		return 2;
	case Token::Add:
	case Token::Sub:
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
		if (c != ' ' && !isdigit(c))
		{
			token_t token;

			try {
				token = to_token(c);
			} catch (const std::exception& e) {
				throw e; // explicitly rethrow
			}

			switch(token)
			{
				case Token::LBracket:
					bracket++;
					break;
				case Token::LParen:
					paren++;
					break;
				case Token::LBrace:
					brace++;
					break;
				case Token::RBracket:
					bracket--;
					break;
				case Token::RParen:
					paren--;
					break;
				case Token::RBrace:
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
