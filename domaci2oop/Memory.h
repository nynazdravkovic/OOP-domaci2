#pragma once
#include <iostream>
#include <vector>

#include "ElementOfMemory.h"

class Memory {
public:
	~Memory();
	void addVariable(ElementOfMemory* newElem);
	ElementOfMemory* findVariable(std::string name);
	int getSize();
	ElementOfMemory* getElem(int i);
	void sortMemByName();
	void printMem();
private:
	std::vector<ElementOfMemory*> memory_;
};