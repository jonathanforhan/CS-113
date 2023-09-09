#pragma once
#include <cassert>

namespace calc {

using op_t = unsigned;

/* Don't use enum class here because the typing is too strong i.e. can't automatically convert to int64_t
 * A limitation of this design is our range goes from +/- n^63 to +/- n^62 because of our reserved bit (62) */
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
};

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

} // calc