#include "PrattParser.hpp"


Parser::BindPower Parser::PrattParser::GetPrecedencePrefix(Lexer::LexicalType tokType)
{
	switch (tokType)
	{
	case Lexer::LexicalType::TOK_NEG:
		return BindPower(9, 9);
	default:
		return BindPower(0, 0);
	}
}

Parser::BindPower Parser::PrattParser::GetPrecedenceInfix(Lexer::LexicalType tokType)
{
	switch (tokType)
	{
	case Lexer::LexicalType::TOK_ADD:
	case Lexer::LexicalType::TOK_NEG:
		return BindPower(1, 2);

	case Lexer::LexicalType::TOK_MOD:
		return BindPower(3, 4);

	case Lexer::LexicalType::TOK_MUL:
	case Lexer::LexicalType::TOK_DIV:
		return BindPower(5, 6);

	case Lexer::LexicalType::TOK_POW:
		return BindPower(8, 7);

	default:
		return BindPower(0, 0);
	}
}

Parser::BindPower Parser::PrattParser::GetPrecedencePostfix(Lexer::LexicalType tokType)
{
	switch (tokType)
	{
	case Lexer::LexicalType::TOK_THOUSAND:
	case Lexer::LexicalType::TOK_MILLION:
	case Lexer::LexicalType::TOK_BILLION:
	case Lexer::LexicalType::TOK_TRILLION:
		return BindPower(8, 8);
	default:
		return BindPower(0, 0);
	}
}

// The parser contains recursion and loops, storing the minimum binding power. If the power does not suffice it will recurse to a deeper level to parse it correctly.
std::shared_ptr<Parser::AST::ASTBaseNode> Parser::PrattParser::ParseLoop(std::uint32_t minPower)
{
	std::shared_ptr<Parser::AST::ASTBaseNode> lhs = 0;

	// Parse primaries / prefix
	Lexer::LexicalToken prefixToken = lexer->peek();
	if (prefixToken == Lexer::LexicalType::TOK_NUM)
	{
		lexer->consume();
		lhs = std::make_unique<AST::ASTNumberNode>(prefixToken.value);
	}
	else if (BindPower prePower = GetPrecedencePrefix(prefixToken.tokenType); prePower.rhs != 0)
	{
		lexer->consume();
		lhs = std::make_unique<AST::ASTUnaryOperatorPrefixNode>(prefixToken.tokenType, ParseLoop(prePower.rhs));
	}
	else if (prefixToken.tokenType == Lexer::LexicalType::TOK_LPAREN) // Parenthesis are special since they define new "context"
	{
		lexer->consume();
		lhs = ParseLoop(0);

		if (lexer->peek() != Lexer::LexicalType::TOK_RPAREN)
			throw std::exception("Expected closing parenthesis");

		lexer->consume();
	}

	// Parse postfix & infix
	while (true)
	{
		Lexer::LexicalToken operatorToken = lexer->peek();
		if (operatorToken == Lexer::LexicalType::TOK_EOF)
			break;

		if (BindPower postPower = GetPrecedencePostfix(operatorToken.tokenType); postPower.lhs != 0)
		{
			if (postPower.lhs < minPower)
				break;

			lexer->consume();
			lhs = std::make_unique<AST::ASTUnaryOperatorPostfixNode>(operatorToken.tokenType, lhs);
			continue;
		}

		if (BindPower infixPower = GetPrecedenceInfix(operatorToken.tokenType); infixPower.rhs != 0)
		{
			if (infixPower.lhs < minPower)
				break;

			lexer->consume();

			// This will continue on the loop of number, operator, etc
			std::shared_ptr<Parser::AST::ASTBaseNode> rhs = ParseLoop(infixPower.rhs);
			std::shared_ptr<Parser::AST::ASTBinaryOperatorNode> newOpNode = std::make_unique<Parser::AST::ASTBinaryOperatorNode>(operatorToken.tokenType, lhs, rhs);

			// Reparent to continue the recursion
			lhs = newOpNode;
			continue;
		}

		break;
	}


	// We can't have LHS be null since that would create a nonexistant branch. If we hit this then something is wrong with input.
	if (prefixToken.tokenType == Lexer::LexicalType::TOK_EOF && lhs == nullptr)
		throw std::exception("Unexpected end in token stream");

	return lhs;
}

std::shared_ptr<Parser::AST::ASTBaseNode> Parser::PrattParser::ParseData()
{
	if (auto& currentToken = lexer->peek(); currentToken != Lexer::LexicalType::TOK_EOF)
	{
		return ParseLoop(0);
	}

	std::printf("No data to read!\n");
	return nullptr;
}