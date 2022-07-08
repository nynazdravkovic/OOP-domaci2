#pragma 
#include <exception>
#include <string>
#include <iostream>

//using namespace std;

class ExceptionHandling {
public:
	static int lineNum;
};
class DivisionByZero :public std::exception {
public:
	DivisionByZero() :exception("Division by zero") {};
	virtual const char* what() const throw() {
		return "Division by zero";
	}
};
class gotoIsZero :public std::exception {
public:
	gotoIsZero() :exception("Goto operand is zero") {};
	virtual const char* what() const throw() {
		std::string num = "Goto operand is zero " + std::to_string(ExceptionHandling::lineNum);
		std::cout << num << std::endl;
		//const char* k = num.c_str();
		return num.c_str();
	}
};
class gotoOutOfRange :public std::exception {
public:
	gotoOutOfRange() :exception("Goto operand is too big") {};
	virtual const char* what() const throw() {
		return "Goto operand is too big";
	}
};

class tooManyArguments :public std::exception {
public:
	tooManyArguments() :exception("Too many arguments") {};
	virtual const char* what() const throw() {
		return "Too many arguments";
	}
};
class negativeLoop :public std::exception {
public:
	negativeLoop() :exception("Negative falue for loop") {};
	virtual const char* what() const throw() {
		return "Negative falue for loop";
	}
};
class nonValidFunction :public std::exception {
public:
	nonValidFunction() :exception("Not valid function") {};
	virtual const char* what() const throw() {
		return "Not valid function";
	}
};
class missingIf :public std::exception {
public:
	missingIf() :exception("If block not valid") {};
	virtual const char* what() const throw() {
		return "If block not valid";
	}
};
class missingEnd :public std::exception {
public:
	missingEnd() :exception("Missing END") {};
	virtual const char* what() const throw() {
		return "Missing END";
	}
};
class missingLoop :public std::exception {
public:
	missingLoop() :exception("Missing LOOP") {};
	virtual const char* what() const throw() {
		return "Missing LOOP";
	}
};
class invalidValue :public std::exception {
public:
	invalidValue() :exception("Invalid value") {};
	virtual const char* what() const throw() {
		return "Invalid value";
	}
};
