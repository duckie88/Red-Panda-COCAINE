#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "node.h"

int main (int argc, char* argv[]){
	std::istringstream inSS;
	std::ifstream inFS;
	std::ofstream outFS;
	std::string line;

	if(argc != 3){  //check for correct input
		std::cout << "\nUSAGE: dpgen netlistFile verilogFile\n\n";
		return -1;
	}

	inFS.open(argv[1]);//open input file
	if(!inFS.is_open()){ //check opened correctly
		std::cout << "Could not open input file." << std::endl;
		return -1;
	}

	outFS.open(argv[2]);//open output file
	if(!outFS.is_open()){ //check opened correctly
		std::cout << "Could not open output file." << std::endl;
		return -1;
	}

	while(!inFS.eof()){ //process file
		getline(inFS, line);
		inSS.clear();
		inSS.str(line);
		if(!line.empty()){ //if line isn't empty (we ignore empty lines)
			std::istringstream iss(line);
			std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());  //break it into words
			if((results[0] == "input")||(results[0] == "output")||(results[0] == "wire")){ //get data types and variable names of inputs and outputs
				//put data in node
			}
			//deal with actual math (make sure to error check - all variables should be an input, output, or wire in a node!
		}
	}



	return 0;
}