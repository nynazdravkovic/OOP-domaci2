#include "ElementOfMemory.h"
using namespace std;

ElementOfMemory::ElementOfMemory(const std::string& name, int value):name_(name),value_(value){}
int ElementOfMemory::getValue(){
	return this->value_;
}

void ElementOfMemory::setValue(int value){
	this->value_ = value;
}

std::string ElementOfMemory::getName()
{
	return this->name_;
}

void ElementOfMemory::setName(const std::string &name){
	this->name_ = name;
}

bool ElementOfMemory::operator>(ElementOfMemory* a){
	return (name_>a->name_);
}

void ElementOfMemory::printElMem(){
	cout << name_ << " " << value_<<endl;
}
