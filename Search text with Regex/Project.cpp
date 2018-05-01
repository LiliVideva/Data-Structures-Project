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

#include"stdafx.h"
#include"dirent.h"
#include"DynamicArray.h"
#include"FindRegexPattern.h"
#include<iostream>

//checks if the file is a text file
bool isTextFile(std::string filename)
{
	//if the last letters are txt, then it is a text file
	if (filename.length() > 3)
	{
		if (filename.substr(filename.size() - 3) == "txt") return true;
	}
	//otherwise, it isn't
	return false;
}

//finds all text files in the directory
void findTextFilesInDirectory(std::string directory, DynamicArray<std::string>& files)
{
	//variables, necessary to work with directories
	DIR* parentDirectory;
	struct dirent* parentDir;
	//if the directory can't be opened, then a message is printed to the user
	if ((parentDirectory = opendir(directory.c_str())) == NULL)
	{
		std::cout << "Wrong directory or file name!" << directory << std::endl;
	}
	//otherwise it reads from the directory
	while ((parentDir = readdir(parentDirectory)) != NULL)
	{
		std::string p1 = ".";
		std::string p2 = "..";
		//if the directory is the current one, or the parenting one
		if (parentDir->d_name != p1 && parentDir->d_name != p2)
		{
			DIR* childDirectory;
			std::string newDirectory = directory + "\\" + parentDir->d_name;
			//if this is a text file, it whole name is appended to the array of files
			if (isTextFile(parentDir->d_name)) files.append(newDirectory);
			//otherwise, if it is a directory the funciton is recursively executed
			else if ((childDirectory = opendir(newDirectory.c_str())) != NULL)
			{
				findTextFilesInDirectory(newDirectory, files);
			}
		}
	}
	//closes the directory
	closedir(parentDirectory);
}

//gets just the name of the file, not with its whole name
DynamicArray<std::string> getListOfFilenames(std::string file)
{
	DynamicArray<std::string> listOfFilenames;
	std::string tempFilename;
	//if the file is a text file, its name is appended to the list of filenames
	if (isTextFile(file))
	{
		for (int i = file.size() - 1; i >= 0; i--)
		{
			if (file[i] == '\\') break;
			else tempFilename = file[i] + tempFilename;
		}
		listOfFilenames.append(tempFilename);
		return listOfFilenames;
	}
	//otherwise it's a directory and the list of files it's got from there
	findTextFilesInDirectory(file, listOfFilenames);
	//returns the list of all files
	return listOfFilenames;
}

int main(size_t argc, char* argv[])
{
	//checks if the arguments are enough and if they aren't, sends a proper message to the user
	if (argc < 2)
	{
	std::cerr << "Usage: " << argv[0] << " SOURCE DESTINATION " << std::endl;
	return 1;
	}
	//gets the list of filenames from the first argument
	DynamicArray<std::string> filenames = getListOfFilenames(argv[1]);
	//gets the regex from the second argument
	std::string regex = std::string(argv[2]);
	//finds the lines in the files, matching the regex
	FindRegexPattern matchRegexPattern;
	matchRegexPattern.findPattern(filenames, regex);
	return 0;
}


