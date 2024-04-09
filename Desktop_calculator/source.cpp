#include "calculator.h"
#include <sstream>

extern int no_of_errors;
extern Token_stream ts;
extern std::map<std::string, double> table;

void calculate(){
	for(;;){

		while(ts.get().kind != Kind::end){
			if(ts.current().kind == Kind::end) break;
			if(ts.current().kind == Kind::print) continue;
			std::cout << expr(false) << '\n';
		}
	}

}

int main(int argc, char* argv[]){

	switch(argc){
	case 1:
		break;
	case 2:
		ts.set_input(new std::istringstream{argv[1]});
		break;
	default:
		error("too many arguments");
		return 1;
	}


	table["pi"] =  3.1415926535897932385;
	table["e"] = 2.7182818284590452354;

	calculate();

	return no_of_errors; 
}