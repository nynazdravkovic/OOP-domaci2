#include "instruction.h"
#include "machine.h"
#include "Exceptions.h"

using namespace std;

Instruction::Instruction(){
	this->next_ = nullptr;
	marker_ = 0;
}
//funkcija nalazi rec u stringu pocev od string[start]
std::string Instruction::findWord(const std::string& line, int& start){
	int i = 0;
	while (start + i < line.size() && line[start+i]!=' ') {
		i++;
	}
	i++; //preskacem razmak ili sledeci red
	start += i;
	return line.substr(start-i, i-1);
}
//funkcija nalazi broj razmaka u liniji
int Instruction::numOfSpaces(const string& line) {
	int cnt = 0;
	int i = 0;
	while (i < line.size()) {
		if (line[i] == ' ') cnt++;
		i++;
	}
	return cnt;
}
int Instruction::getLineNum(){
	return lineNum_;
}
//vraca vrednost promenljive u memoriji ili cita konstantu
int Instruction::getValueFromMem(const std::string& name){
	if (Machine::mem->findVariable(name)) {
		return Machine::mem->findVariable(name)->getValue();
	}
	else {//to znaci da je const promenljiva ako nije greska koja treba da se hvata u try catchu
		try {
			return stoi(name);
			//ako stoi ne prodje ili je unet neki skroz nevalidan karakter ili korisnik trazi promenljivu koja nije u memoriji
		}
		catch (...) {
			cout << "Input is not valid. Please enter a number or check which operands are avalible"<<endl;
		}
	}
}

void Instruction::setValueInMem(ElementOfMemory* elem, int newVal){
	elem->setValue(newVal);
}

std::string Instruction::getNameOfIstruction() {
	return nameOfInst_;
}

char Instruction::getMarker(){
	return marker_;
	//marker mi sluzi da kada prolazim kroz instrukcije ne moram da proveravam da li je ceo string nameofinst jednak nekom stringu nego samo uporedim char-ove
}

void Instruction::setNext(Instruction* next){
	next_ = next;
}

//printuj funkcija sluzi za debagovanje
void Instruction::printuj(){
	if(next_ != nullptr) cout << next_->marker_<<endl;
}

SetInstruction::SetInstruction(const std::string& line, int linenum) {
	int i = 0;
	marker_ = 's';
	nameOfInst_ = findWord(line, i);
	destname_ = findWord(line, i);
	op_ = findWord(line, i);
	if (numOfSpaces(line) != 2)	throw new tooManyArguments();
	lineNum_ = linenum;
}

Instruction* SetInstruction::run() {
	//trazim vrednost u memoriji, ako je nema vracam nullptr
	dest_ = Machine::mem->findVariable(destname_);
	if (dest_) dest_->setValue(getValueFromMem(op_)); 
	//setovana je nova vrednost u memoriji ili je nova memorija zauzeta i formirana je nova promenljiva
	else {
		ElementOfMemory* novi = new ElementOfMemory(destname_, getValueFromMem(op_));
		Machine::mem->addVariable(novi);
	}
	return next_;
}

void SetInstruction::printuj() {
	cout << "Set next:" << next_->getMarker() << endl;
}

ArithmeticInstruction::ArithmeticInstruction(const std::string& line, int linenum) {
	int i = 0;
	marker_ = 'a';
	//prvo nadjem ime instrukcije 
	nameOfInst_ = findWord(line, i);
	//onda nalazim operande
	destname_ = findWord(line, i);
	op1_ = findWord(line, i);
	op2_ = findWord(line, i);
	if (numOfSpaces(line) != 3) throw(new tooManyArguments());
	lineNum_ = linenum;
}

