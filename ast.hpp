#include <memory>
#include <vector>
#include "bricks/brick-types"

using brick::types::Union;

template<typename T>
using Ptr = std::shared_ptr<T>;

struct Expression;
struct Identifier {
	std::string m_name;
	Identifier& operator=(const Identifier& identifier) = default;
	explicit Identifier(const std::string& str) : m_name(str) {};
	Identifier() = default;
};

using Constant = Union<int, std::string>;

//keyword + ( condition )
struct IfLike
{
	Ptr<Expression> m_condition;
	std::vector<Ptr<Expression>> m_body;
};

struct If : IfLike {};
struct While : IfLike {};


struct List {
	std::vector<Ptr<Expression>> m_operands;
};


struct Let {
	Identifier m_identifier;
	Ptr<Expression> m_body;
};


struct Def : List {
	Identifier m_identifier;
	std::vector<Ptr<Expression>> m_body;
};


struct Call : List {
	Identifier m_identifier;
};

struct Op { char m_operator; };

struct App : List, Op {};

using Global = Union<Def, Let>;

using ExpBase = Union <Let, Def, If, While, App, Call, Identifier, Constant>;
struct Expression : ExpBase {
	explicit Expression(ExpBase b) : ExpBase(b) {}

};

struct Toplevel
{
	std::vector<Global> m_globals;
};

namespace {
std::ostream& operator<< ( std::ostream &o, Expression e );

std::ostream &operator<<( std::ostream &o, Identifier i) {
    return o << i.m_name;
}

std::ostream &operator<<( std::ostream &o, IfLike i) {
    o << Expression(*i.m_condition);
    for (auto& a: i.m_body) {
        o << Expression(*a);
    }
    return o;
}

std::ostream &operator<<( std::ostream &o, Let l) {
    return o << l.m_identifier << Expression(*l.m_body);
}

std::ostream &operator<<( std::ostream &o, Def d) {
    o << d.m_identifier;
    for (auto& a: d.m_operands) {
        o << Expression(*a);
    }
    for (auto& a: d.m_body) {
        o << Expression(*a);
    }
    return o;
}

std::ostream &operator<<( std::ostream &o, Call c) {
    o << c.m_identifier;
    for (auto& a: c.m_operands) {
        o << Expression(*a);
    }
    return o;
}

std::ostream &operator<<( std::ostream &o, Global g ) {
    g.match( [&] (Def d) { o << Def( d ); },
             [&] (Let l) {o << Expression( l ); }
    );
    return o;
}

std::ostream &operator<<( std::ostream &o, App a ) {
    o << a.m_operator;
    for (auto& b: a.m_operands) {
        o << Expression(*b);
    }
    return o;
}

std::ostream& operator<< ( std::ostream &o, Expression e )
{
    e.match( [&]( If v ) { o << "if?" << IfLike(v); },
             [&]( Call v ) { o << "("  << Call( v ) << ")"; },
             [&]( App v ) { o << "(" << App( v ) << ")"; },
             [&]( While v ) { o << "(while" << IfLike( v ) << ")"; },
             [&]( Identifier i ) { o << i; },
             [&]( Constant c )
             {
                 c.match( [&]( int i ) { o << i; },
                          [&]( std::string s ) { o << '"' << s << '"'; } );
             },
             [&]( Let v ) { o << "let?" << Let(v); } );
    return o;
}


std::ostream &operator<<( std::ostream &o, Toplevel t ) {
	for (auto& a: t.m_globals) {
		o << a << std::endl;
	}
	return o;
}

}

