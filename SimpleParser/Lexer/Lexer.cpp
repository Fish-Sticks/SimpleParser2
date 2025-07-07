#include "Lexer.hpp"
#include <iostream>
#include <string>


void Lexer::Lexer::ParseInputStream(const std::string& input)
{
	if (input == "")
		return;

	std::size_t currentPosition = -1;
	do
	{
		currentPosition++;
		char currentChar = input[currentPosition];

		if (isspace(currentChar)) // Skip whitespace
			continue;

		if (isdigit(currentChar))
		{
			std::string numberString{};
			do
			{
				numberString.push_back(currentChar);
				if (currentPosition + 1 != input.size()) // We don't want to overread.
				{
					currentPosition++;
					currentChar = input[currentPosition];
				}
				else
					break; // We are at end of stream

			} while (isdigit(currentChar) || currentChar == '.'); // Reconstruct the number by itself
			tokenStream.emplace_back(std::stof(numberString)); // Bug: user can use multiple dots, idc.
		}

		switch (currentChar)
		{
		case '+':
			tokenStream.emplace_back(LexicalType::TOK_ADD);
			break;
		case '-':
			tokenStream.emplace_back(LexicalType::TOK_NEG);
			break;
		case '*':
			tokenStream.emplace_back(LexicalType::TOK_MUL);
			break;
		case '/':
			tokenStream.emplace_back(LexicalType::TOK_DIV);
			break;
		case '^':
			tokenStream.emplace_back(LexicalType::TOK_POW);
			break;
		case '%':
			tokenStream.emplace_back(LexicalType::TOK_MOD);
			break;
		case '(':
			tokenStream.emplace_back(LexicalType::TOK_LPAREN);
			break;
		case ')':
			tokenStream.emplace_back(LexicalType::TOK_RPAREN);
			break;

		case 'k':
		case 'K':
			tokenStream.emplace_back(LexicalType::TOK_THOUSAND);
			break;
		case 'm':
		case 'M':
			tokenStream.emplace_back(LexicalType::TOK_MILLION);
			break;
		case 'b':
		case 'B':
			tokenStream.emplace_back(LexicalType::TOK_BILLION);
			break;
		case 't':
		case 'T':
			tokenStream.emplace_back(LexicalType::TOK_TRILLION);
			break;
		}
	} while (input.size() - 1 != currentPosition);
	tokenStream.emplace_back(); // Push EOF at end
}

Lexer::LexicalToken& Lexer::Lexer::peek()
{
	if ((position + 1) < tokenStream.size())
		return tokenStream[position + 1];
	else
		throw std::exception("Token stream empty (peek)");

	return tokenStream[position];
}

void Lexer::Lexer::consume()
{
	int newPosition = position + 1;

	if (newPosition < tokenStream.size())
		position = newPosition;
	else
		throw std::exception("Token stream empty (consume)");
}