Instruction* ArithmeticInstruction::run(){
	dest = Machine::mem->findVariable(destname_);
	//trazim vrednosti operanada
	int op1 = getValueFromMem(op1_);
	int op2 = getValueFromMem(op2_);
	if (nameOfInst_ == "ADD") {
		setValueInMem(dest,op1 + op2);
	}
	else if (nameOfInst_ == "SUB") {
		setValueInMem(dest, op1 - op2);
	}
	else if (nameOfInst_ == "MUL") {
		setValueInMem(dest, op1 * op2);
	}
	else if (nameOfInst_ == "DIV") {
		 if(op2!=0)setValueInMem(dest, op1 / op2);
		 else  throw(new DivisionByZero()); 
	}
	return next_;
}

void ArithmeticInstruction::printuj(){
	cout << "Arithnmetic next:" << next_->getMarker() << endl;
}

IfInstruction::IfInstruction(const std::string& line, int linenum){
	lineNum_ = linenum;
	int i = 0;
	marker_ = 'i';
	nameOfInst_ = findWord(line, i);
	op1_= findWord(line, i);
	op2_= findWord(line, i);
	if (numOfSpaces(line) != 2) throw(new tooManyArguments());

}

Instruction* IfInstruction::run(){
	int op1 = getValueFromMem(op1_);
	int op2 = getValueFromMem(op2_);
	if (nameOfInst_ == "IFGR") {
		if (op1 > op2) {
			return next_; 
		}
		else {
			return nextFalse_;
		}
	}
	else if (nameOfInst_ == "IFEQ") {
		if (op1 == op2) {
			return next_;
		}
		else {
			return nextFalse_;
		}
	}
}

void IfInstruction::setNextFalse(Instruction* nextFalse){
	nextFalse_ = nextFalse;
}

void IfInstruction::printuj(){
	cout << next_->getMarker() << " levo je true, desno je false "<< nextFalse_->getMarker()<<endl;
}

GotoInstruction::GotoInstruction(const std::string& line, int linenum){
	int i = 0;
	lineNum_ = linenum;
	marker_ = 'g';
	nameOfInst_ = findWord(line,i);
	try {
		numberOfJumps_ = stoi(findWord(line, i));
		//ako stoi ne prodje unet je neki nevalidan karakter
	}
	catch (...) {
		cout << "Input is not valid. Please enter a number." << endl;
	}
	if (numberOfJumps_==0)	throw(new gotoIsZero());
	if (numOfSpaces(line) != 1) throw(new tooManyArguments());
}

int GotoInstruction::getNumOfJumps(){
	return numberOfJumps_;
}

void GotoInstruction::printuj(){
	std::cout << "goto sledeci: " << next_->getMarker() << std::endl;
}

LoopInstruction::LoopInstruction(const std::string& line, int linenum){
	int i = 0; 
	lineNum_ = linenum;
	marker_ = 'l';
	nameOfInst_ = findWord(line, i);
	if (line.length() == 4) {
		numberOfLoops_ = -1;
	}
	else {
		try {
			numberOfLoops_ = stoi(findWord(line, i));
			//ako stoi ne prodje unet je neki nevalidan karakter
		}
		catch (...) {
			cout << "Input is not valid. Please enter a number." << endl;
		}
		if (numberOfLoops_ < 0) throw(new negativeLoop());
		if (numOfSpaces(line) != 1) throw(new tooManyArguments());
	}
	iterator_ = -1;
}

Instruction* LoopInstruction::run() {
	iterator_++; 
	if (numberOfLoops_ == -1) {
		return next_;
	}
	else if (iterator_ < numberOfLoops_) return next_;
	else {
		iterator_ = -1;
		return exit_;
	}
}

void LoopInstruction::setExit(Instruction* exit) {
	exit_ = exit;
}

void LoopInstruction::printuj(){
	std::cout << next_->getMarker() << " levo je sledeci, desno je exit " << exit_->getMarker() << std::endl;
}

int LoopInstruction::getNumberOfLoops(){
	return numberOfLoops_;
}

instructionWuthoutEffect::instructionWuthoutEffect(const std::string& line, int linenum){
	lineNum_ = linenum;
	int i = 0;
	marker_ = 'w';
	nameOfInst_ = findWord(line, i);
	if (numOfSpaces(line) != 0)throw(new tooManyArguments());
}
