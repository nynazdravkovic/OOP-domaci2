#pragma once
#include <string>
#include "Memory.h"

class Instruction {
public:
	virtual ~Instruction() {};
	Instruction();
	static std::string findWord(const std::string& line, int& start);//vraca mi rec u stringu i updatuje dokle smo stigli u stringu  
	virtual Instruction* run()=0; // kada se pozove run vratice pokazivac na sledecu instrukciju koja se izvrsava
	std::string getNameOfIstruction();
	char getMarker();
	void setNext(Instruction* next);
	virtual void printuj();
	int numOfSpaces(const std::string& line);
	int lineNum_;
	int getLineNum();

protected:
	int getValueFromMem(const std::string& name);
	void setValueInMem(ElementOfMemory* elem, int newVal);
	Instruction* next_;
	std::string nameOfInst_;
	char marker_;
};

class ArithmeticInstruction : public Instruction {
public:
	ArithmeticInstruction(const std::string& line,int linenum);
	Instruction* run() override;
private:
	std::string op1_;
	std::string op2_;
	std::string destname_;
	ElementOfMemory* dest;
	void printuj() override;
};

class SetInstruction : public Instruction {
public:
	SetInstruction(const std::string& line, int linenum);
	Instruction* run() override;
	void printuj() override;
private:
	std::string destname_;
	std::string op_;
	ElementOfMemory* dest_;
};

class IfInstruction :public Instruction {
public:
	IfInstruction(const std::string& line, int linenum);
	Instruction* run() override;
	void setNextFalse(Instruction* nextFalse);
	void printuj() override;
private:
	std::string op1_;
	std::string op2_;
	Instruction* nextFalse_;
};

class GotoInstruction :public Instruction {
public:
	GotoInstruction(const std::string& line, int linenum);
	int getNumOfJumps();
	Instruction* run() override { return next_; };
	void printuj() override;
private:
	int numberOfJumps_;
};

class LoopInstruction :public Instruction {
public:
	LoopInstruction(const std::string& line, int linenum);
	Instruction* run() override;
	void setExit(Instruction* exit);
	void printuj()override;
	int getNumberOfLoops();
private:
	int numberOfLoops_;
	int iterator_;
	Instruction* exit_;
};

class instructionWuthoutEffect :public Instruction {
public:
	instructionWuthoutEffect(const std::string& line, int linenum);
	Instruction* run() override { return next_; };
};