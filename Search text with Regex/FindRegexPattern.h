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
#include"DynamicArray.h"
#include"State.h"
#include"ConvertToPostfix.h"
#include"NFA.h"
#include"CheckTextByNFA.h"
#include<iostream>
#include<fstream>
#include<sstream>

//class, responsible for getting all lines from the files and checking if they are recognised by the NFA
class FindRegexPattern
{
private:
	DynamicArray<std::string> readFromFile(std::string);
	std::string convertStringToLower(std::string*);
public:
	void findPattern(DynamicArray<std::string>, std::string);
};

//reads the lines from a text file
DynamicArray<std::string> FindRegexPattern::readFromFile(std::string filename)
{
	DynamicArray<std::string> fileLines;
	std::string line;
	std::ifstream file;
	//opens the file
	file.open(filename);
	//if it hasn't opened properly, it returns a message to the user
	if (file.fail())
	{
		std::cout << "Couldn't open the file !!!" << std::endl;
		std::exit(1);
	}
	//otherwise it append every line in the file to an array and closes file in the end
	else
	{
		while (std::getline(file, line))
		{
			fileLines.append(line);
		}
		file.close();
	}
	//returns the array of lines in the file
	return fileLines;
}

//makes the line case-insensitive
std::string FindRegexPattern::convertStringToLower(std::string* line)
{
	std::string convertedLine;
	char symbol;
	//checks every symbol in the line
	for (size_t i = 0; i< line->size(); i++)
	{
		char currentSymbol = line->at(i);
		//if it is an upper case, it is turned into a lower case and appended to the new, converted line
		if (currentSymbol <= 'Z' && currentSymbol >= 'A')
		{
			symbol = currentSymbol + 32;
			convertedLine = convertedLine + symbol;
		}
		//otherwise it is just appended to the new, converted line
		else
			convertedLine = convertedLine + currentSymbol;
	}
	//returns the new line in lower case
	return convertedLine;
}

//find all lines in the files matching the regex
void FindRegexPattern::findPattern(DynamicArray<std::string> listOfFilenames, std::string regex)
{
	//converts the regex to a lower case
	std::string convertedRegex = convertStringToLower(&regex);
	std::string checkedLine;

	//checks all available files
	for (size_t i = 0; i < listOfFilenames.getSize(); i++)
	{
		//gets just the name of the file, not the whole path to the file
		std::string filename;
		for (int k = listOfFilenames.getElement(i).size() - 1; k >= 0; k--)
		{
			if (listOfFilenames.getElement(i)[k] == '\\') break;
			else filename = listOfFilenames.getElement(i)[k] + filename;
		}
		//creates an array, containing all lines in the current file
		DynamicArray<std::string> fileLines = readFromFile(listOfFilenames.getElement(i));
		//checks every line in the current file
		for (size_t j = 0; j < fileLines.getSize(); j++)
		{
			checkedLine = fileLines.getElement(j);
			//checks every symbol in the current line
			for (size_t l = 0; l < checkedLine.size(); l++)
			{
				//converts the line to a lower case
				std::string convertedCombination = convertStringToLower(&checkedLine);
				//variables, helping turn the lower case regex to a postfix and then creating NFA from it
				ConvertToPostfix transformToPostfix;
				NFA createNFA;
				State* initialState = createNFA.postfixToNFA(transformToPostfix.regexToPostfix(convertedRegex));
				//checks if the line is recognised by the NFA
				CheckTextByNFA matchedPatterns;
				bool matches = matchedPatterns.checkLine(initialState, convertedCombination);
				//if there is no regex, all of the lines are matching the pattern
				if (convertedRegex == "")
				{
					std::cout << listOfFilenames.getElement(i) << ":" << j + 1 << ":" << std::endl;
					break;
				}
				//otherwise, if there is a match, prints a line containing the name of the file, the matched line and its number
				else if (matches)
				{
					std::cout << filename << ":" << j + 1 << ":" << checkedLine << std::endl;
					break;
				}
			}
			//if the symbol is the empty word
			if (checkedLine.size() == 0)
			{
				//converts the line to a lower case
				std::string convertedCombination = convertStringToLower(&checkedLine);
				//variables, helping turn the lower case regex to a postfix and then creating NFA from it
				ConvertToPostfix transformToPostfix;
				NFA createNFA;
				State* initialState = createNFA.postfixToNFA(transformToPostfix.regexToPostfix(convertedRegex));
				//checks if the line is recognised by the NFA
				CheckTextByNFA matchedPatterns;
				bool matches = matchedPatterns.checkLine(initialState, convertedCombination);
				//if there is no regex and it is not \e, then every line matches the pattern
				if (convertedRegex == "" && regex != "\\e")
				{
					std::cout << listOfFilenames.getElement(i) << ":" << j + 1 << ":" << std::endl;
					break;
				}
				//otherwise, if there is a match, prints a line containing the name of the file, the matched line and its number
				else if (matches)
				{
					std::cout << filename << ":" << j + 1 << ":" << checkedLine << std::endl;
				}
			}
		}
	}
}