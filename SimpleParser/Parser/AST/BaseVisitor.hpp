#pragma once

namespace Parser::AST
{
	class ASTNumberNode;
	class ASTUnaryOperatorPrefixNode;
	class ASTUnaryOperatorPostfixNode;
	class ASTBinaryOperatorNode;

	class ASTBaseVisitor
	{
	public:
		virtual void Visit(ASTNumberNode* number) = 0;
		virtual void Visit(ASTUnaryOperatorPrefixNode* operation) = 0;
		virtual void Visit(ASTUnaryOperatorPostfixNode* operation) = 0;
		virtual void Visit(ASTBinaryOperatorNode* operation) = 0;
	};
}