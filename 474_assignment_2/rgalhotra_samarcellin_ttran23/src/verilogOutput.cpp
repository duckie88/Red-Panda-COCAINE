#include "verilogOutput.h"

void generateIO(std::vector<node> list, char* outFileStr) {
	// Variables for string concatenation
	std::stringstream ss;
	std::string ioTemp;
	// Variables for file, appending
	std::ofstream myFile;
	myFile.open(outFileStr, std::ios::app);

	// Looping through
	for (int i = 0; i < list.size(); i++) {
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
	myFile << std::endl << std::endl;
	myFile.close();
}

void generateModule() {

}

void generateVerilogFile(std::vector<node> ioList, std::vector<node> moduleList, char* inFileStr, char* outFileStr) {
	// https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
	std::string moduleName = inFileStr;
	moduleName = moduleName.substr(moduleName.find_last_of("/\\") + 1);	
	moduleName = moduleName.substr(0, moduleName.find_last_of("."));

	std::ofstream outFS;
	outFS.open(outFileStr);//open output file
	if (!outFS.is_open()){ //check opened correctly
		std::cout << "Could not open output file." << std::endl;
		exit(EXIT_FAILURE);
	}

	// Start of module
	outFS << "module " << moduleName << "( ";
	for (int i = 0; i < ioList.size() - 1; i++) {
		outFS << ioList.at(i).getName() << ", ";
	}
	outFS << ioList.at(ioList.size()-1).getName() << " );\n";
	outFS.close();
	generateIO(ioList, outFileStr);

	outFS.open(outFileStr, std::ios::app);
	outFS << "endmodule" << std::endl;
	outFS.close();
}