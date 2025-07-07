#pragma once
#include "BaseVisitor.hpp"
#include "AST.hpp"
#include <sstream>

namespace Parser::AST
{
	// This visitor is kinda trash and I should've just kept it as evaluate by using a func like node->eval
	// but it is a POC of the visitor pattern so I keep it.
	// The work around for not being able to return values is to simply use a stack to compute operations.
	class EvaluationVisitor : ASTBaseVisitor
	{
	private:
		std::vector<float> output{};
	public:
		void Visit(ASTNumberNode* numberNode)
		{
			output.push_back(numberNode->value);
		}

		void Visit(ASTUnaryOperatorPrefixNode* operationNode)
		{
			operationNode->child->Accept(this);

			if (operationNode->operation == Lexer::LexicalType::TOK_NEG)
			{
				float num = output.back();
				output.pop_back();
				output.push_back(-num);
			}
		}

		void Visit(ASTUnaryOperatorPostfixNode* operationNode)
		{
			operationNode->child->Accept(this);

			float num = output.back();
			output.pop_back();

			switch (operationNode->operation)
			{
			case Lexer::LexicalType::TOK_THOUSAND:
				num *= 1000.f;
				break;
			case Lexer::LexicalType::TOK_MILLION:
				num *= 1000000.f;
				break;
			case Lexer::LexicalType::TOK_BILLION:
				num *= 1000000000.f;
				break;
			case Lexer::LexicalType::TOK_TRILLION:
				num *= 1000000000000.f;
				break;
			}
			output.push_back(num);
		}

		void Visit(ASTBinaryOperatorNode* operationNode)
		{
			operationNode->left->Accept(this);
			operationNode->right->Accept(this);
			
			float secondValue = output.back();
			output.pop_back();

			float firstValue = output.back();
			output.pop_back();

			float computedValue = 0;
			switch (operationNode->operation)
			{
				case Lexer::LexicalType::TOK_ADD:
					computedValue = firstValue + secondValue;
					break;
				case Lexer::LexicalType::TOK_NEG:
					computedValue = firstValue - secondValue;
					break;
				case Lexer::LexicalType::TOK_MUL:
					computedValue = firstValue * secondValue;
					break;
				case Lexer::LexicalType::TOK_DIV:
					computedValue = firstValue / secondValue;
					break;
				case Lexer::LexicalType::TOK_POW:
					computedValue = std::powf(firstValue, secondValue);
					break;
				case Lexer::LexicalType::TOK_MOD:
					computedValue = std::fmodf(firstValue, secondValue);
					break;
			}
			output.push_back(computedValue);
		}

		float EvaluateEquation(std::shared_ptr<ASTBaseNode> node)
		{
			output.clear();
			node->Accept(this);
			return output.back();
		}
	};

	class DisplayVisitor : ASTBaseVisitor
	{
	private:
		std::stringstream outputStream{};

		char GetOperationChar(Lexer::LexicalType op)
		{
			switch (op)
			{
			case Lexer::LexicalType::TOK_ADD:
				return '+';
			case Lexer::LexicalType::TOK_NEG:
				return '-';
			case Lexer::LexicalType::TOK_MUL:
				return '*';
			case Lexer::LexicalType::TOK_DIV:
				return '/';
			case Lexer::LexicalType::TOK_POW:
				return '^';
			case Lexer::LexicalType::TOK_MOD:
				return '%';
			case Lexer::LexicalType::TOK_THOUSAND:
				return 'K';
			case Lexer::LexicalType::TOK_MILLION:
				return 'M';
			case Lexer::LexicalType::TOK_BILLION:
				return 'B';
			case Lexer::LexicalType::TOK_TRILLION:
				return 'T';
			default:
				return '?';
			}
		}
	public:
		void Visit(ASTNumberNode* numberNode)
		{
			outputStream << std::fixed << std::setprecision(2) << numberNode->value;
		}

		void Visit(ASTUnaryOperatorPrefixNode* operationNode)
		{
			outputStream << "(";
			outputStream << GetOperationChar(operationNode->operation);
			operationNode->child->Accept(this);
			outputStream << ")";
		}

		void Visit(ASTUnaryOperatorPostfixNode* operationNode)
		{
			outputStream << "(";
			operationNode->child->Accept(this);
			outputStream << GetOperationChar(operationNode->operation);
			outputStream << ")";
		}

		void Visit(ASTBinaryOperatorNode* operationNode)
		{
			outputStream << "(";
			operationNode->left->Accept(this);
			outputStream << GetOperationChar(operationNode->operation);
			operationNode->right->Accept(this);
			outputStream << ")";
		}

		std::string GetDisplayEquation(std::shared_ptr<ASTBaseNode> node)
		{
			outputStream.str("");
			outputStream.clear();

			node->Accept(this);
			return outputStream.str();
		}
	};
}