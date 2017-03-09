#include "ast.hpp"
#include "scope.hpp"

struct Interpreter {
	SymbolTable symTable;
	Toplevel toplevel;

	Interpreter(Toplevel tl) : toplevel(tl) {
		//Empty
	}
}
