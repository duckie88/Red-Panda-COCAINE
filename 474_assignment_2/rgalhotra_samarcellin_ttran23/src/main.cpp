#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

#include "node.h"
#include "verilogOutput.h"

int main(int argc, char* argv[]) {
	std::istringstream inSS;
	std::ifstream inFS;
	std::string line;
	std::string temp;
	std::vector<node> list;
	std::vector<std::string> list2;
	std::string result;
	std::string oper1;
	std::string oper2;
	std::string oper3;
	std::string type;
	int i, j, size, num = 0;
	char name;
	bool SIGN = false;
	bool clkrst = false;

	if (argc != 3) {  //check for correct input
		std::cout << "\nUSAGE: dpgen netlistFile verilogFile\n\n";
		return -1;
	}

	inFS.open(argv[1]);//open input file
	if (!inFS.is_open()) { //check opened correctly
		std::cout << "Could not open input file." << std::endl;
		return -1;
	}

	while (!inFS.eof()) { //process file
		getline(inFS, line);
		inSS.clear();
		inSS.str(line);
		if (!line.empty()) { //if line isn't empty (we ignore empty lines)
			std::istringstream iss(line);
			std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());  //break it into words
			if ((results[0] == "input") || (results[0] == "output") || (results[0] == "wire") || (results[0] == "register")) { //get data types and variable names of inputs and outputs
																															   //parse data for node
				temp = results[1];
				if (temp[0] == 'U') {
					SIGN = 0; //unsigned 
				}
				else {
					SIGN = 1; //signed
				}
				j = 0;
				for (i = 0; i < temp.size(); ++i) { //get dataSize by removing letters from Int32, etc.
					if (isdigit(temp[i])) {
						temp[j] = temp[i];
						j++;
					}
				}
				while (j < temp.size()) {
					temp[j] = NULL;
					j++;
				}
				std::istringstream(temp) >> size;
				for (i = 2; i < results.size(); i++) { //create a node for each variable with these stats
					temp = results[i]; //get rid of commas
					temp.erase(std::remove(temp.begin(), temp.end(), ','), temp.end());
					results[i] = temp;
					list.push_back(node(results[0],results[i],SIGN,size));
				}
			}
			else if (results[1] == "=") {
				//std::cout << results[3] << std::endl; //for debugging
				if (results.size() == 3) {
					std::cout << "this actually happens" << std::endl;
					result = results[0];
					oper1 = results[2];
					type = "reg";
					std::cout << type << std::endl;
					list2.push_back(generateModule(result, oper1, "help", type, num, list));
					num++;
					if (!clkrst) {
						list.push_back(node("input", "Clk", true, 1));
						list.push_back(node("input", "Rst", true, 1));
						clkrst = true;
					}
				}
				else if (results[3] == "?") { //how do we handle the module names?
					result = results[0];
					oper1 = results[2];
					oper2 = results[4];
					oper3 = results[6];
					type = results[3];
					list2.push_back(generateMux(result, oper1, oper2, oper3, num, list));
					num++;
				}
				else {
					result = results[0];
					oper1 = results[2];
					oper2 = results[4];
					type = results[3];
					std::cout << results[3] << "this" << std::endl;
					std::cout << type << "sucks" << std::endl;
					list2.push_back(generateModule(result, oper1, oper2, type, num, list));
					num++;
				}
			}
		}
	}

	//test print of list
	for (i = 0; i < list.size(); i++) {
		std::cout << list[i].getName() << '\t' << list[i].getType() << '\t' << list[i].getSIGN() << '\t' << list[i].getDataSize() << std::endl;
	}
	std::cout << "\n\n\n";
	generateVerilogFile(list, list2, argv[1], argv[2]);

	return 0;
}