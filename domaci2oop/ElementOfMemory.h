#pragma once
#include <iostream>
#include <string>


class ElementOfMemory {
public:
	ElementOfMemory(const std::string& name, int value);
	int getValue();
	void setValue(int value);
	std::string getName();
	void setName(const std::string& name);
	bool operator>(ElementOfMemory* a);
	void printElMem();
private:
	std::string name_;
	int value_;
};