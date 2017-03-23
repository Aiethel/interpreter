#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "interpreter.hpp"

using TC = Token::Cat;

TEST_CASE("Testing testing") {
	CHECK(1 == 1);
}

TEST_CASE("Lexer Hello World") {
	Lexer lexer("test/testdata1.txt");
	SECTION ("Text Values") {
		CHECK(lexer.next().m_text == "Hello");
		CHECK(lexer.next().m_text == "World");
	}
	SECTION ("Categories") {
		CHECK(lexer.next().m_cat == Token::Cat::Identifier);
		CHECK(lexer.next().m_cat == Token::Cat::Identifier);
		CHECK(lexer.next().m_cat == Token::Cat::Eof);
	}

}


TEST_CASE("Lexer keyword") {
	Lexer lexer("test/testdata2.txt");
	SECTION ("Text Values") {
		CHECK(lexer.next().m_text == "Hello");
		CHECK(lexer.next().m_text == "if");
		CHECK(lexer.next().m_text == "+");
		CHECK(lexer.next().m_text == "-");
		CHECK(lexer.next().m_text == "else");
		CHECK(lexer.next().m_text == "world");
	}
	SECTION ("Categories") {
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::If);
		CHECK(lexer.next().m_cat == TC::Plus);
		CHECK(lexer.next().m_cat == TC::Minus);
		CHECK(lexer.next().m_cat == TC::Else);
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::Eof);
	}
}

TEST_CASE("Lexer small program") {
	Lexer lexer("test/testdata3.txt");
	SECTION ("Text Values"){
		CHECK(lexer.next().m_text == "h");
		CHECK(lexer.next().m_text == "=");
		CHECK(lexer.next().m_text == "Hello");
		CHECK(lexer.next().m_text == "w");
		CHECK(lexer.next().m_text == "=");
		CHECK(lexer.next().m_text == "234");
		CHECK(lexer.next().m_text == "if");
		CHECK(lexer.next().m_text == "(");
		CHECK(lexer.next().m_text == "h");
		CHECK(lexer.next().m_text == ")");
		CHECK(lexer.next().m_text == "w");
	}
	SECTION ("Categories") {
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::Equals);
		CHECK(lexer.next().m_cat == TC::LitString);
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::Equals);
		CHECK(lexer.next().m_cat == TC::LitNumber);
		CHECK(lexer.next().m_cat == TC::If);
		CHECK(lexer.next().m_cat == TC::ParenOpen);
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::ParenClose);
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::Eof);
		
	}
}

//Program was modyfied during testing of lexer
#if 1
TEST_CASE("Program") {
	Lexer lexer("test/data.txt");
	SECTION("Text Values") {
		CHECK(lexer.next().m_text == "(");
		CHECK(lexer.next().m_text == "let");
		CHECK(lexer.next().m_text == "a");
		CHECK(lexer.next().m_text == "string");
		CHECK(lexer.next().m_text == ")");
		CHECK(lexer.next().m_text == "(");
		CHECK(lexer.next().m_text == "def");
		CHECK(lexer.next().m_text == "foo");
		CHECK(lexer.next().m_text == "(");
		CHECK(lexer.next().m_text == "a");
		CHECK(lexer.next().m_text == ")");
		CHECK(lexer.next().m_text == "(");
		CHECK(lexer.next().m_text == "let");
		CHECK(lexer.next().m_text == "g");
		CHECK(lexer.next().m_text == "(");
		CHECK(lexer.next().m_text == "+");
		CHECK(lexer.next().m_text == "2");
		CHECK(lexer.next().m_text == "3");
		CHECK(lexer.next().m_text == ")");
		CHECK(lexer.next().m_text == ")");
	}
	SECTION("Categories") {
		CHECK(lexer.next().m_cat == TC::ParenOpen);
		CHECK(lexer.next().m_cat == TC::Let);
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::LitString);
		CHECK(lexer.next().m_cat == TC::ParenClose);
		CHECK(lexer.next().m_cat == TC::ParenOpen);
		CHECK(lexer.next().m_cat == TC::Def);
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::ParenOpen);
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::ParenClose);
		CHECK(lexer.next().m_cat == TC::ParenOpen);
		CHECK(lexer.next().m_cat == TC::Let);
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::ParenOpen);
		CHECK(lexer.next().m_cat == TC::Plus);
		CHECK(lexer.next().m_cat == TC::LitNumber);
		CHECK(lexer.next().m_cat == TC::LitNumber);
		CHECK(lexer.next().m_cat == TC::ParenClose);
		CHECK(lexer.next().m_cat == TC::ParenClose);
	}
	SECTION("Line numbers") {
		CHECK(lexer.next().m_line == 1);
		CHECK(lexer.next().m_line == 1);
		CHECK(lexer.next().m_line == 1);
		CHECK(lexer.next().m_line == 1);
		CHECK(lexer.next().m_line == 1);
		CHECK(lexer.next().m_line == 2);
		CHECK(lexer.next().m_line == 2);
		CHECK(lexer.next().m_line == 2);
		CHECK(lexer.next().m_line == 2);
		CHECK(lexer.next().m_line == 2);
		CHECK(lexer.next().m_line == 2);
		CHECK(lexer.next().m_line == 3);
		CHECK(lexer.next().m_line == 3);
		CHECK(lexer.next().m_line == 3);
		CHECK(lexer.next().m_line == 3);
		CHECK(lexer.next().m_line == 3);
		CHECK(lexer.next().m_line == 3);
		CHECK(lexer.next().m_line == 3);
		CHECK(lexer.next().m_line == 3);
		CHECK(lexer.next().m_line == 3);
	}

}
#endif

