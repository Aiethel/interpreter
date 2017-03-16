#include <map>
#include <string>
#include <set>
#include <memory>

struct SymbolTable {
	std::map<std::string, int> m_data;

	void add(const std::string& str) {
		m_data.insert(std::make_pair(str, m_data.size()));
	}

	int get(const std::string& str) {
        auto a = m_data.find(str);
        if( a == m_data.end() ) {
            return -1;
        }
        return a->second;

	}
};

std::ostream& operator<<(std::ostream& o, const SymbolTable& sm) {
	for (auto a: sm.m_data) {
		o << a.first << ": " << a.second << std::endl;
	}
	return o << std::endl;
}

struct Scope {
	Scope* m_parent;
	std::set<int> m_identifiers;
    std::vector<Scope> m_children;

    Scope() = default;
    Scope(Scope* parent) : m_parent(parent) {
        //Empty
    }

    Scope(const Scope& scope) = default;

    void check(int n) {
        Scope* hndl = this;
        while (hndl) {
            if (hndl->m_identifiers.find(n) != hndl->m_identifiers.end()) {
                return;
            }
            hndl = hndl->m_parent;
        }
        std::cerr << "THROWING NOT IN SCOPE: " << n << std::endl;
        throw("Using variable not in scope");
    }

	void define(int n) {
		if (m_identifiers.find(n) != m_identifiers.end()) {
			throw("Duplicate identifier");
		}
		m_identifiers.insert(n);
	}

    void setparent(Scope* parent) {
        m_parent = parent;;
    }

    void save(const Scope& scope) {
        m_children.push_back(std::move(scope));
    }
};

std::ostream& operator<<(std::ostream& o, const Scope& scope) {
    o << "In scope ";
    for (auto& a: scope.m_identifiers) {
        o << a << " ";
    }
    o << std::endl;
    for (auto& a : scope.m_children) {
        o << a;
    }
    return o;
}