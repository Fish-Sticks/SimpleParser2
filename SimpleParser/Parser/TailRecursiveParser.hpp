#pragma once
#include "../Lexer/Lexer.hpp"
#include "AST/AST.hpp"

namespace Parser
{
	class GenericParser
	{

	};


	/*
		A method of parsing which involves recursive descent parsing.
		The "tail" part means it reparents its nodes so that it doesn't have infinite recursion issues.
		Instead of what parsers traditionally use which is parse tables, this keeps the logic inside recursive function calls, hence the name recursive.
		Descent meaning its going down a parse tree, as this is a top-down parser and parses left to right.
		Another name for this is a tail recursive descent LL(1) parser since we only look ahead 1 token, and don't do any backtracking.
	*/
	class TailRecursiveParser
	{
	private:
		std::shared_ptr<Lexer::Lexer> lexer{};
	
		std::shared_ptr<Parser::AST::ASTBaseNode> ParseNumber(); // Highest strength, number is the core
		std::shared_ptr<Parser::AST::ASTBaseNode> ParseParenthesis(); // Special case, it redefines how the parser goes about parsing data, precedence is defined by its scope.
		std::shared_ptr<Parser::AST::ASTBaseNode> ParseNegate(); // Second highest strength, negates anything its next to
		std::shared_ptr<Parser::AST::ASTBaseNode> ParsePow();
		std::shared_ptr<Parser::AST::ASTBaseNode> ParseMulDiv(); // Considered only after add / sub
		std::shared_ptr<Parser::AST::ASTBaseNode> ParseMod();
		std::shared_ptr<Parser::AST::ASTBaseNode> ParseAddSub(); // Last to be considered
	public:
		TailRecursiveParser() = default;
		TailRecursiveParser(std::shared_ptr<Lexer::Lexer> lexer) : lexer{ lexer } {}
		TailRecursiveParser(const std::string& input)
		{
			lexer = std::make_shared<Lexer::Lexer>(input);
		}

		std::shared_ptr<AST::ASTBaseNode> ParseData();
	};
}