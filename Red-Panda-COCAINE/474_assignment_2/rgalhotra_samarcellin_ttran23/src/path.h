#ifndef PATH_H
#define PATH_H

#include <string>

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