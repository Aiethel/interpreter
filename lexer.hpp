#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <cctype>

struct Token {
	enum class Cat {
		ParenOpen, ParenClose, Plus, Minus, Times, Div, Equals, LitString, LitNumber,
		Identifier, If, Else, Endif, While, Error, Eof
	} m_cat;

	std::string m_text;
	int m_line;

	Token(Cat cat, std::string& text, int line)
		: m_cat(cat), m_text(text), m_line(line) {
		//Empty
	};
};


struct Lexer {
	std::map <char, Token::Cat> m_operators = {
		{'+', Token::Cat::Plus},
		{'-', Token::Cat::Minus},
		{'*', Token::Cat::Times},
		{'/', Token::Cat::Div},
		{'(', Token::Cat::ParenOpen},
		{')', Token::Cat::ParenClose},
		{'=', Token::Cat::Equals},
	};
	
	std::map <std::string, Token::Cat> m_keywords = {
		{"if", Token::Cat::If},
		{"else", Token::Cat::Else},
		{"endif", Token::Cat::Endif},
		{"while", Token::Cat::While},
	};

	std::ifstream m_input;
	std::string m_buf;
	char c;
	int m_line = 1;

	Lexer (const std::string& file) : m_input(file) {
		
	}

	Token next() {
		whitespace();
		m_buf += (c = m_input.get());
		for (auto& a: m_operators) {
			if (a.first == c) {
				return shift(a.second);
			}
		}
		
		if (c == '"') {
			m_buf.pop_back();
			return stringLiteral();
		}

		if (std::isdigit(c)) {
			return numberLiteral();
		}
		if (std::isalpha(c)) {
			return identifier();
		}
		return shift(Token::Cat::Eof);
	}



private:

	Token identifier() {
		while (std::isalnum( c = m_input.get())) {
			m_buf += c;
			for (auto& a : m_keywords) {
				if (a.first == m_buf) {
					return shift(a.second);
				}
			}
		}
		return shift(Token::Cat::Identifier);
		m_input.unget();
	}

	Token numberLiteral() {
		bool error = false;
		while (!std::isspace(c = m_input.get())) {
			if (!error && !std::isdigit(c)) {
				error = true;
			}
			m_buf += c;
		}
		if (error) {
			return shift(Token::Cat::Error);
		}
		return shift(Token::Cat::LitNumber);
	}

	Token stringLiteral() {
		while ((c = m_input.get()) != '"') {
			m_buf += c;
		}
		return shift(Token::Cat::LitString);
	}

	Token shift(Token::Cat c) {
		Token token(c, m_buf, m_line);
		m_buf.clear();
		return token;
	}
	void whitespace() {
		while (std::isspace ( c = m_input.get())) {
			if (c == '\n') {
				++m_line;
			}
		}
		m_input.unget();
	}
};
