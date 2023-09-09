#pragma once
#include <cassert>

namespace calc {

using token_t = int64_t;

/* Don't use enum class here because the typing is too strong i.e. can't automatically convert to int64_t
 * A limitation of this design is our range goes from +/- n^63 to +/- n^62 because of our reserved bit (62) */
struct Token
{
	static constexpr int64_t MOD_BIT    = 1LL << 62;

	static constexpr token_t LBracket	= 0x1 | MOD_BIT;	// [
	static constexpr token_t RBracket	= 0x2 | MOD_BIT;	// ]
	static constexpr token_t LParen		= 0x3 | MOD_BIT;	// (
	static constexpr token_t RParen 	= 0x4 | MOD_BIT;	// )
	static constexpr token_t LBrace 	= 0x5 | MOD_BIT;	// {
	static constexpr token_t RBrace 	= 0x6 | MOD_BIT;	// }
	static constexpr token_t Exp 		= 0x7 | MOD_BIT;	// ^
	static constexpr token_t Mod 		= 0x8 | MOD_BIT;	// %
	static constexpr token_t Mul 		= 0x9 | MOD_BIT;	// *
	static constexpr token_t Div 		= 0xA | MOD_BIT;	// /
	static constexpr token_t Add 		= 0xB | MOD_BIT;	// +
	static constexpr token_t Sub 		= 0xC | MOD_BIT;	// -

	static token_t get_pair(token_t t)
	{
		assert(t < Exp);
		return t % 2 ? t + 1 : t - 1;
	}
};

} // calc