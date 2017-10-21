#ifndef VERILOGOUTPUT_H
#define VERILOGOUTPUT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "node.h"

// Call it as: generateVerilogFile(argv[2]);
void generateVerilogFile(char* outFileStr);
void generateIO(std::vector<node> list, char* outFileStr);
void generateModule();


#endif // VERILOGOUTPUT_H