/**
*
* Solution to homework task
* Data Structures Course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2016/2017
*
* @author Lilyana Videva
* @idnumber 61887
* @task 0
* @compiler VC
*
*/

#pragma once
#include"Stack.h"
#include<iostream>

//class, responsible for the convertion from infix to postfix
class ConvertToPostfix
{
private:
	bool isUnion(char) const;
	bool isConcatenation(char) const;
	bool isKleeneStar(char) const;
	bool isBasicSymbol(char) const;
	bool correctBrakets(std::string) const;
	std::string convertSpecialSymbols(std::string);
public:
	std::string explicitConcatenation(std::string);
	bool hasHigherPriority(char, char);
	std::string regexToPostfix(std::string);
};

//checks if the symbol is the operation union of two expressions
bool ConvertToPostfix::isUnion(char symbol) const
{
	if (symbol == '|') return true;
	return false;
}

//checks if the symbol is the operation concatenation of two expressions
bool ConvertToPostfix::isConcatenation(char symbol) const
{
	if (symbol == '.') return true;
	return false;
}

//checks if the symbol is the operation kleene star of an expression
bool ConvertToPostfix::isKleeneStar(char symbol) const
{
	if (symbol == '*') return true;
	return false;
}

//checks if the symbol is a basic one
bool ConvertToPostfix::isBasicSymbol(char symbol) const
{
	//if it is one of the above operation, it is not
	if (isUnion(symbol) || isConcatenation(symbol) || isKleeneStar(symbol) || symbol == '(' || symbol == ')') return false;
	return true;
}

//checks if the number of brackets in the expression is correct
bool ConvertToPostfix::correctBrakets(std::string line) const
{
	size_t count = 0;
	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] == '(') count++;
		if (line[i] == ')') count--;
	}
	if (count != 0) return false;
	else return true;
}

//converts the special symbols in the expression to their equivalents
std::string ConvertToPostfix::convertSpecialSymbols(std::string line)
{
	std::string newLine;
	//checks every symbol in the expression
	for (size_t i = 0; i < line.size(); i++)
	{
		//if there is a \, than checks and the next one 
		if (line[i] == '\\')
		{
			//if the \ is not the last symbol it checks the next one
			if (i + 1 < line.size())
			{
				char nextSymbol = line[i + 1];
				//if the next symbol is one of the following, it appends its equivalent to the converted line
				switch (nextSymbol)
				{
				case 's':
					newLine = newLine + "(( )|(\t)|(\n))"; break;
				case 'd':
					newLine = newLine + "(1|2|3|4|5|6|7|8|9|0)"; break;
				case 'a':
					newLine = newLine + "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)"; break;
				case 'e':
					newLine = newLine + "\0"; break;
				default:
					newLine = newLine + "\\"; break;
				}
				//if the next symbol is one of the letters s,d,a or e,by the next turn of the loop it's skipped
				if (nextSymbol == 's' || nextSymbol == 'd' || nextSymbol == 'a' || nextSymbol == 'e') i++;
			}
			//if it is the last one, then just appends \ to the newly formed converted line
			else newLine = newLine + "\\";
		}
		//if the symbol is not \,then it appends it to the newly formed converted line
		else
		{
			newLine = newLine + line[i];
		}
	}
	//returns the newly formed converted line 
	return newLine;
}

//adds . to expicitly shows concatenation between expressions
std::string ConvertToPostfix::explicitConcatenation(std::string line)
{
	std::string newLine;
	//creates new expression line,containing the first symbol
	newLine += line[0];
	//checks all of the symbols in the original line
	for (size_t i = 1; i < line.size(); i++)
	{
		//checks if the previous symbol is *,) or a basic symbol i.e. showing end of an expression
		if (isBasicSymbol(line[i - 1]) || isKleeneStar(line[i - 1]) || line[i - 1] == ')')
		{
			//if the cuurent one is ( or a basic symbol, then appends . before it to show concatenation
			if (isBasicSymbol(line[i]) || line[i] == '(')
			{
				newLine += '.';
			}
		}
		//appends the symbol to the new expression line
		newLine += line[i];
	}
	//returns the new expression line
	return newLine;
}

//checks which of the two operators has higher priority
bool ConvertToPostfix::hasHigherPriority(char firstOperator, char secondOperator)
{
	//if the first one is *, than it has higher priority
	if (isKleeneStar(firstOperator))
	{
		//an exception is made, when the second one is * too
		if (isKleeneStar(secondOperator)) return false;
		else return true;
	}
	//checks if the first one is .
	else if (isConcatenation(firstOperator))
	{
		//if the second one is | than it has higher priority
		if (isUnion(secondOperator)) return true;
		//otherwise, it doesn't have (if it is * or |)
		else return false;
	}
	//if the first one is |, it has lower priority in all cases
	else return false;
}

//converts the expression line from infix to postfix
std::string ConvertToPostfix::regexToPostfix(std::string line)
{
	//checks if the regex is correct on number of brackets
	if (!correctBrakets(line)) std::cerr << "Incorrect regular expression!";
	//creates a new line, where the special symbols have been converted
	std::string convertedLine = convertSpecialSymbols(line);
	//. have been added to the new, converted line, where necessary
	convertedLine = explicitConcatenation(convertedLine);
	//stack that will contain the symbols temporary and help the convertion to postfix
	Stack<char> postfixExpression;
	//line that will contain the final postfix version of the expression
	std::string convertedExpression;
	//checks every symbol in the line
	for (size_t i = 0; i < convertedLine.size(); i++)
	{
		//if it is an operation
		if (isUnion(convertedLine[i]) || isConcatenation(convertedLine[i]) || isKleeneStar(convertedLine[i]))
		{
			//if the stack is empty, it pushes the symbol in the stack
			if (postfixExpression.isEmpty())
			{
				postfixExpression.push(convertedLine[i]);
			}
			else
			{
				if (postfixExpression.isEmpty()) std::cerr << "Incorrect regular expression!";
				//checks the element on the top
				char element = postfixExpression.onTop();
				//if it is an opertion, checks which of the two has a higher priority
				if (isUnion(element) || isConcatenation(element) || isKleeneStar(element))
				{
					bool priority = hasHigherPriority(convertedLine[i], element);
					//if the one on the top has a higher priority it's popped from the stack and appended to the converted expression
					if (!priority)
					{
						postfixExpression.pop(element);
						convertedExpression += element;
					}
				}
				//otherwise, the current symbol is pushed into the stack
				postfixExpression.push(convertedLine[i]);
			}
		}
		//if the current symbol is (, it just pueshes it into the stack
		else if (convertedLine[i] == '(')
		{
			postfixExpression.push(convertedLine[i]);
		}
		//if the current symbol is ), it pops all the symbols in the stack and appends them to the converted expression until it reaches (
		else if (convertedLine[i] == ')')
		{
			char top;
			postfixExpression.pop(top);
			while (top != '(')
			{
				convertedExpression += top;
				postfixExpression.pop(top);
			}
		}
		//if it is not an operation,( or ), the symbol is appended to the converted expression
		else
		{
			convertedExpression += convertedLine[i];
		}
	}
	//everything that is left in the stack is popped and appended to the converted expression
	while (!postfixExpression.isEmpty())
	{
		char element;
		postfixExpression.pop(element);
		convertedExpression += element;
	}
	//the stack is no longer necessary, so it is destroyed
	postfixExpression.destroy();
	//returns the postfix version of the regex
	return convertedExpression;
}