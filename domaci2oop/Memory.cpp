#include "Memory.h"

Memory::~Memory(){
	int i = 0;
	while (i<memory_.size()) {
		delete memory_[i];
		i++;
	}
}

void Memory::addVariable(ElementOfMemory* newElem){
	memory_.push_back(newElem);
}

ElementOfMemory* Memory::findVariable(std::string name){
	for (int i = 0; i < memory_.size();i++) {
		if (memory_[i]->getName() == name) return memory_[i];
	}
	return nullptr;
}

int Memory::getSize(){
	return memory_.size();
}

ElementOfMemory* Memory::getElem(int i){
	return memory_[i];
}

void Memory::sortMemByName(){
	for (int i = 0; i < memory_.size()-1; i++) {
		for (int j = i+1; j < memory_.size(); j++) {
			if (memory_[i]->getName() > memory_[j]->getName()) {
				ElementOfMemory* temp = memory_[i];
				memory_[i] = memory_[j];
				memory_[j] = temp;
			}
		}
	}
}

void Memory::printMem(){
	std::cout << "printujem mem" << std::endl;
	if (memory_.empty()) { std::cout << "prazno" << std::endl; }
	else{
		for (auto e : memory_) {
			std::cout << e->getName() <<" "<<e->getValue()<< std::endl;
		}
	}
}


