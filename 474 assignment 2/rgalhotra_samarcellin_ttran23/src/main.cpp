#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

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
		if(!line.empty()){ //if line isn't empty
			//do stuff
			outFS << line << std::endl;//print for testing atm
		}
		if(line.empty()){ //if it is empty, skip it
			break;
		}
	}



	return 0;
}