TEST_CASE("Scoping") {
    CHECK_THROWS_AS(Interpreter("test/scope.txt"), ScopeError);
    try {
        Interpreter("test/scope.txt");
    } catch (ScopeError& err) {
        CHECK(err.text == "Variable not define yet used 5");
    }
}

TEST_CASE("Redefine") {
    REQUIRE_THROWS_AS(Interpreter("test/scope1.txt"), ScopeError);
    try {
        Interpreter("test/scope1.txt");
    } catch (ScopeError& err) {
        CHECK(err.text == "Variable already defined in scope 5");
    }
}

TEST_CASE("Function Names") {
    REQUIRE_THROWS_AS(Interpreter("test/scope2.txt"), ScopeError);
    try {
        Interpreter("test/scope2.txt");
    } catch (ScopeError& err) {
        CHECK(err.text == "Variable already defined in scope 0");
    }
}

TEST_CASE("SymbolTable") {
    Interpreter interpreter("test/data.txt");

    auto& scope = interpreter.toplevel.m_scope;
    auto& st = interpreter.symTable;

    CHECK(scope.m_children.size() == 2);
    CHECK(scope.m_identifiers.size() == 3);
    CHECK(scope.m_identifiers.find(st.get("foo")) != scope.m_identifiers.end());
    CHECK(scope.m_identifiers.find(st.get("func")) != scope.m_identifiers.end());
    CHECK(scope.m_identifiers.find(st.get("a")) != scope.m_identifiers.end());

    auto& curr = scope.m_children[0];
    CHECK(curr.m_children.size() == 0);
    CHECK(curr.m_identifiers.size() == 3);
    CHECK(curr.m_identifiers.find(st.get("g")) != curr.m_identifiers.end());
    CHECK(curr.m_identifiers.find(st.get("r")) != curr.m_identifiers.end());
    CHECK(curr.m_identifiers.find(st.get("a")) != curr.m_identifiers.end());


    curr = scope.m_children[1];
    CHECK(curr.m_children.size() == 1);
    CHECK(curr.m_identifiers.size() == 4);
    CHECK(curr.m_identifiers.find(st.get("b")) != curr.m_identifiers.end());
    CHECK(curr.m_identifiers.find(st.get("c")) != curr.m_identifiers.end());
    CHECK(curr.m_identifiers.find(st.get("e")) != curr.m_identifiers.end());
    CHECK(curr.m_identifiers.find(st.get("q")) != curr.m_identifiers.end());

    curr = curr.m_children[0];
    CHECK(curr.m_children.size() == 0);
    CHECK(curr.m_identifiers.size() == 1);
    CHECK(curr.m_identifiers.find(st.get("ff")) != curr.m_identifiers.end());
}

TEST_CASE("Output") {
    Interpreter interpreter("test/data.txt");
    std::cout << interpreter.toplevel;
    std::cout << interpreter.symTable;
    std::cout << "****" << std::endl << interpreter.toplevel.m_scope << std::endl;
}
