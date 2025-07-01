#include "TailRecursiveParser.hpp"

std::shared_ptr<Parser::AST::ASTBaseNode> Parser::TailRecursiveParser::ParseNumber()
{
	auto& currentToken = lexer->peek();
	if (currentToken != Lexer::LexicalType::TOK_NUM)
	{
		std::printf("EXPECTED NUMBER, GOT %s\n", currentToken.name().c_str());
		throw std::exception("Error parsing, expected NUMBER got garbage");
	}

	lexer->consume();
	return std::make_unique<Parser::AST::ASTNumberNode>(currentToken.value);
}

std::shared_ptr<Parser::AST::ASTBaseNode> Parser::TailRecursiveParser::ParseParenthesis()
{
	// Why it's an if statement: it resumes the whole parse loop calling itself again, so it is already in it's own higher level loop.
	// this operator is special because it *changes* precedence of whats inside it by making the parser parse it in a different order.

	if (lexer->peek() == Lexer::LexicalType::TOK_LPAREN)
	{
		lexer->consume();
		std::shared_ptr<Parser::AST::ASTBaseNode> expr = ParseData();

		if (lexer->peek() != Lexer::LexicalType::TOK_RPAREN)
			throw std::exception("Error parsing, expected closing parenthesis");
		lexer->consume();

		return expr;
	}

	return ParseNumber();
}

// Pay attention! This one is slightly different since it is prefix...
std::shared_ptr<Parser::AST::ASTBaseNode> Parser::TailRecursiveParser::ParseNegate()
{
	while (lexer->peek() == Lexer::LexicalType::TOK_NEG)
	{
		Lexer::LexicalType currentOperator = lexer->peek().tokenType;
		lexer->consume();

		return std::make_shared<Parser::AST::ASTUnaryOperatorNode>(currentOperator, ParseNegate());
	}

	return ParseParenthesis();
}

std::shared_ptr<Parser::AST::ASTBaseNode> Parser::TailRecursiveParser::ParsePow()
{
	std::shared_ptr<Parser::AST::ASTBaseNode> lhs = ParseNegate();

	while (lexer->peek() == Lexer::LexicalType::TOK_POW)
	{
		Lexer::LexicalType currentOperator = lexer->peek().tokenType;
		lexer->consume();

		std::shared_ptr<Parser::AST::ASTBaseNode> rhs = ParseNegate();
		lhs = std::make_unique<Parser::AST::ASTBinaryOperatorNode>(currentOperator, lhs, rhs); // Reparent node with operator included into it
	}

	return lhs;
}

std::shared_ptr<Parser::AST::ASTBaseNode> Parser::TailRecursiveParser::ParseMulDiv()
{
	std::shared_ptr<Parser::AST::ASTBaseNode> lhs = ParsePow();

	while (lexer->peek() == Lexer::LexicalType::TOK_MUL || lexer->peek() == Lexer::LexicalType::TOK_DIV)
	{
		Lexer::LexicalType currentOperator = lexer->peek().tokenType;
		lexer->consume();

		std::shared_ptr<Parser::AST::ASTBaseNode> rhs = ParsePow();
		lhs = std::make_unique<Parser::AST::ASTBinaryOperatorNode>(currentOperator, lhs, rhs);
	}

	return lhs;
}

std::shared_ptr<Parser::AST::ASTBaseNode> Parser::TailRecursiveParser::ParseMod()
{
	std::shared_ptr<Parser::AST::ASTBaseNode> lhs = ParseMulDiv();

	while (lexer->peek() == Lexer::LexicalType::TOK_MOD)
	{
		Lexer::LexicalType currentOperator = lexer->peek().tokenType;
		lexer->consume();

		std::shared_ptr<Parser::AST::ASTBaseNode> rhs = ParseMulDiv();
		lhs = std::make_unique<Parser::AST::ASTBinaryOperatorNode>(currentOperator, lhs, rhs); // Reparent node with operator included into it
	}

	return lhs;
}

std::shared_ptr<Parser::AST::ASTBaseNode> Parser::TailRecursiveParser::ParseAddSub()
{
	std::shared_ptr<Parser::AST::ASTBaseNode> lhs = ParseMod();

	while (lexer->peek() == Lexer::LexicalType::TOK_ADD || lexer->peek() == Lexer::LexicalType::TOK_NEG)
	{
		Lexer::LexicalType currentOperator = lexer->peek().tokenType;
		lexer->consume();

		std::shared_ptr<Parser::AST::ASTBaseNode> rhs = ParseMod();
		lhs = std::make_unique<Parser::AST::ASTBinaryOperatorNode>(currentOperator, lhs, rhs);
	}

	return lhs;
}

std::shared_ptr<Parser::AST::ASTBaseNode> Parser::TailRecursiveParser::ParseData()
{
	if (auto& currentToken = lexer->peek(); currentToken != Lexer::LexicalType::TOK_EOF)
	{
		return ParseAddSub();
	}

	std::printf("No data to read!\n");
	return nullptr;
}