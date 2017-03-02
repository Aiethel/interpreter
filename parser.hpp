#include <memory>

#include "lexer.hpp"
#include "ast.hpp"

using TC = Token::Cat;

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
	
	Toplevel toplevel() {
		Toplevel res;
		while(true) {
			shift();
			if (m_token.m_cat == TC::Eof) {
				return res;
			}
			result.globals.push_back( global() );
		}

	}

	void parenOpen() {
	}

	void parenClose() {

	}

	Global global() {
		if (m_token.m_cat != TC::ParenOpen) {
			//error
		}
		shift();
		if (m_token.m_cat == TC::Let) {
			return let();
		}
		if (m_token.m_cat == TC::Def) {	
			return def();	
		}
		//error
	}
	
	Let let() {
		Let l;
		shift();
		l.m_identifier = identifier();
		shift();
		l.m_body = expression();	
	}

	Def def() {
		Def f;
		shift();
		d.m_indentifier = identifier();
		shift();
		if (m_token.m_cat != TC::ParenOpen) {
			//error
		}
		d.m_operands = expression();
		shift();
		if (m_token.m_cat != TC:ParenClose) {
			//error
		}
		d.m_body = expression();
	}

	Identifier identifier() {
		if (m_token != TC::Identifier) {
			//error
		}
		return Identifier(token.m_text);
	}


	Ptr<Expression> expression() {
		if (m_token.m_cat == TC::LitString) {
			return std::make_shared<std::string>(m_token.m_text);
		}
		if (m_token.m_cat == TC::LitString) {
			return std::make_shared<int>(std::stoi(m_token.m_text));
		}
		if (m_token.m_cat == TC:Identifier) {
			return std::make_shared<Identifier>(identifier());
		}
		if (m_token.m_cat == TC::OpenParen) {
			shift();
			if (m_token.m_cat == TC::Identifier) {
				return std::make_shared<Call>(call());
			}
			if (m_token.m_cat == TC::If) {
				return std::make_shared<If>(funcIf());
			}
			if (m_token.m_cat == TC::While) {
				return std::make_shared<While>(funcWhile());
			}
			for (auto& a : m_lexer.m_operators) {
				if (m_token.m_cat == a) {
					return std::make_shared<App>(app());
				}
			}
		}
		
	}

	Call call() {

	}
	
	App app() {

	}
	
	If funcIf() {

	}

	While funcWhile() {

	}
}
