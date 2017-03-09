#include <memory>
#include <vector>
#include <iostream>

#include "lexer.hpp"
#include "ast.hpp"

using TC = Token::Cat;

struct BadParse {
	Token m_found;
	std::string m_expected;
	BadParse(Token t, const std::string& e) : m_found(t), m_expected(e) {}
};

static inline std::ostream& operator<<(std::ostream& o, BadParse& bp) {
	return o << "Parse error at " << bp.m_found.m_text << 
		", at "<< bp.m_found.m_line << ", expected: " << bp.m_expected << std::endl;
}

struct Parser {
	Lexer m_lexer;
	Token m_token;

	Parser( const char* file) : m_lexer(file) {
		//Empty
	}

	template<typename T>
	Ptr <Expression> make_expr(T e) {
		std::make_shared<Expression> (e);
	}

	void shift() {
		m_token = m_lexer.next();
	}
	
	void fail(const std::string& str) {
		BadParse bp(m_token, str);
		std::cout << bp << std::endl;
		throw(bp);
	}

	Toplevel toplevel() {
		Toplevel res;
		while(true) {
			shift();
			if (m_token.m_cat == TC::Eof) {
				return res;
			}
			res.m_globals.push_back( global() );
		}
		return res;

	}


	Global global() {
		if (m_token.m_cat != TC::ParenOpen) {
			fail("OpenParen");
		}
		shift();
		if (m_token.m_cat == TC::Let) {
			return let();
		}
		if (m_token.m_cat == TC::Def) {	
			return def();	
		}
		fail("Let or Def");
	}
	
	Let let() {
		Let l;
		shift();
		l.m_identifier = identifier();
		l.m_body = expression();
		if (m_token.m_cat != TC::ParenClose) {
			fail("ParenOpen");
		}
		return l;
	}

	Def def() {
		Def d;
		shift();
		d.m_identifier = identifier();
		if (m_token.m_cat != TC::ParenOpen) {
			fail("ParenOpen");
		}

		shift();
		while (m_token.m_cat != TC::ParenClose) {
			d.m_operands.push_back(expression());
		}

		shift();
		while (m_token.m_cat != TC::ParenClose) {
			d.m_body.push_back(expression());
		}

        shift();
		return d;
	}

	Identifier identifier() {
		if (m_token.m_cat != TC::Identifier) {
			fail("identifier");
		}
		auto i = Identifier(m_token.m_text);
		shift();
		return i;
	}

	int litNumber() {
		auto i = std::stoi(m_token.m_text);
		shift();
		return i;
	}

	std::string litString() {
		auto i = m_token.m_text;
		shift();
		return i;
	}

	Ptr<Expression> expression() {
		if (m_token.m_cat == TC::LitString) {
			return std::make_shared<Expression>(litString());
		}
		if (m_token.m_cat == TC::LitNumber) {
			return std::make_shared<Expression>(litNumber());
		}
		if (m_token.m_cat == TC::Identifier) {
			return std::make_shared<Expression>(identifier());
		}
		if (m_token.m_cat == TC::ParenOpen) {
			shift();
			if (m_token.m_cat == TC::Identifier) {
				return std::make_shared<Expression>(call());
			}
			if (m_token.m_cat == TC::If) {
				return std::make_shared<Expression>(cycle<If>());
			}
			if (m_token.m_cat == TC::While) {
				return std::make_shared<Expression>(cycle<While>());
			}
			if (m_token.m_cat == TC::Let) {
				return std::make_shared<Expression>(let());
			}
			for (auto& a : m_lexer.m_operators) {
				if (m_token.m_text[0] == a.first) {
					return std::make_shared<Expression>(app());
				}
			}
		}
		fail("Unknown expression");
	}

	Call call() {
		Call c;
		c.m_identifier = Identifier(m_token.m_text);
		shift();
		while (m_token.m_cat != TC::ParenClose) {
			c.m_operands.push_back(expression());
		}
		shift();
		return c;
	}
	
	App app() {
		App a;
		a.m_operator = m_token.m_text[0];
		shift();
		while(m_token.m_cat != TC::ParenClose) {
			a.m_operands.push_back(expression());
		}

		shift();
		return a;
	}


	template<typename T>
	T cycle() {
		T i;
		shift();
		if (m_token.m_cat != TC::ParenOpen) {
			fail("ParenOpen");
		}
		i.m_condition = expression();

		while (m_token.m_cat == TC::ParenOpen) {
			i.m_body.push_back(expression());
		}
		shift();
		return i;
	}

	If funcIf() {
		If i;
		shift();
		if (m_token.m_cat != TC::ParenOpen) {
			fail("ParenOpen");
		}
		i.m_condition = expression();

		while (m_token.m_cat == TC::ParenOpen) {
			i.m_body.push_back(expression());
		}
		shift();
		return i;
	}

	While funcWhile() {
		While i;
		shift();
		if (m_token.m_cat != TC::ParenOpen) {
			fail("ParenOpen");
		}
		shift();
		i.m_condition = expression();
		shift();
		//i.m_body = expression();
		shift();
		if (m_token.m_cat != TC::ParenClose) {
			fail("ParenClose");
		}
		return i;
	}
};
