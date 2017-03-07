#include <memory>
#include <vector>

#include "lexer.hpp"
#include "ast.hpp"

using TC = Token::Cat;
namespace pb173{

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
			res.m_globals.push_back( global() );
		}
		return res;

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
		if (m_token.m_cat != TC::ParenOpen) {
			//error
		}
		return l;
	}

	Def def() {
		Def d;
		shift();
		d.m_indentifier = identifier();
		shift();
		if (m_token.m_cat != TC::ParenOpen) {
			//error
		}
		d.m_operands.push_back(expression());
		shift();
		if (m_token.m_cat != TC::ParenClose) {
			//error
		}
		d.m_body = expression();
		if (m_token.m_cat != TC::ParenOpen) {
			//error
		}
		return d;
	}

	Identifier identifier() {
		if (m_token.m_cat != TC::Identifier) {
			//error
		}
		return Identifier(m_token.m_text);
	}


	Ptr<Expression> expression() {
		if (m_token.m_cat == TC::LitString) {
			return std::make_shared<Expression>(m_token.m_text);
		}
		if (m_token.m_cat == TC::LitString) {
			return std::make_shared<Expression>(std::stoi(m_token.m_text));
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
				return std::make_shared<Expression>(funcIf());
			}
			if (m_token.m_cat == TC::While) {
				return std::make_shared<Expression>(funcWhile());
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
		
	}

	Call call() {
		Call c;
		c.m_identifier = m_token.m_text;
		shift();
		while (m_token.m_cat == TC::Identifier || m_token.m_cat == TC::LitString 
			|| m_token.m_cat == TC::LitNumber) {
				if (m_token.m_cat == TC::ParenClose) {
					return c;
				}
				c.m_operands.push_back(expression());
				shift();
			}
		//error
	}
	
	App app() {
		App a;
		a.m_operator = m_token.m_text[0];
		shift();
		if (m_token.m_cat == TC::Identifier || m_token.m_cat == TC::LitString 
			|| m_token.m_cat == TC::LitNumber) {
			a.m_operands.push_back(expression());
		}
		shift();
		if (m_token.m_cat == TC::ParenOpen) {
			shift();
			for (auto& o : m_lexer.m_operators) {
				if (m_token.m_text[0] == o.first) {
					a.m_operands.emplace_back
						(std::make_shared<Expression>(app()));
				}
			}
			
		}
		shift();
		if (m_token.m_cat == TC::Identifier || m_token.m_cat == TC::LitString 
			|| m_token.m_cat == TC::LitNumber) {
			a.m_operands.push_back(expression());
		}
		shift();
		if (m_token.m_cat != TC::ParenClose) {
			//error
		}
		return a;
	}
	
	If funcIf() {
		If i;
		shift();
		if (m_token.m_cat != TC::ParenOpen) {
			//error
		}
		shift();
		i.m_condition = expression();
		shift();
		i.m_body = expression();
		shift();
		if (m_token.m_cat != TC::ParenClose) {
			//error
		}
		return i;
	}

	While funcWhile() {
		While i;
		shift();
		if (m_token.m_cat != TC::ParenOpen) {
			//error
		}
		shift();
		i.m_condition = expression();
		shift();
		i.m_body = expression();
		shift();
		if (m_token.m_cat != TC::ParenClose) {
			//error
		}
		return i;
	}
};

}
