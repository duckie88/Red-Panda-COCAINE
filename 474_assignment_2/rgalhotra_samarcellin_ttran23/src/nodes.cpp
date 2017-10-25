#include "nodes.h"

node::node(){
	return;
}
	
node::node(std::string type, std::string name, bool SIGN, int dataSize){
	this->type = type;
	this->name = name;
	this->SIGN = SIGN;
	this->dataSize = dataSize;
	return;
}
	
void node::setType(std::string type){
	this->type = type;
	return;
}

std::string node::getType(){
	return this->type;
}

void node::setName(std::string name){
	this->name = name;
	return;
}

std::string node::getName(){
	return this->name;
}

void node::setSIGN(bool SIGN){
	this->SIGN = SIGN;
	return;
}

bool node::getSIGN(){
	return this->SIGN;
}

void node::setDataSize(int dataSize){
	this->dataSize = dataSize;
	return;
}

int node::getDataSize(){
	return this->dataSize;
}

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
	this->delayLength += delay;
	return;
}
