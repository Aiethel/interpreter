#pragma once

#include <string>

#include "parser.hpp"

struct SymbolTable;
struct TopLevel;

struct Interpreter {
	SymbolTable symTable;
	Toplevel toplevel;
	std::string result;


	Interpreter(const char* c)  {
		Parser parser(c);
		toplevel = parser.toplevel();
        buildScope(symTable, toplevel);
	}

	int run() {
		auto x = eval(symTable, toplevel);
        int res = 0;
        x.match(
                [&](int i) {res = i;}
        );
        return res;
	}
};

