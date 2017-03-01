#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "lexer.hpp"

using N = Token::Cat;

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
		CHECK(lexer.next().m_cat == N::Identifier);
		CHECK(lexer.next().m_cat == N::If);
		CHECK(lexer.next().m_cat == N::Plus);
		CHECK(lexer.next().m_cat == N::Minus);
		CHECK(lexer.next().m_cat == N::Else);
		CHECK(lexer.next().m_cat == N::Identifier);
		CHECK(lexer.next().m_cat == N::Eof);
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
		CHECK(lexer.next().m_cat == N::Identifier);
		CHECK(lexer.next().m_cat == N::Equals);
		CHECK(lexer.next().m_cat == N::LitString);
		CHECK(lexer.next().m_cat == N::Identifier);
		CHECK(lexer.next().m_cat == N::Equals);
		CHECK(lexer.next().m_cat == N::LitNumber);
		CHECK(lexer.next().m_cat == N::If);
		CHECK(lexer.next().m_cat == N::ParenOpen);
		CHECK(lexer.next().m_cat == N::Identifier);
		CHECK(lexer.next().m_cat == N::ParenClose);
		CHECK(lexer.next().m_cat == N::Identifier);
		CHECK(lexer.next().m_cat == N::Eof);
		
	}
}
