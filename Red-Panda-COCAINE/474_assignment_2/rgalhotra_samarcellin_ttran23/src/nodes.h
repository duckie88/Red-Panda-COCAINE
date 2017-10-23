#ifndef NODES_H
#define NODES_H

#include <string>

class node{
public:
	node(); //default constructor
	node(std::string type, std::string name, bool SIGN, int dataSize);
	void setName(std::string name);
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

class path{
public:
	path(); //default constructor
	path(std::string name, float delayLength);
	void setName(std::string name);
	std::string getName();
	void setDelayLength(float delayLength);
	float getDelayLength();
	void addDelay(float delay);
private:
	std::string name;
	float delayLength;
};

#endif