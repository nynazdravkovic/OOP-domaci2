#include "machine.h"
using namespace std;


Memory* Machine::mem = nullptr;

Machine::Machine() {
	Machine::mem = new Memory();
}
//funkcija ucitava fajl i pravi vektror u kome su odgovarajuci objekti instrukcija
void Machine::loadProgram(const std::string& filepath) {
	ifstream input(filepath);
	string line;
	string nameOfInstr;
	try {
		while (getline(input, line)) {
			ExceptionHandling::lineNum++;
			int i = 0;
			nameOfInstr = Instruction::findWord(line, i);
			if (nameOfInstr == "SET") {
				instructions.push_back(new SetInstruction(line,ExceptionHandling::lineNum));
			}
			else if (nameOfInstr == "ADD" || nameOfInstr == "SUB" || nameOfInstr == "MUL" || nameOfInstr == "DIV") {
				instructions.push_back(new ArithmeticInstruction(line, ExceptionHandling::lineNum));
			}
			else if (nameOfInstr == "GOTO") {
				instructions.push_back(new GotoInstruction(line, ExceptionHandling::lineNum));
			}
			else if (nameOfInstr == "IFEQ" || nameOfInstr == "IFGR") {
				instructions.push_back(new IfInstruction(line, ExceptionHandling::lineNum));
			}
			else if (nameOfInstr == "LOOP") {
				instructions.push_back(new LoopInstruction(line, ExceptionHandling::lineNum));
			}
			else if (nameOfInstr == "ENDIF" || nameOfInstr == "ELSE" || nameOfInstr == "ENDLOOP") {
				instructions.push_back(new instructionWuthoutEffect(line, ExceptionHandling::lineNum));
			}
			else {
				throw new nonValidFunction();
			}
		}
		instructions.push_back(nullptr);
		setNextInstructions();
		input.close();
	}
	catch (exception* e) {
		cout << e->what() << endl;
		instructions.clear();
	}
	catch (invalid_argument& e) {
		cout << e.what();
		cout << "That is not a valid number." << '\n';
	}
}
//ovde povezujem istrukcije
void Machine::setNextInstructions() {
	int i = 0;
	while (i + 1 < instructions.size()) {
		if (instructions[i]->getMarker() == 's') {
			instructions[i]->setNext(instructions[i + 1]);
		}
		if (instructions[i]->getMarker() == 'a') {
			instructions[i]->setNext(instructions[i + 1]);
		}
		if (instructions[i]->getMarker() == 'g') {
			GotoInstruction* temp = (GotoInstruction*)instructions[i];
			if (i + temp->getNumOfJumps() == instructions.size() - 1) {
				instructions[i]->setNext(instructions[instructions.size() - 1]);
				//poseban slucaj kada ga salje na poslednji
			}
			else if (i + temp->getNumOfJumps() >= instructions.size()) {
				//poseban slucaj kada ga salje negde van vektora
				//instructions[i]->setNext(instructions[instructions.size() - 1]);
				throw new gotoOutOfRange();
			}
			//next goto-a je poseban:
			else instructions[i]->setNext(instructions[i + temp->getNumOfJumps()]);
		}
		if (instructions[i]->getMarker() == 'i') {
			//zbog steka ne moram da brinem o ugnjezdenim ifovima
			stackIf.push(instructions[i]);
			//ovo je ako je ispunjen uslov ifa
			instructions[i]->setNext(instructions[i + 1]);
		}
		if (instructions[i]->getNameOfIstruction() == "ELSE") {
			//cuvam poslednju instrukciju koja se izvrsava u bloku kada je if tacan da bi mogla kasnije njoj da postavim next na endif
			beaforeElse.push(instructions[i - 1]);
			stackIf.push(instructions[i]);
			instructions[i]->setNext(instructions[i + 1]);
		}
		if (instructions[i]->getNameOfIstruction() == "ENDIF") {
			if (stackIf.size() == 1 || stackIf.size() == 0) throw new missingIf();//nedostaju ili i if i else ili samo jedan od njih
			Instruction* instrBeaforeElse = beaforeElse.top();
			instructionWuthoutEffect* elseInstr = (instructionWuthoutEffect*)stackIf.top();
			stackIf.pop();
			IfInstruction* ifInstruction = (IfInstruction*)stackIf.top();
			stackIf.pop();
			//prepovezujem sve:
			if (instrBeaforeElse->getMarker() != 'g') instrBeaforeElse->setNext(instructions[i]);
			ifInstruction->setNextFalse(elseInstr);
			instructions[i]->setNext(instructions[i + 1]);
		}
		if (instructions[i]->getMarker() == 'l') {
			instructions[i]->setNext(instructions[i + 1]);
			stackLoop.push(instructions[i]);
			//zvog setka ne moram da brinem o ugnjezdenim petljama
		}
		if (instructions[i]->getNameOfIstruction() == "ENDLOOP") {
			if (stackLoop.size() == 0) throw(new missingLoop());
			LoopInstruction* loopStart = (LoopInstruction*)stackLoop.top();
			stackLoop.pop();
			instructions[i]->setNext(loopStart);
			if (loopStart->getNumberOfLoops() >= 0) loopStart->setExit(instructions[i + 1]);
		}
		i++;
	}
	if (stackIf.size() != 0 || stackLoop.size() != 0) throw(new missingEnd());
}

Machine::~Machine() {
	delete mem;
	int i = 0;
	while (i < instructions.size()) {
		delete instructions[i];
		i++;
	}
}

void Machine::execute(const std::string& filepath) {
	if (instructions.size() == 0)
		return;
	int i = 0;
	Instruction* curr = instructions[0];
	while (curr) {
		try {
			//cout << curr->getMarker() << endl;
			//system("pause");
			curr = curr->run();
		}
		catch (exception* e) {
			cout << e->what() << endl;
			return;
		}
	}
	ofstream output;
	mem->sortMemByName();
	ElementOfMemory* temp;
	output.open(filepath);
	while (i < mem->getSize()) {
		temp = mem->getElem(i);
		if (i == mem->getSize() - 1)output << temp->getName() << "=" << temp->getValue();
		else output << temp->getName() << "=" << temp->getValue() << "\n";
		i++;
	}
	output.close();
}