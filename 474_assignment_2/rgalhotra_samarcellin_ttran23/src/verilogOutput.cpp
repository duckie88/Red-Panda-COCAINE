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
	bool real3 = false;
	bool sign3 = false;
	int datawidth = 0;
	int indata = 0;
	for (i = 0; i < ioList.size(); i++) {
		if (ioList.at(i).getName() == oper1) {
			real1 = true;
			sign1 = ioList.at(i).getSIGN();
			if (ioList.at(i).getDataSize() > indata) indata = ioList.at(i).getDataSize();
		}
		if (ioList.at(i).getName() == oper2) {
			real2 = true;
			sign2 = ioList.at(i).getSIGN();
			if (ioList.at(i).getDataSize() > indata) indata = ioList.at(i).getDataSize();
		}
		if (ioList.at(i).getName() == result) {
			real3 = true;
			sign3 = ioList.at(i).getSIGN();
			if (ioList.at(i).getDataSize() > datawidth) datawidth = ioList.at(i).getDataSize();
		}
	}
	std::cout << sign1 << sign2 << std::endl;
	std::cout << type << "what"<< std::endl;
	std::cout << !real1 << !(real2 || type == "reg") << !real3 << std::endl;
	if (!real1 || !(real2 || type == "reg") || !real3) return "error"; //error case 1-3
	std::cout << type << std::endl;
	if (type == "+") {
		if(oper2 == "1") out = "INC #(.DATAWIDTH(" + std::to_string(datawidth) + ")) incrementor" + std::to_string(num) + "(" + oper1 + "," + result + ");";
		else out = "ADD #(.DATAWIDTH(" + std::to_string(datawidth) + ")) adder" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "-") {
		if (oper2 == "1") out = "DEC #(.DATAWIDTH(" + std::to_string(datawidth) + ")) decrementor" + std::to_string(num) + "(" + oper1 + "," + result + ");";
		else out = "SUB #(.DATAWIDTH(" + std::to_string(datawidth) + ")) subber" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "reg") {
		std::cout << "we got here" << std::endl;
		out = "REG #(.DATAWIDTH(" + std::to_string(datawidth) + ")) register" + std::to_string(num) + "(Clk,Rst" + "," + oper1 + "," + result + ");";
	}
	else if (type == "*") {
		out = "MUL #(.DATAWIDTH(" + std::to_string(datawidth) + ")) multiplier" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "==") {
		out = "COMP #(.DATAWIDTH(" + std::to_string(indata) + ")) comparator" + std::to_string(num) + "(" + oper1 + "," + oper2 + ", , ," + result + ");";
	}
	else if (type == ">") {
		out = "COMP #(.DATAWIDTH(" + std::to_string(indata) + ")) comparator" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ", , );";
	}
	else if (type == "<") {
		out = "COMP #(.DATAWIDTH(" + std::to_string(indata) + ")) comparator" + std::to_string(num) + "(" + oper1 + "," + oper2 + ", ," + result + ", );";
	}
	else if (type == "<<") {
		out = "SHL #(.DATAWIDTH(" + std::to_string(datawidth) + ")) shiftleft" + std::to_string(num) + "(" + oper2 + "," + oper1 + "," + result + ");";
	}
	else if (type == ">>") {
		out = "SHR #(.DATAWIDTH(" + std::to_string(datawidth) + ")) shiftright" + std::to_string(num) + "(" + oper2 + "," + oper1 + "," + result + ");";
	}
	else if (type == "/") {
		out = "DIV #(.DATAWIDTH(" + std::to_string(datawidth) + ")) divider" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "%") {
		out = "MOD #(.DATAWIDTH(" + std::to_string(datawidth) + ")) modulus" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else {
		return "error"; //error case 4
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
	bool sign3 = false;
	bool real3 = false;
	int datawidth = 0;
	for (i = 0; i < ioList.size(); i++) {
		if (ioList.at(i).getName() == oper1) {
			real1 = true;
			sign1 = ioList.at(i).getSIGN();
		}
		if (ioList.at(i).getName() == oper2) {
			real2 = true;
			sign2 = ioList.at(i).getSIGN();
		}
		if (ioList.at(i).getName() == result) {
			real3 = true;
			sign3 = ioList.at(i).getSIGN();
			if (ioList.at(i).getDataSize() > datawidth) datawidth = ioList.at(i).getDataSize();
		}
	}
	if (!real1 || !real2 || !real3) return "error"; //error case 1-3
	if (sign1 || sign2) return "SMUX2x1 mux" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + oper3 + "," + result + ");";
	else return "MUX2x1 #(.DATAWIDTH(" + std::to_string(datawidth) + ")) mux" + std::to_string(num) + "(" + oper1 + "," + oper2 + "," + oper3 + "," + result + ");";
}

void generateVerilogFile(std::vector<node> ioList, std::vector<std::string> moduleList, char* inFileStr, char* outFileStr) {
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
	for (int i = 0; i < moduleList.size(); i++) {
		outFS << moduleList.at(i) << std::endl;
	}
	outFS << "endmodule" << std::endl;
	outFS.close();
}