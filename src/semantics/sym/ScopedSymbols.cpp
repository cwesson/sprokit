/**
 * @file ScopedSymbols.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "ScopedSymbols.h"
#include <iomanip>

ScopedSymbols::ScopedSymbols(const std::string& n, SymbolTable* p) :
	OrderedSymbol(p->name + n, p)
{}

std::ostream& ScopedSymbols::print(std::ostream& os, unsigned int depth) const {
	for(unsigned int i = 0; i < depth; ++i){
		os << "#";
	}
	os << " " << name << std::endl;

	for(auto sym : children){
		sym->print(os, depth+1);
	}

	return os;
}

bool ScopedSymbols::isScope() const {
	return true;
}
