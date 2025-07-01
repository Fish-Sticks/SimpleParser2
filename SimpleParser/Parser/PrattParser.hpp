#pragma once
#pragma once
#include "../Lexer/Lexer.hpp"
#include "AST/AST.hpp"

namespace Parser
{
	struct BindPower
	{
		std::uint32_t lhs = 0;
		std::uint32_t rhs = 0;

		BindPower(std::uint32_t lhs, std::uint32_t rhs) : lhs{ lhs }, rhs{ rhs } {}
	};


	/*
		Pratt parser is a method of tail recursive descent parser LL(1) which utilizes self recursion and loops. Unlike the standard tail recursive parser this parser only has one main recursion function.
		The rest of the work is handled by the precedence pairs which help define what should take place and when.
		The pratt parser is more concise and easier to change than the standard tail recursive parser.
		The pratt parsers downside is that its more complex internally, outside of typical changing precedence of tables.
		Since the pratt parser is just a more condensed version of the recursive descent parser, it is faster due to less function calls. (* do a speed test)

		Unlike standard tail recursive descent parsing, pratt parsing can switch associtivity to be right associated i.e [a + (b + c)] instead of [(a + b) + c] very easily.
		Naturally tail recursive descent parsing would require another function for handling this since it is going left to right without modifications, but with pratt parsing you can just switch the precedence numbers.
	*/
	class PrattParser
	{
	private:
		std::shared_ptr<Lexer::Lexer> lexer{};

		BindPower GetPrecedencePrefix(Lexer::LexicalType tokType);
		BindPower GetPrecedenceInfix(Lexer::LexicalType tokType);
		BindPower GetPrecedencePostfix(Lexer::LexicalType tokType);

		std::shared_ptr<Parser::AST::ASTBaseNode> ParseLoop(std::uint32_t minPower);
	public:
		PrattParser() = default;
		PrattParser(std::shared_ptr<Lexer::Lexer> lexer) : lexer{ lexer } {}
		PrattParser(const std::string& input)
		{
			lexer = std::make_shared<Lexer::Lexer>(input);
		}

		std::shared_ptr<AST::ASTBaseNode> ParseData();
	};
}