#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

#include "node.h"
#include "verilogOutput.h"

int main (int argc, char* argv[]){
	std::istringstream inSS;
	std::ifstream inFS;
	std::string line;
	std::string temp;
	std::vector<node> list;
	int i, j, size;
	char name;
	bool SIGN;

	if(argc != 3){  //check for correct input
		std::cout << "\nUSAGE: dpgen netlistFile verilogFile\n\n";
		return -1;
	}

	inFS.open(argv[1]);//open input file
	if(!inFS.is_open()){ //check opened correctly
		std::cout << "Could not open input file." << std::endl;
		return -1;
	}

	while(!inFS.eof()){ //process file
		getline(inFS, line);
		inSS.clear();
		inSS.str(line);
		if(!line.empty()){ //if line isn't empty (we ignore empty lines)
			std::istringstream iss(line);
			std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());  //break it into words
			if((results[0] == "input")||(results[0] == "output")||(results[0] == "wire")||(results[0] == "register")){ //get data types and variable names of inputs and outputs
				//parse data for node
				temp = results[1];
				if(temp[0] == 'U'){
					SIGN = 0; //unsigned 
				}
				else{
					SIGN = 1; //signed
				}
				j = 0;
				for(i = 0; i < temp.size(); ++i){ //get dataSize by removing letters from Int32, etc.
					if (isdigit(temp[i])){
						 temp[j] = temp[i];
						 j++;
					 }
				}
				while(j < temp.size()){
					temp[j] = NULL;
					j++;
				}
				std::istringstream (temp) >> size;
				for(i = 2; i < results.size(); i++){ //create a node for each variable with these stats
					temp = results[i]; //get rid of commas
					if(temp[1] == ','){			// Setting to NULL messes up the output to file, but since it looked like you were just setting temp[1]
						results[i] = temp[0];	// to NULL, it would mean the message would just end before that, so temp[0] is effectively the same.
					}
					else {
						results[i] = temp;
					}
					list.push_back(node(results[0],results[i],SIGN,size));
				}
			}
		}
	}
	

	//test print of list
	for(i = 0; i < list.size(); i++){
		std::cout << list[i].getName() << '\t' << list[i].getType() << '\t' << list[i].getSIGN() << '\t' << list[i].getDataSize() << std::endl;
	}
	std::cout << "\n\n\n";
	std::vector<node> list2;	//for when we do the operations
	generateVerilogFile(list, list2, argv[1], argv[2]);

	return 0;
}