#include "path.h"

path::path(){
	return;
}

path::path(std::string name, float delayLength){
	this->name = name;
	this->delayLength = delayLength;
	return;
}
	
void path::setName(std::string name){
	this->name = name;
	return;
}

std::string path::getName(){
	return this->name;
}
	
void path::setDelayLength(float delayLength){
	this->delayLength = delayLength;
	return;
}

float path::getDelayLength(){
	return this->delayLength;
}

void path::addDelay(float delay){
	this->delayLength += delayLength;
	return;
}


