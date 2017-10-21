#ifndef VERILOGOUTPUT_H
#define VERILOGOUTPUT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "node.h"

// Call it as: generateVerilogFile(argv[2]);
void generateVerilogFile(std::vector<node> ioList, std::vector<node> moduleList, char* inFileStr, char* outFileStr);
void generateIO(std::vector<node> list, char* outFileStr);
std::string generateModule(std::string result, std::string oper1, std::string oper2, std::string type);
std::string generateMux(std::string result, std::string oper1, std::string oper2, std::string oper3);

#endif // VERILOGOUTPUT_H