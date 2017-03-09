#include <map>
#include <string>
#include <set>

struct SymbolTable {
	std::map<std::string, int> m_data;

	void add(const std::string& str) {
		m_data.insert(str, m_data.size());
	}

	int get(const std::string& str) {
		*m_data.find(n);
	}
}

struct Scope {
	Scope* m_parent;
	std::set<int> m_identifiers;

	void add(int n) {
		if (m_identifiers.find(n) != m_identifiers.end()) {
			throw("Duplicate identifier");
		}
		m_identifiers.insert(n);
	}
}
