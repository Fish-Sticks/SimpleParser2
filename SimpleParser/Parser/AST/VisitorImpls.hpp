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


	// Makes a parenthesized display of the equation for each operation, this one will parenthesize everything but numbers
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
		void Visit(ASTNumberNode* numberNode) // Fucking annoying to display
		{
			outputStream << std::fixed << std::setprecision(3) << numberNode->value;
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


	class SmartDisplayVisitor : ASTBaseVisitor
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
		void Visit(ASTNumberNode* numberNode) // Fucking annoying to display
		{
			outputStream << numberNode->value;
		}

		void Visit(ASTUnaryOperatorPrefixNode* operationNode)
		{
			outputStream << GetOperationChar(operationNode->operation);
			operationNode->child->Accept(this);
		}

		void Visit(ASTUnaryOperatorPostfixNode* operationNode)
		{
			operationNode->child->Accept(this);
			outputStream << GetOperationChar(operationNode->operation);
		}

		void Visit(ASTBinaryOperatorNode* operationNode)
		{
			operationNode->left->Accept(this);
			outputStream << GetOperationChar(operationNode->operation);
			operationNode->right->Accept(this);
		}

		std::string GetSmartDisplayEquation(std::shared_ptr<ASTBaseNode> node)
		{
			outputStream.str("");
			outputStream.clear();

			node->Accept(this);
			return outputStream.str();
		}
	};

	// Explodes the equation and complexes it, making it harder to solve.
	// Note: This returns a clone of the expression with the explosion applied, it doesn't directly explode the original expression.
	// Warning: Explosion might not work well with floating point values.
	class ExplodeVisitor : ASTBaseVisitor
	{
	private:
		std::shared_ptr<ASTBaseNode> rootNode{}; // Node that gets returned
		std::vector<std::shared_ptr<ASTBaseNode>> nodeStack{};
	public:
		void Visit(ASTNumberNode* numberNode) // Todo: Add modulo
		{
			int result = rand() % 2;
			if (result == 1) // Subtraction obfuscation
			{
				float newNumberA = (float)rand() * (float)(rand() % 100);
				float newNumberB = (float)newNumberA - numberNode->value;

				std::shared_ptr<ASTBaseNode> leftNode{}, rightNode{};

				// Add negative nodes if needed, technically not required but I don't like having negatives without it.
				if (newNumberA < 0.f)
					leftNode = std::make_shared<ASTUnaryOperatorPrefixNode>(Lexer::LexicalType::TOK_NEG, std::make_unique<ASTNumberNode>(newNumberA * -1.f));
				else
					leftNode = std::make_unique<ASTNumberNode>(newNumberA);

				if (newNumberB < 0.f)
					rightNode = std::make_shared<ASTUnaryOperatorPrefixNode>(Lexer::LexicalType::TOK_NEG, std::make_unique<ASTNumberNode>(newNumberB * -1.f));
				else
					rightNode = std::make_unique<ASTNumberNode>(newNumberB);

				std::shared_ptr<ASTBinaryOperatorNode> explodeNode = std::make_unique<ASTBinaryOperatorNode>(Lexer::LexicalType::TOK_NEG, leftNode, rightNode);
				nodeStack.push_back(explodeNode);
			}
			else // Multiply / divide obfuscation (rough, not perfect.)
			{
				float first = 0.f, second = 0.f, ratio = 0.f;
				do
				{
					first = (float)rand() * (float)(rand() % 100);
					second = (float)rand() * (float)(rand() % 100);
					ratio = numberNode->value / (first / second);
				} while (((std::isnan(first) || std::isinf(first)) || (std::isnan(second) || std::isinf(second)) || (std::isnan(ratio) || std::isinf(ratio))));

				std::shared_ptr<ASTBaseNode> firstNumberNode{}, secondNumberNode{}, ratioNumberNode{};
				if (first < 0.f)
					firstNumberNode = std::make_shared<ASTUnaryOperatorPrefixNode>(Lexer::LexicalType::TOK_NEG, std::make_shared<ASTNumberNode>(first * -1.f));
				else
					firstNumberNode = std::make_shared<ASTNumberNode>(first);

				if (second < 0.f)
					secondNumberNode = std::make_shared<ASTUnaryOperatorPrefixNode>(Lexer::LexicalType::TOK_NEG, std::make_shared<ASTNumberNode>(second * -1.f));
				else
					secondNumberNode = std::make_shared<ASTNumberNode>(second);

				if (ratio < 0.f)
					ratioNumberNode = std::make_shared<ASTUnaryOperatorPrefixNode>(Lexer::LexicalType::TOK_NEG, std::make_shared<ASTNumberNode>(ratio * -1.f));
				else
					ratioNumberNode = std::make_shared<ASTNumberNode>(ratio);

				// first / second * ratio
				std::shared_ptr<ASTBinaryOperatorNode> explodeNode = std::make_shared<ASTBinaryOperatorNode>(Lexer::LexicalType::TOK_MUL,
					std::make_shared<ASTBinaryOperatorNode>(Lexer::LexicalType::TOK_DIV, firstNumberNode, secondNumberNode), ratioNumberNode);
				
				// Sometimes it gets imprecise, this will adjust if needed
				float differenceNumber = (first / second * ratio) - numberNode->value; // nan and inf
				if (differenceNumber != 0.f)
				{
					std::shared_ptr<ASTBaseNode> differenceNumberNode{};
					if (differenceNumber < 0.f)
						differenceNumberNode = std::make_shared<ASTUnaryOperatorPrefixNode>(Lexer::LexicalType::TOK_NEG, std::make_shared<ASTNumberNode>(differenceNumber * -1.f));
					else
						differenceNumberNode = std::make_shared<ASTNumberNode>(differenceNumber);

					explodeNode = std::make_shared<ASTBinaryOperatorNode>(Lexer::LexicalType::TOK_NEG, explodeNode, differenceNumberNode);
				}

				nodeStack.push_back(explodeNode);
			}
		}

		void Visit(ASTUnaryOperatorPrefixNode* operationNode)
		{
			std::shared_ptr<ASTUnaryOperatorPrefixNode> currentNode = std::make_unique<ASTUnaryOperatorPrefixNode>();
			currentNode->operation = operationNode->operation;

			operationNode->child->Accept(this);
			currentNode->child = nodeStack.back();

			nodeStack.pop_back();
			nodeStack.push_back(currentNode);
		}

		void Visit(ASTUnaryOperatorPostfixNode* operationNode)
		{
			std::shared_ptr<ASTUnaryOperatorPostfixNode> currentNode = std::make_unique<ASTUnaryOperatorPostfixNode>();
			currentNode->operation = operationNode->operation;

			operationNode->child->Accept(this);
			currentNode->child = nodeStack.back();

			nodeStack.pop_back();
			nodeStack.push_back(currentNode);
		}

		void Visit(ASTBinaryOperatorNode* operationNode)
		{
			std::shared_ptr<ASTBinaryOperatorNode> currentNode = std::make_unique<ASTBinaryOperatorNode>();
			currentNode->operation = operationNode->operation;

			operationNode->left->Accept(this);
			operationNode->right->Accept(this);

			currentNode->right = nodeStack.back();
			nodeStack.pop_back();
			currentNode->left = nodeStack.back();
			nodeStack.pop_back();
			nodeStack.push_back(currentNode); // Push our new node onto stack
		}

		std::shared_ptr<ASTBaseNode> ExplodeExpression(std::shared_ptr<ASTBaseNode> node, int complexity)
		{
			std::srand(std::time(nullptr));
			nodeStack.clear();
			do
			{
				complexity--;
				node->Accept(this);
				node = nodeStack.back();
				nodeStack.pop_back();
			} while (complexity != 0);
			return node;
		}
	};
}