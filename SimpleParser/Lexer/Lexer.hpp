#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace Lexer
{
	enum struct LexicalType : std::uint8_t
	{
		TOK_EOF,
		TOK_NUM, // 0-9
		TOK_ADD, // +
		TOK_NEG, // -
		TOK_MUL, // *
		TOK_DIV, // /
		TOK_POW, // ^
		TOK_MOD, // %
		TOK_RPAREN, // (
		TOK_LPAREN, // )

		TOK_THOUSAND, // k/K
		TOK_MILLION, // m/M
		TOK_BILLION, // b/B
		TOK_TRILLION // t/T
	};

	class LexicalToken
	{
	public:
		LexicalType tokenType = LexicalType::TOK_EOF;
		float value = 0;

		LexicalToken() = default;
		LexicalToken(LexicalType t) : tokenType{ t } {}
		LexicalToken(double num) : tokenType{ LexicalType::TOK_NUM }, value{ (float)num } {}
		LexicalToken(int num) : tokenType{ LexicalType::TOK_NUM }, value{ (float)num } {}
		LexicalToken(float num) : tokenType{ LexicalType::TOK_NUM }, value{ num } {}

		bool operator==(LexicalType t)
		{
			return tokenType == t;
		}

		bool operator!=(LexicalType t)
		{
			return tokenType != t;
		}

		std::string name()
		{
			switch (tokenType)
			{
			case LexicalType::TOK_EOF:
				return "EOF";
			case LexicalType::TOK_NUM:
				return std::string("NUMBER: ") + std::to_string((int)value);
			case LexicalType::TOK_ADD:
				return "ADD";
			case LexicalType::TOK_NEG:
				return "NEG";
			case LexicalType::TOK_MUL:
				return "MUL";
			case LexicalType::TOK_DIV:
				return "DIV";
			case LexicalType::TOK_POW:
				return "POW";
			case LexicalType::TOK_MOD:
				return "MOD";
			case LexicalType::TOK_RPAREN:
				return "RPAREN";
			case LexicalType::TOK_LPAREN:
				return "LPAREN";
			default:
				return "UNK";
			}
		}
	};

	class Lexer
	{
	private:
		void ParseInputStream(const std::string& input);
	public:
		std::vector<LexicalToken> tokenStream{};
		std::size_t position = -1;

		Lexer() = default;
		Lexer(const std::string& input)
		{
			ParseInputStream(input);
		}

		LexicalToken& peek();
		void consume();
	};
}