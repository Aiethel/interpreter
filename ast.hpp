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
	Identifier(const std::string& str) : m_name(str) {};
	Identifier() = default;
};

using Constant = Union<int, std::string>;

//keyword + ( condition )
struct IfLike
{
	Ptr<Expression> m_condition, m_body;
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
	Identifier m_indentifier;
	Ptr<Expression> m_body;
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
