#include "parser.hpp"

struct SymbolTable;
struct TopLevel;

struct Interpreter {
	SymbolTable symTable;
	Toplevel toplevel;


	Interpreter(const char* c)  {
		Parser parser(c);
		toplevel = parser.toplevel();
        buildScope(symTable, toplevel);
	}
};

