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

std::string generateModule(std::string result, std::string oper1, std::string oper2, std::string type, int num, std::vector<node>ioList) {
	//switch statement based off of type
	//if - or + check if oper2 is 1, then use inc or dec
	//default case to check for error where it's not a real operator
	std::string out;
	int i = 0;
	bool real1 = false;
	bool sign1 = false;
	bool real2 = false;
	bool sign2 = false;
	for (i = 0; i < ioList.size() - 1; i++) {
		if (ioList.at(i).getName() == oper1) {
			real1 = true;
			sign1 = ioList.at(i).getSIGN();
		}
		if (ioList.at(i).getName() == oper2) {
			real2 = true;
			sign2 = ioList.at(i).getSIGN();
		}
	}
	if (!real1 || !real2) return; //error case 1-3
	if (type == "+") {
		if(oper2 == "1") out = "INC incrementor" + std::to_string(num) + "(" + oper1 + "," + result + ");";
		else out = "ADD adder" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "-") {
		if (oper2 == "1") out = "DEC decrementor" + std::to_string(num) + "(" + oper1 + "," + result + ");";
		else out = "SUB subber" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "reg") {
		out = "REG register" + std::to_string(num) + "(Clk, Rst" + "," + oper1 + "," + result + ");";
	}
	else if (type == "mul") {
		out = "MUL multiplier" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "==") {
		out = "COMP comparator" + std::to_string(num) + "(" + oper1 + "," + oper2 + ", , ," + result + ");";
	}
	else if (type == ">") {
		out = "COMP comparator" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ", , );";
	}
	else if (type == "<") {
		out = "COMP comparator" + std::to_string(num) + "(" + oper1 + "," + oper2 + ", ," + result + ", );";
	}
	else if (type == "<<") {
		out = "SHL shiftleft" + std::to_string(num) + "(" + oper2 + "," + oper1 + "," + result + ");";
	}
	else if (type == ">>") {
		out = "SHR shiftright" + std::to_string(num) + "(" + oper2 + "," + oper1 + "," + result + ");";
	}
	else if (type == "/") {
		out = "DIV divider" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "%") {
		out = "MOD modulus" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else {
		return; //error case 4
	}
	if (sign1 || sign2) return "S" + out;
	else return out;
}

std::string generateMux(std::string result, std::string oper1, std::string oper2, std::string oper3, int num, std::vector<node>ioList) {
	int i = 0;
	bool real1 = false;
	bool sign1 = false;
	bool real2 = false;
	bool sign2 = false;
	for (i = 0; i < ioList.size() - 1; i++) {
		if (ioList.at(i).getName() == oper1) {
			real1 = true;
			sign1 = ioList.at(i).getSIGN();
		}
		if (ioList.at(i).getName() == oper2) {
			real2 = true;
			sign2 = ioList.at(i).getSIGN();
		}
	}
	if (!real1 || !real2) return; //error case 1-3
	if (sign1 || sign2) return "SMUX2x1 mux" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + oper3 + "," + result + ");";
	else return "MUX2x1 mux" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + oper3 + "," + result + ");";
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
		if (ioList.at(i).getType().compare("input") == 0 || ioList.at(i).getType().compare("output") == 0) {
			outFS << ioList.at(i).getName() << ", ";
		}
	}
	outFS << ioList.at(ioList.size()-1).getName() << " );\n";
	outFS.close();
	generateIO(ioList, outFileStr);

	outFS.open(outFileStr, std::ios::app);
	outFS << "endmodule" << std::endl;
	outFS.close();
}