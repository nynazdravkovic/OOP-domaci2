#pragma once
#include <iostream>
#include <fstream>
#include <stack>

#include "Exceptions.h"
#include "Memory.h"
#include "Instruction.h"

class Machine {
public:
	Machine();
	~Machine();
	void loadProgram(const std::string& filepath);
	void execute(const std::string& filepath);
	void setNextInstructions();
	static Memory* mem;
private:
	std::vector<Instruction*> instructions;
	std::stack<Instruction*> stackIf;
	std::stack<Instruction*> stackLoop;
	std::stack<Instruction*> beaforeElse;
};
