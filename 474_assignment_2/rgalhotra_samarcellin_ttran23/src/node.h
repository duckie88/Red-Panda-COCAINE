#ifndef NODE_H
#define NODE_H

#include <string>

class node{
public:
	node(); //default constructor
	node(std::string type, std::string name, bool SIGN, int dataSize);
	void setName(char name);
	std::string getName();
	void setSIGN(bool SIGN);
	bool getSIGN();
	void setDataSize(int dataSize);
	int getDataSize();
	void setType(std::string type);
	std::string getType();
private:
	std::string type;
	std::string name;
	bool SIGN;
	int dataSize;
};

#endif