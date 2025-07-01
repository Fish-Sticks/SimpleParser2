#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include "../../Lexer/Lexer.hpp"

namespace Parser::AST
{
	struct ASTBaseNode
	{
	public:
		// Calculates value and returns it, each node evaluates with a different operation so this is implemented differently across different nodes.
		virtual float evaluate()
		{
			throw std::exception("Attempt to evaluate base AST node");
			return 0;
		};

		// Pretty prints and returns a formatted string of the node.
		virtual std::string print()
		{
			throw std::exception("Attempt to print base AST node");
			return "";
		}
	};

	struct ASTNumberNode : ASTBaseNode
	{
	private:
		float value = 0;
	public:
		ASTNumberNode() = default;
		ASTNumberNode(float v) : value{ v } {}

		float evaluate()
		{
			return value;
		}

		std::string print()
		{
			return std::to_string((int)value);
		}
	};

	struct ASTUnaryOperatorNode : ASTBaseNode
	{
	private:
		Lexer::LexicalType operation{};
		std::shared_ptr<ASTBaseNode> child{};
	public:
		ASTUnaryOperatorNode() = default;
		ASTUnaryOperatorNode(Lexer::LexicalType operation, std::shared_ptr<ASTBaseNode> child) : operation{ operation }, child { child } {}

		float evaluate()
		{
			switch (operation)
			{
			case Lexer::LexicalType::TOK_NEG:
				return -child->evaluate();
			case Lexer::LexicalType::TOK_THOUSAND:
				return child->evaluate() * 1000;
			case Lexer::LexicalType::TOK_MILLION:
				return child->evaluate() * 1000000;
			case Lexer::LexicalType::TOK_BILLION:
				return child->evaluate() * 1000000000;
			case Lexer::LexicalType::TOK_TRILLION:
				return child->evaluate() * 1000000000000;
			default:
				throw std::exception("Unknown unary operator evaluation attempt");
			}
		}

		std::string print()
		{
			switch (operation)
			{
			case Lexer::LexicalType::TOK_NEG:
				return "(-" + child->print() + ")";
			case Lexer::LexicalType::TOK_THOUSAND:
				return "(" + child->print() + "*1K)";
			case Lexer::LexicalType::TOK_MILLION:
				return "(" + child->print() + "*1M)";
			case Lexer::LexicalType::TOK_BILLION:
				return "(" + child->print() + "*1B)";
			case Lexer::LexicalType::TOK_TRILLION:
				return "(" + child->print() + "*1T)";
			default:
				throw std::exception("Unknown binary operator evaluation attempt");
			}
		}
	};

	struct ASTBinaryOperatorNode : ASTBaseNode
	{
	private:
		std::shared_ptr<ASTBaseNode> left{};
		std::shared_ptr<ASTBaseNode> right{};
		Lexer::LexicalType operation{};
	public:
		ASTBinaryOperatorNode() = default;
		ASTBinaryOperatorNode(Lexer::LexicalType operation, std::shared_ptr<ASTBaseNode> left, std::shared_ptr<ASTBaseNode> right) :
			operation{ operation }, left{ left }, right{ right } {
		}

		float evaluate()
		{
			switch (operation)
			{
			case Lexer::LexicalType::TOK_ADD:
				return left->evaluate() + right->evaluate();
			case Lexer::LexicalType::TOK_NEG:
				return left->evaluate() - right->evaluate();
			case Lexer::LexicalType::TOK_MUL:
				return left->evaluate() * right->evaluate();
			case Lexer::LexicalType::TOK_DIV:
				return left->evaluate() / right->evaluate();
			case Lexer::LexicalType::TOK_POW:
				return std::powf(left->evaluate(), right->evaluate());
			case Lexer::LexicalType::TOK_MOD:
				return std::fmodf(left->evaluate(), right->evaluate());
			}
		}

		std::string print()
		{
			switch (operation)
			{
			case Lexer::LexicalType::TOK_ADD:
				return "(" + left->print() + "+" + right->print() + ")";
			case Lexer::LexicalType::TOK_NEG:
				return "(" + left->print() + "-" + right->print() + ")";
			case Lexer::LexicalType::TOK_MUL:
				return "(" + left->print() + "*" + right->print() + ")";
			case Lexer::LexicalType::TOK_DIV:
				return "(" + left->print() + "/" + right->print() + ")";
			case Lexer::LexicalType::TOK_POW:
				return "(" + left->print() + "^" + right->print() + ")";
			case Lexer::LexicalType::TOK_MOD:
				return "(" + left->print() + "%" + right->print() + ")";
			}
		}
	};
}