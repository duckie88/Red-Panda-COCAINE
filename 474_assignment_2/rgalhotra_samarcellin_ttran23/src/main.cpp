#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <math.h>

#include "nodes.h"
#include "verilogOutput.h"

int main(int argc, char* argv[]) {
	std::istringstream inSS;
	std::ifstream inFS;
	std::string line;
	std::string temp;
	std::vector<node> list;
	std::vector<std::string> list2;
	std::vector<path> crit;
	std::string result;
	std::string oper1;
	std::string oper2;
	std::string oper3;
	std::string type;
	int critTemp, x = 0;
	int i, j, size, num = 0;
	char name;
	float max = 0;
	bool SIGN = false;
	bool clkrst = false;
	
	const float RegDelays[] = {2.616,2.644,2.879,3.061,3.602,3.966};
	const float AddDelays[] = {2.704,3.713,4.924,5.638,7.270,9.566};
	const float SubDelays[] = {3.024,3.412,4.890,5.569,7.253,9.566};
	const float MulDelays[] = {2.438,3.651,7.453,7.811,12.395,15.354};
	const float CompDelays[] = {3.031,3.934,5.949,6.256,7.264,8.416};
	const float MuxDelays[] = {4.083,4.115,4.815,5.623,8.079,8.766};
	const float SHRDelays[] = {3.644,4.007,5.178,6.460,8.819,11.095};
	const float SHLDelays[] = {3.614,3.980,5.152,6.549,8.565,11.220};
	const float DivDelays[] = {0.619,2.144,15.439,33.093,86.312,243.233};
	const float ModDelays[] = {0.758,2.149,16.078,35.563,88.142,250.583};
	const float IncDelays[] = {1.792,2.218,3.111,3.471,4.347,6.200};
	const float DecDelays[] = {1.792,2.218,3.108,3.701,4.685,6.503};

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
					SIGN = false; //unsigned 
				}
				else {
					SIGN = true; //signed
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
					if(results[0] == "input"){ //if it's an input, also save for crit path with delay 0
						crit.push_back(path(results[i],0));
					}
				}
			}
			else if (results[1] == "=") {
				
				if (results.size() == 3) { //register ie: a = b
					result = results[0];
					oper1 = results[2];
					type = "reg";
					list2.push_back(generateModule(result, oper1, "help", type, num, list));
					num++;
					if (!clkrst) { //add clk and rst if not already in module to account for register
						list.push_back(node("input", "Clk", true, 1));
						list.push_back(node("input", "Rst", true, 1));
						clkrst = true;
					}

					//add to critical path
					for(i = 0; i < crit.size(); i++){
						if(results[2] == crit.at(i).getName()){
							for(j = 0; j < list.size(); j++){
								if(results[0] == list.at(j).getName()){
									critTemp = list.at(j).getDataSize();
									x = log(critTemp)/log(2); //get index of delay array
									crit.push_back(path(results[0],crit.at(i).getDelayLength() + RegDelays[x]));
								}
							}
						}
					}
				}
				else if (results[3] == "?") {  //mux
					result = results[0];
					oper1 = results[2];
					oper2 = results[4];
					oper3 = results[6];
					type = results[3];
					list2.push_back(generateMux(result, oper1, oper2, oper3, num, list));
					num++;

					//add to critical path
					for(i = 0; i < crit.size(); i++){
						if((results[2] == crit.at(i).getName()) || (results[4] == crit.at(i).getName()) || (results[6] == crit.at(i).getName())){
							for(j = 0; j < list.size(); j++){
								if(results[0] == list.at(j).getName()){
									critTemp = list.at(j).getDataSize();
									x = log(critTemp)/log(2); //get index of delay array
									crit.push_back(path(results[0],crit.at(i).getDelayLength() + RegDelays[x]));
								}
							}
						}
					}

				}
				else { //rest
					result = results[0];
					oper1 = results[2];
					oper2 = results[4];
					type = results[3];
					list2.push_back(generateModule(result, oper1, oper2, type, num, list));
					num++;

					//add to critical path
					for(i = 0; i < crit.size(); i++){
						if((results[2] == crit.at(i).getName()) || (results[4] == crit.at(i).getName())){
							for(j = 0; j < list.size(); j++){
								if(results[0] == list.at(j).getName()){
									critTemp = list.at(j).getDataSize();
								}
							}
							x = log(critTemp)/log(2);

							if(results[3] == "+"){
								if(results[4] != "1"){
									crit.push_back(path(results[0],crit.at(i).getDelayLength() + AddDelays[x]));
								}
								else{
									crit.push_back(path(results[0],crit.at(i).getDelayLength() + IncDelays[x]));
								}
							}
							else if(results[3] == "-"){
								if(results[4] != "1"){
									crit.push_back(path(results[0],crit.at(i).getDelayLength() + SubDelays[x]));
								}
								else{
									crit.push_back(path(results[0],crit.at(i).getDelayLength() + DecDelays[x]));
								}
							}
							else if(results[3] == "*"){
								crit.push_back(path(results[0],crit.at(i).getDelayLength() + MulDelays[x]));
							}
							else if((results[3] == "<")  || (results[3] == ">") || (results[3] == "==")){
								crit.push_back(path(results[0],crit.at(i).getDelayLength() + CompDelays[x]));
							}
							else if(results[3] == ">>"){
								crit.push_back(path(results[0],crit.at(i).getDelayLength() + SHRDelays[x]));
							}
							else if(results[3] == "<<"){
								crit.push_back(path(results[0],crit.at(i).getDelayLength() + SHLDelays[x]));
							}
							else if(results[3] == "/"){
								crit.push_back(path(results[0],crit.at(i).getDelayLength() + DivDelays[x]));
							}
							else if(results[3] == "%"){
								crit.push_back(path(results[0],crit.at(i).getDelayLength() + ModDelays[x]));
							}
						}
					}
				}
			}
		}
	}

	generateVerilogFile(list, list2, argv[1], argv[2]);

	for(i = 0; i < crit.size(); i++){
		if(crit.at(i).getDelayLength() > max){
			max = crit.at(i).getDelayLength();
		}
	}

	//test print of crit
			for (i = 0; i < crit.size(); i++) {
				std::cout << crit[i].getName() << '\t' << crit[i].getDelayLength() << std::endl;
			}
			std::cout << "\n\n\n";

	std::cout << "Critical Path Delay : " << max << "ns" << std::endl;	

	return 0;
}