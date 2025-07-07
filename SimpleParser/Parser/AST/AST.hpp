#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include "../../Lexer/Lexer.hpp"
#include "BaseVisitor.hpp"

namespace Parser::AST
{
	struct ASTBaseNode
	{
	public:
		virtual void Accept(ASTBaseVisitor* visitor) = 0;
	};

	struct ASTNumberNode : ASTBaseNode
	{
	public:
		float value = 0;

		ASTNumberNode() = default;
		ASTNumberNode(float v) : value{ v } {}

		void Accept(ASTBaseVisitor* visitor)
		{
			visitor->Visit(this);
		}
	};

	struct ASTUnaryOperatorPrefixNode : ASTBaseNode
	{
	public:
		Lexer::LexicalType operation{};
		std::shared_ptr<ASTBaseNode> child{};

		ASTUnaryOperatorPrefixNode() = default;
		ASTUnaryOperatorPrefixNode(Lexer::LexicalType operation, std::shared_ptr<ASTBaseNode> child) : operation{ operation }, child { child } {}

		void Accept(ASTBaseVisitor* visitor)
		{
			visitor->Visit(this);
		}
	};

	struct ASTUnaryOperatorPostfixNode : ASTBaseNode
	{
	public:
		Lexer::LexicalType operation{};
		std::shared_ptr<ASTBaseNode> child{};

		ASTUnaryOperatorPostfixNode() = default;
		ASTUnaryOperatorPostfixNode(Lexer::LexicalType operation, std::shared_ptr<ASTBaseNode> child) : operation{ operation }, child{ child } {}

		void Accept(ASTBaseVisitor* visitor)
		{
			visitor->Visit(this);
		}
	};

	struct ASTBinaryOperatorNode : ASTBaseNode
	{
	public:
		std::shared_ptr<ASTBaseNode> left{};
		std::shared_ptr<ASTBaseNode> right{};
		Lexer::LexicalType operation{};

		ASTBinaryOperatorNode() = default;
		ASTBinaryOperatorNode(Lexer::LexicalType operation, std::shared_ptr<ASTBaseNode> left, std::shared_ptr<ASTBaseNode> right) :
			operation{ operation }, left{ left }, right{ right } {
		}

		void Accept(ASTBaseVisitor* visitor)
		{
			visitor->Visit(this);
		}
	};
}