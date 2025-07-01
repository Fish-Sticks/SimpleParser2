#include <string>
#include <iostream>
#include <memory>
#include <chrono>

#include "Lexer/Lexer.hpp"
#include "Parser/TailRecursiveParser.hpp"
#include "Parser/PrattParser.hpp"


int main()
{
	std::printf("SimpleParser is a parser which implements both LL and LR parsers, using methods such as tail recursive parsing, pratt parsing, and SLR parsing.\n");
	std::printf("Expressions are parsed into AST which can be evaluated.\n");

	// (5 + (3 + 3 * 10 / 2 * (9)) % 2) * (10 * 1 / 2 + (3 - 2) ^ 3)
	// -(3 * -10 + 2 ^ (-3 * -3)) + (-3-3-3-3-3-3*10+(-69))
	
	std::chrono::high_resolution_clock timer{};

	// Test tail recursion
	{
		std::shared_ptr<Lexer::Lexer> myLexer = std::make_shared<Lexer::Lexer>("(5 + (3 + 3 * 10 / 2 * (9)) % 2) * (10 * 1 / 2 + (3 - 2) ^ 3)");
		std::shared_ptr<Parser::TailRecursiveParser> myParser = std::make_shared<Parser::TailRecursiveParser>(myLexer);
		try
		{
			const auto& before = timer.now();
			std::shared_ptr<Parser::AST::ASTBaseNode> result = myParser->ParseData();
			const auto& after = timer.now();

			std::uint64_t nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(after - before).count();
			std::printf("Compiling tail recursive descent took %d nanoseconds.\n", nanoSeconds);

			if (result == nullptr)
				std::printf("Parsing failed!\n");
			else
			{
				std::printf("Calculated result: %.2f\n", result->evaluate());
				std::printf("Parenthesized: %s\n", result->print().c_str());
			}

			if (result->evaluate() != 30)
			{
				std::printf("TAIL RECURSION TEST FAILED!\n");
			}
		}
		catch (std::exception& e)
		{
			std::printf("%s\n", e.what());
		}
	}

	// Test pratt
	{
		// std::shared_ptr<Lexer::Lexer> myLexer = std::make_shared<Lexer::Lexer>("-1k");
		std::shared_ptr<Lexer::Lexer> myLexer = std::make_shared<Lexer::Lexer>("(5 + (3 + 3 * 10 / 2 * (9)) % 2) * (10 * 1 / 2 + (3 - 2) ^ 3)");
		std::shared_ptr<Parser::PrattParser> myParser = std::make_shared<Parser::PrattParser>(myLexer);

		try
		{
			const auto& before = timer.now();
			std::shared_ptr<Parser::AST::ASTBaseNode> result = myParser->ParseData();
			const auto& after = timer.now();

			std::uint64_t nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(after - before).count();
			std::printf("Compiling pratt took %d nanoseconds.\n", nanoSeconds);

			if (result == nullptr)
				std::printf("Parsing failed!\n");
			else
			{
				std::printf("Calculated result: %.2f\n", result->evaluate());
				std::printf("Parenthesized: %s\n", result->print().c_str());
			}
		}
		catch (std::exception& e)
		{
			std::printf("%s\n", e.what());
		}
	}
}

/*
	TODO:
	Add postfix number notations to tail recursive parser
	Add SLR parsing.
	Add recursive ascent parsing.
*/