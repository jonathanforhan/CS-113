#pragma once
#include <cassert>
#include <stdexcept>

namespace calc {

using op_t = unsigned;

struct Token
{
	Token(double val)
		: numeric(true), val(val)
	{}

	Token(op_t op)
		: numeric(false), op(op)
	{}

	// if numeric we hold val else op
	bool numeric;
	union {
		double val;
		op_t op;
	};
};

/* Don't use enum class here because the typing is too strong, hard to make comparisons */
struct Op
{
	enum {
		eLBracket	= 0x1,	// [
		eRBracket	= 0x2,	// ]
		eLParen		= 0x3,	// (
		eRParen		= 0x4,	// )
		eLBrace		= 0x5,	// {
		eRBrace		= 0x6,	// }
		eExp		= 0x7,	// ^
		eMod		= 0x8,	// %
		eMul		= 0x9,	// *
		eDiv		= 0xA,	// /
		eAdd		= 0xB,	// +
		eSub		= 0xC,	// -
	};

	static op_t get_pair(op_t t)
	{
		assert(t <= eRBrace);
		return t % 2 ? t + 1 : t - 1;
	}

	static op_t to_op(char c)
	{
		switch (c)
		{
		case '[':
			return Op::eLBracket;
		case '(':
			return Op::eLParen;
		case '{':
			return Op::eLBrace;
		case ']':
			return Op::eRBracket;
		case ')':
			return Op::eRParen;
		case '}':
			return Op::eRBrace;
		case '^':
			return Op::eExp;
		case '%':
			return Op::eMod;
		case '*':
			return Op::eMul;
		case '/':
			return Op::eDiv;
		case '+':
			return Op::eAdd;
		case '-':
			return Op::eSub;
		default:
			throw std::runtime_error("Invalid Character as input: " + c);
		}
	}

	static char to_char(op_t t)
	{
		switch (t)
		{
		case Op::eLBracket:
			return '[';
		case Op::eLParen:
			return '(';
		case Op::eLBrace:
			return '{';
		case Op::eRBracket:
			return ']';
		case Op::eRParen:
			return ')';
		case Op::eRBrace:
			return '}';
		case Op::eExp:
			return '^';
		case Op::eMod:
			return '%';
		case Op::eMul:
			return '*';
		case Op::eDiv:
			return '/';
		case Op::eAdd:
			return '+';
		case Op::eSub:
			return '-';
		default:
			throw std::runtime_error("Invalid Token");
		}
	}
};

} // calc