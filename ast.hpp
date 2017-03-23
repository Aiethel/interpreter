#include <memory>
#include <vector>
#include "bricks/brick-types"
#include "scope.hpp"

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

	Scope m_scope;
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

	Scope m_scope;
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

	Scope m_scope;
};




namespace {
	void buildScope(SymbolTable& sm, Expression e, Scope* scope);

	void buildSymTable(SymbolTable& sm, Expression e) {
		e.match( [&]( If v ) {
				buildSymTable(sm, *v.m_condition);
				for (auto& a: v.m_body) {
					buildSymTable(sm, *a);
				}
			},
			[&]( Call v ) {
				sm.add(v.m_identifier.m_name);
				for (auto& a: v.m_operands) {
						buildSymTable(sm, *a);
				}
			},
			[&]( App v ) {
				for (auto& b: v.m_operands) {
					buildSymTable(sm, *b);
				}
			},
			[&]( While v ) {
				buildSymTable(sm, *v.m_condition);
				for (auto& a: v.m_body) {
					buildSymTable(sm, *a);
				}
			},
			[&]( Identifier i) {
                          	sm.add(i.m_name);
			},
			[&]( Let v ) {
				buildSymTable(sm, v);
			}
		);

	}

	void buildScope(SymbolTable& sm, IfLike i, Scope* scope) {
        buildScope(sm, *i.m_condition, scope);
        i.m_scope.setparent(scope);
		for (auto& a: i.m_body) {
			buildScope(sm, *a, &i.m_scope);
		}
        scope->save(i.m_scope);
	}

	void buildScope(SymbolTable &sm, Let l, Scope* scope) {
		sm.add(l.m_identifier.m_name);
        scope->define(sm.get(l.m_identifier.m_name));
		buildScope(sm, *l.m_body, scope);
	}

	void buildScope(SymbolTable& sm, Def d, Scope* scope) {
		sm.add(d.m_identifier.m_name);

        scope->define(sm.get(d.m_identifier.m_name));
        d.m_scope.setparent(scope);

		for (auto a: d.m_operands) {
            a->match([&](Identifier i) {
                sm.add(i.m_name);
                d.m_scope.define(sm.get(i.m_name));
            });
			buildScope(sm, *a, &d.m_scope);
		}

		for (auto& a: d.m_body) {
			buildScope(sm, *a, &d.m_scope);
		}

        scope->save(d.m_scope);
	}

	void buildScope(SymbolTable& sm, Toplevel& tl) {
        for (auto& g : tl.m_globals) {
		g.match(
			[&](Def d) {buildSymTable(sm, d);},
			[&](Let l) {buildSymTable(sm, l);}
		);
	}
	

	for (auto& g : tl.m_globals) {
			g.match(
				[&] (Def d) {buildScope(sm, d, &tl.m_scope);},
				[&] (Let l) {buildScope(sm, l, &tl.m_scope);}
			);
		}
	}

	void buildScope(SymbolTable& sm, Expression e, Scope* scope)
	{
		e.match( [&]( If v ) {
					 buildScope(sm, v, scope);
				 },
				 [&]( Call v ) {
					sm.add(v.m_identifier.m_name);
				        scope->check(sm.get(v.m_identifier.m_name));
					for (auto& a: v.m_operands) {
						buildScope(sm, *a, scope);
					}
				 },
				 [&]( App v ) {
					for (auto& b: v.m_operands) {
						buildScope(sm, *b, scope);
					}
				 },
				 [&]( While v ) {
					 buildScope(sm, v, scope);
				 },
				 [&]( Identifier i) {
                          		sm.add(i.m_name);
        				scope->check(sm.get(i.m_name));
				 },
				 [&]( Let v ) {
					 buildScope(sm, v, scope );
				 }
		);
	}

}




namespace {
std::ostream& operator<< ( std::ostream &o, Expression e );

std::ostream &operator<<( std::ostream &o, Identifier i) {
    return o << i.m_name;
}

std::ostream &operator<<( std::ostream &o, IfLike i) {
    o << *i.m_condition;
    for (auto& a: i.m_body) {
        o << *a;
    }
    return o;
}

std::ostream &operator<<( std::ostream &o, Let l) {
    return o << l.m_identifier << *l.m_body;
}

std::ostream &operator<<( std::ostream &o, Def d) {
    o << d.m_identifier;
    for (auto& a: d.m_operands) {
        o << *a;
    }
    for (auto& a: d.m_body) {
        o << *a;
    }
    return o;
}

std::ostream &operator<<( std::ostream &o, Call c) {
    o << c.m_identifier;
    for (auto& a: c.m_operands) {
        o << *a;
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
        o << *b;
    }
    return o;
}

std::ostream& operator<< ( std::ostream &o, Expression e )
{
    e.match( [&]( If v ) { o << "if?" << IfLike(v); },
             [&]( Call v ) { o << "("  << v << ")"; },
             [&]( App v ) { o << "(" << v << ")"; },
             [&]( While v ) { o << "(while" << IfLike( v ) << ")"; },
             [&]( Identifier i ) { o << i; },
             [&]( Constant c )
             {
                 c.match( [&]( int i ) { o << i; },
                          [&]( std::string s ) { o << '"' << s << '"'; } );
             },
             [&]( Let v ) { o << "let?" << v; } );
    return o;
}


std::ostream &operator<<( std::ostream &o, Toplevel t ) {
	for (auto& a: t.m_globals) {
		o << a << std::endl;
	}
	return o;
}

}

