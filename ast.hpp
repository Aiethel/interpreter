#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <stack>

#include <brick-types>

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

	void buildSymTable(SymbolTable& sm, Expression e);

    void buildSymTable(SymbolTable& sm, IfLike i) {
        for (auto& a: i.m_body) {
            buildSymTable(sm, *a);
        }
    }

    void buildSymTable(SymbolTable &sm, Let l) {
        sm.add(l.m_identifier.m_name);
        buildSymTable(sm, *l.m_body);
    }

    void buildSymTable(SymbolTable& sm, Def d) {
        sm.add(d.m_identifier.m_name);
        sm.func_ptrs.insert({sm.get(d.m_identifier.m_name), Def(d)});
        for (auto a: d.m_operands) {
            a->match([&](Identifier i) {
                sm.add(i.m_name);
            });
            buildSymTable(sm, *a);
        }
        for (auto& a: d.m_body) {
            buildSymTable(sm, *a);
        }
    }


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
        scope->define(sm.get(l.m_identifier.m_name));
        buildScope(sm, *l.m_body, scope);
    }

    void buildScope(SymbolTable& sm, Def d, Scope* scope) {
        scope->define(sm.get(d.m_identifier.m_name));
        d.m_scope.setparent(scope);

        for (auto a: d.m_operands) {
            a->match([&](Identifier i) {
                d.m_scope.define(sm.get(i.m_name));
            });
            buildScope(sm, *a, &d.m_scope);
        }

        for (auto& a: d.m_body) {
            buildScope(sm, *a, &d.m_scope);
        }
        scope->save(d.m_scope);
    }

	void buildScope(SymbolTable& sm, Expression e, Scope* scope)
	{
		e.match( [&]( If v ) {
                     buildScope(sm, v, scope);
				 },
				 [&]( Call v ) {
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
        				scope->check(sm.get(i.m_name));
				 },
				 [&]( Let v ) {
					 buildScope(sm, v, scope );
				 }
		);
	}


    void buildScope(SymbolTable& sm, Toplevel& tl) {
        for (auto& g : tl.m_globals) {
            g.match(
                    [&](Def d) {buildSymTable(sm, d);},
                    [&](Let l) {buildSymTable(sm, l);}
            );
        }

        tl.m_scope.define(0);
        for (auto& g : tl.m_globals) {
            g.match(
                    [&] (Def d) {buildScope(sm, d, &tl.m_scope);},
                    [&] (Let l) {buildScope(sm, l, &tl.m_scope);}
            );
        }

        if (sm.get("main") == -1) {
            std::cerr << "Main not defined" << std::endl;
            throw ScopeError("Main not found");
        }
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

namespace {
    using Value = Union<int, std::string>;
    using SptrVal = std::shared_ptr<Value>;
    using ValMap = std::unordered_map<int, SptrVal>;


    std::ostream& operator<<(std::ostream& os, Value val) {
        val.match(
                [&] (int i) {os << "* int " << i << std::endl;},
                [&] (std::string i) {os << "* int " << i << std::endl;}
        );
        return os;
    }

    Value eval(SymbolTable& sm, Expression e, ValMap& vals);

    Value eval(SymbolTable& sm, While i, ValMap& vals) {
        auto x = eval(sm, *i.m_condition, vals);
        while( x != 0) {
            for (auto& a: i.m_body) {
                eval(sm, *a, vals);
            }
            x = eval(sm, *i.m_condition, vals);
        }
        return x;
    }

    //Evaluates inside of IfLike only if condition is met
    Value eval(SymbolTable& sm, If i, ValMap& vals) {
        auto x = eval(sm, *i.m_condition, vals);
        if( x != 0) {
            for (auto& a: i.m_body) {
                eval(sm, *a, vals);
            }
        }
        return x;
    }

    //Sets new variable in ValMap
    Value eval(SymbolTable &sm, Let l, ValMap& vals) {
        Value x = eval(sm, *l.m_body, vals);
        auto iter = vals.find(sm.get(l.m_identifier.m_name));
        if (iter == vals.end()) {
            vals.insert({sm.get(l.m_identifier.m_name), std::make_shared<Value>(x)});
        } else {
            *iter->second = x;
        }
        x.match(
                [&](int i) {std::cout <<"let" << i << std::endl;}
        );
        return x;
    }

    //Simply evaluates the body, arguments are already passed in
    Value eval(SymbolTable& sm, Def d, ValMap& vals) {
        Value x;
        for (auto& a: d.m_body) {
            x = eval(sm, *a, vals);
        }
        return x;
    }

    //Setup new scope and returns inside of function
    Value eval(SymbolTable& sm, Call c, ValMap& vals) {
        Def* next = &sm.func_ptrs.find(sm.get(c.m_identifier.m_name))->second;
        if (next->m_operands.size() != c.m_operands.size()) {
            throw std::runtime_error("Calling " + next->m_identifier.m_name + "with wrong number of arguments");
        }
        ValMap newVal;
        for (int i = 0; i < c.m_operands.size(); ++i) {
            next->m_operands[i]->match(
                    [&](Identifier n) {
                        auto h = *c.m_operands[i];
                        h.match(
                                [&](Identifier q) {newVal.insert({sm.get(n.m_name),
                                                                  SptrVal(vals.find(sm.get(q.m_name))->second)});
                                                                 }
                        );
                        newVal.insert({sm.get(n.m_name), std::make_shared<Value>(eval(sm, *c.m_operands[i], vals))});
                    }
            );
        }
        return eval(sm, *next, newVal);
    }



    Value eval(SymbolTable& sm, App a, ValMap& vals) {
        char o = a.m_operator;

        if (o == '~') {
            Value x = 0;
            a.m_operands[0]->match(
                    [&](Identifier i) {
                        auto hndl = vals.find(sm.get(i.m_name));
                        x = eval(sm, *a.m_operands[1], vals);
                        *hndl->second = x;
                    }
            );
        }
        auto rhs = eval(sm, *a.m_operands[0], vals);
        auto lhs = eval(sm, *a.m_operands[1], vals);

        Value x;

        rhs.match(
                [&](int i) {
                    lhs.match(
                            [&](int j) {
                                if (o == '+') {
                                    x = i + j;
                                } else if (o == '-') {
                                    x = i - j;
                                } else if (o == '*') {
                                    x = i * j;
                                } else if (o == '/') {
                                    x = i / j;
                                }
                            }
                    );
                }
        );

        if (o == '=') {
            if (rhs == lhs) {
                return 1;
            } else {
                return 0;
            }
        } else if (o == '!') {
            if (rhs == lhs) {
                return 0;
            } else {
                return 1;
            }
        }
        return x;
    }

    //Dispatch function for every type of node
    Value eval(SymbolTable& sm, Expression e, ValMap& vals)
    {
        Value x;
        e.match( [&]( If v ) {
                     x = eval(sm, v, vals);
                 },
                 [&]( Call v ) {
                     sm.call_stack.push(&e);
                    x =  eval(sm, v, vals);
                 },
                 [&]( App v ) {
                    x = eval(sm, v, vals);
                 },
                 [&]( While v ) {
                     x = eval(sm, v, vals);
                 },
                 [&]( Identifier i) {
                     x = *vals.find(sm.get(i.m_name))->second;
                 },
                 [&]( Let v ) {
                     x = eval(sm, v, vals);
                 },
                 [&] (Constant c) {
                     x = c;
                 }
        );
        return x;
    }


    Value eval(SymbolTable& sm, Toplevel& tl) {
        Def* a = &sm.func_ptrs.find(sm.get("main"))->second;
        ValMap vals;
        for (auto b : a->m_operands) {
            int index = 0;
            b->match(
                    [&](Identifier i) {index = sm.get(i.m_name);}
            );
            vals.insert({index, std::make_shared<Value>()});
        }
        eval(sm, *a, vals);
        return *vals.find(sm.get("a"))->second.get();
    }

}
