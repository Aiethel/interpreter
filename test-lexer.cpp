#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "parser.hpp"

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
#if 0
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
		CHECK(lexer.next().m_text == "func");
		CHECK(lexer.next().m_text == "(");
		CHECK(lexer.next().m_text == "b");
		CHECK(lexer.next().m_text == ")");
		CHECK(lexer.next().m_text == "(");
		CHECK(lexer.next().m_text == "+");
		CHECK(lexer.next().m_text == "a");
		CHECK(lexer.next().m_text == "b");
		CHECK(lexer.next().m_text == ")");
		CHECK(lexer.next().m_text == "(");
		CHECK(lexer.next().m_text == "*");
		CHECK(lexer.next().m_text == "a");
		CHECK(lexer.next().m_text == "45");
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
		CHECK(lexer.next().m_cat == TC::Plus);
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::Identifier);
		CHECK(lexer.next().m_cat == TC::ParenClose);
		CHECK(lexer.next().m_cat == TC::ParenOpen);
		CHECK(lexer.next().m_cat == TC::Times);
		CHECK(lexer.next().m_cat == TC::Identifier);
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
		CHECK(lexer.next().m_line == 4);
		CHECK(lexer.next().m_line == 4);
		CHECK(lexer.next().m_line == 4);
		CHECK(lexer.next().m_line == 4);
		CHECK(lexer.next().m_line == 4);
		CHECK(lexer.next().m_line == 5);

	}

}
#endif

TEST_CASE("Parser") {
	Parser parser("test/data.txt");
	auto a = parser.toplevel();
	std::cout << "Code" << std::endl;
	std::cout << a << std::endl;
}
