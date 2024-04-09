#include "calculator.h"

int no_of_errors;
Token_stream ts{std::cin};
std::map<std::string, double> table;

double error(std::string s){
	no_of_errors++;
	std::cerr << "error: " << s << '\n';
	return 1;
}


std::ostream& operator<<(std::ostream& os, Kind& k){
	switch(k){
	case Kind::name:
		std::cout << "name";
		break;
	case Kind::number:
		std::cout << "number";
		break;
	case Kind::end: 
		std::cout << "end";
		break;
	case Kind::plus:
	case Kind::minus:
	case Kind::mul:
	case Kind::div:
	case Kind::print:
	case Kind::assign:
	case Kind::lp:
	case Kind::rp:
		std::cout << static_cast<char>(k);
	}
	return os;
}


Token Token_stream::get(){
	char ch = 0;
	is ->get(ch);

    while(isspace(ch) && ch!='\n') is->get(ch);

	switch(ch){
	case 0:
		return ct = {Kind::end};
	case '+':
	case '-':
	case '*':
	case '/':
	case '(':
	case ')':
	case '=':
		return ct = {static_cast<Kind>(ch)};

	case '\n':
	case ';':
		return ct = {Kind::print};

	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.':
		is->putback(ch);
		*is >> ct.num;
		ct.kind = Kind::number;

		return ct;
	default:
		if(isalpha(ch)){
			ct.s = ch;

			while(is->get(ch) && isalnum(ch))
				ct.s += ch;

			is->putback(ch);
			ct.kind = Kind::name;
			return ct;
		}

		error("bad token");
		return ct = {Kind::print};

	}
}



double expr(bool);

double prim(bool get){

	if(get) ts.get();


	switch(ts.current().kind){
	case Kind::number:
		{
			double v = ts.current().num;
			ts.get();
			return v;
		}
	case Kind::name:
		{
			double& v = table[ts.current().s];
			if(ts.get().kind == Kind::assign) v = expr(true);
			return v;

		}
	case Kind::minus:
		return -prim(true);
	case Kind::lp:
		{
			auto e = expr(true);
			if(ts.current().kind != Kind::rp) return error("')' expected");
			ts.get();
			return e;
		}		
	default:
		return error("primary expected");
	
	}
}


double term(bool get){

	double left = prim(get);

	for(;;){
		switch(ts.current().kind){
		case Kind::mul:
			left *= prim(true);
			break;
		case Kind::div:
			if(double d = prim(true)){
				left /= d;
				break;
			}
			return error("divided by 0");
		default:
			return left;
		}
	}
}


double expr(bool get){
	double left = term(get);

	for(;;){
		switch(ts.current().kind){
		case Kind::plus:
			left += term(true);
			break;
		case Kind::minus:
			left -= term(true);
			break;
		default:
			return left;
		}
	}
}
