#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <iostream>
#include <map>
#include <cctype>


enum class Kind : char {
	name, number, end,
	plus = '+', minus = '-', mul = '*', div = '/', print = ';', assign = '=', lp = '(', rp = ')'
};

std::ostream& operator<<(std::ostream& , Kind&);


struct Token{
	Kind kind;
	std::string s;
	double num;
};



class Token_stream{
private:
	std::istream* is;
	bool owns;
	Token ct{Kind::end};

	void close(){
		if(owns) delete is;
	}
public:
	Token_stream(std::istream& s): is(&s), owns{false} {}
	Token_stream(std::istream* p): is(p), owns{true} {}

	~Token_stream(){close();}

	Token get();
	Token& current() {return ct;};

	void set_input(std::istream& s){
		close();
		is = &s;
		owns = false;
	}

	void set_input(std::istream* p){
		close();
		is = p;
		owns = true;
	}
};

double error(std::string);
double expr(bool);
double prim(bool);
double term(bool);

#endif