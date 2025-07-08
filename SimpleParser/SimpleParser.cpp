#include <string>
#include <iostream>
#include <memory>
#include <chrono>

#include "Lexer/Lexer.hpp"
#include "Parser/TailRecursiveParser.hpp"
#include "Parser/PrattParser.hpp"
#include "Parser/AST/VisitorImpls.hpp"

int main()
{
	std::printf("SimpleParser is a parser which implements different LL parsers, using methods such as tail recursive parsing, and pratt parsing.\n");
	std::printf("LR parsing is planned, but im currently working on a way to create it without automated parser generators, which really teach nothing.\n");
	std::printf("Expressions are parsed into AST which can be evaluated by the visitor pattern.\n");

	// (5 + (3 + 3 * 10 / 2 * (9)) % 2) * (10 * 1 / 2 + (3 - 2) ^ 3)
	// -(3 * -10 + 2 ^ (-3 * -3)) + (-3-3-3-3-3-3*10+(-69))
	
	std::chrono::high_resolution_clock timer{};
	std::shared_ptr<Parser::AST::DisplayVisitor> displayVisitor = std::make_shared<Parser::AST::DisplayVisitor>();
	std::shared_ptr<Parser::AST::SmartDisplayVisitor> smartDisplayVisitor = std::make_shared<Parser::AST::SmartDisplayVisitor>();
	std::shared_ptr<Parser::AST::EvaluationVisitor> evaluateVisitor = std::make_shared<Parser::AST::EvaluationVisitor>();
	std::shared_ptr<Parser::AST::ExplodeVisitor> explodeVisitor = std::make_shared<Parser::AST::ExplodeVisitor>();

	// Test tail recursion
	{
		std::shared_ptr<Lexer::Lexer> myLexer = std::make_shared<Lexer::Lexer>("(5 + (3 + 3 * 10 / 2 * (9)) % 2) * (10 * 1 / 2 + (3 - 2) ^ 3) + 5 + -5");
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
				std::printf("Calculated result: %.2f\n", evaluateVisitor->EvaluateEquation(result));
				std::printf("Parenthesized: %s\n", displayVisitor->GetDisplayEquation(result).c_str());
			}

			if (evaluateVisitor->EvaluateEquation(result) != 30)
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
		std::shared_ptr<Lexer::Lexer> myLexer = std::make_shared<Lexer::Lexer>("(1k + 49k) / 100k");
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
				std::printf("Calculated result: %.2f\n", evaluateVisitor->EvaluateEquation(result));
				std::printf("Parenthesized: %s\n", displayVisitor->GetDisplayEquation(result).c_str());
			}
		}
		catch (std::exception& e)
		{
			std::printf("%s\n", e.what());
		}
	}

	std::printf("\n\nEnter any expressions below to test:\n");
	while (true)
	{
		std::string input{};
		std::getline(std::cin, input);

		try
		{
			std::shared_ptr<Lexer::Lexer> myLexer = std::make_shared<Lexer::Lexer>(input);
			std::shared_ptr<Parser::PrattParser> myParser = std::make_shared<Parser::PrattParser>(myLexer);

			const auto& before = timer.now();
			std::shared_ptr<Parser::AST::ASTBaseNode> result = myParser->ParseData();
			const auto& after = timer.now();

			std::uint64_t nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(after - before).count();
			std::printf("Compiling pratt took %d nanoseconds.\n", nanoSeconds);

			if (result == nullptr)
				std::printf("Parsing failed!\n");
			else
			{
				std::printf("Calculated result: %.5f\n", evaluateVisitor->EvaluateEquation(result));
				std::printf("Parenthesized: %s\n", displayVisitor->GetDisplayEquation(result).c_str());
				std::printf("Smart display: %s\n", smartDisplayVisitor->GetSmartDisplayEquation(result).c_str());

				std::shared_ptr<Parser::AST::ASTBaseNode> exploded = explodeVisitor->ExplodeExpression(result, 1);
				std::printf("Exploded: %s\n", displayVisitor->GetDisplayEquation(exploded).c_str());
				std::printf("Evaluated explosion: %.5f\n", evaluateVisitor->EvaluateEquation(exploded));
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
	Add postfix number notations to tail recursive parser (Not happening, idc enough to)
	Divide by zero error (idc).
*/