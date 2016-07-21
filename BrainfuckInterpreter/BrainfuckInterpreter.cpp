
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	//The size of the tape
	size_t allocSize = 2048;

	
	if (argc == 1) {
		std::cout << "No file specified!\n";
		return 1;
	}

	
	if (strcmp(argv[1], "-help") == 0) {
		std::cout << "Program that interprets BrainFuck. Takes two parameters: \n\t-The name of the file that contains the code, relative to " << argv[0] <<
			"\n\t-(Optional)The size of the BrainFuck tape. Initial position is in the middle of the tape.\n\tMust be divisible by two, and fit in size_t" << std::endl;
		return 0;
	}

	//If the user has given us the size of the tape
	if (argc == 3) {
		try
		{
			allocSize = std::stoul(argv[2]);
			if (allocSize % 2 != 0) throw std::invalid_argument("Not divisible by two!");
		}
		catch (...)
		{
			std::cout << "Error with the second parameter, use -help for instructions\n";
			return 1;
		}
		

	}


	//Open the file
	std::ifstream f(argv[1]);

	if (!f.is_open()) {
		std::cout << "Failed to open the file!\n";
		return 1;
	}

	std::string src;

	//Copy the contents of the file to the string
	f.seekg(0, std::ios::end);
	src.reserve(f.tellg());
	f.seekg(0, std::ios::beg);

	src.assign((std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>());

	f.close();


	//Remove single-line comments
	int index = src.find("//");

	while (index != -1) {
		src.erase(index, src.find_first_of('\n', index) - index);
		index = src.find("//", index);
	}

	//Remove multi-line comments
	index = src.find("/*");
	
	while (index != -1) {
		src.erase(index, src.find("*/", index) + 2 - index );
		index = src.find("/*", index);
	}

	//Remove spaces, newlines and tabs
	src.erase(std::remove(src.begin(), src.end(), ' '), src.end());
	src.erase(std::remove(src.begin(), src.end(), '\n'), src.end());
	src.erase(std::remove(src.begin(), src.end(), '\t'), src.end());


	//Pointer to the start of the tape, needed for deletion
	char* tapeStart = new char[allocSize];
	
	//Current cell, starts in the middle of the tape
	char* ptr = tapeStart + allocSize / 2;

	//Set all cells to 0
	memset(tapeStart, 0, allocSize);
	
	std::cout << "---------- Start Execution ----------\n";

	//Go through the source
	for (auto i = src.begin(); i != src.end(); i++) {
	
		switch (*i)
		{

		case '+': (*ptr)++; break;		
		case '-': (*ptr)--; break;		
		case '>': ptr++; break;	
		case '<': ptr--; break;		
		case '.': std::cout << (*ptr); break;
		case ',': std::cin >> (*ptr); break;

		case '[': {
			if (*ptr == 0) {
				//At least there isn't an Infinite loop on syntax error, but no error message either :/
				while (*i != ']' && i != src.end()) i++;			
				continue;
			}
			break;
		}

		case ']': {
			if (*ptr != 0) {
				while (*i != '[' && i != src.end()) i--;
				continue;
			}
			break;
		}

		default:
			std::cout << "Illegal character " << *i << "!\n";
			return 1;
			break;
		}
	
	} 

	std::cout << "\n---------- Stop Execution ----------\n";

	delete tapeStart;

}

