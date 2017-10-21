#include "verilogOutput.h"

void generateIO(std::vector<node> list, char* outFileStr) {
	// Variables for string concatenation
	std::stringstream ss;
	std::string ioTemp;
	// Variables for file, appending
	std::ofstream myFile;
	myFile.open(outFileStr, std::ios::app);

	// Looping through
	for (int i = 1; i < list.size(); i++) {
	//list[i].getName() << list[i].getType() << list[i].getSIGN() << list[i].getDataSize() 
		if (list.at(i).getType().compare("wire") == 0) {
			ss = std::stringstream();	// Clear string stream
			ss << "wire [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
			ioTemp = ss.str();
			myFile << ioTemp;
		}
		else if (list.at(i).getType().compare("input") == 0) {
			ss = std::stringstream();	// Clear string stream
			ss << "input [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
			ioTemp = ss.str();
			myFile << ioTemp;
		}
		else if (list.at(i).getType().compare("output") == 0) {
			ss = std::stringstream();	// Clear string stream
			ss << "output [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
			ioTemp = ss.str();
			myFile << ioTemp;
		}
		else {
			std::cout << "Not an input, output, or wire" << std::endl;
		}
	}
	myFile << '\n\n';
	myFile.close();
}

void generateModule() {

}

void generateVerilogFile(char* outFileStr) {

}