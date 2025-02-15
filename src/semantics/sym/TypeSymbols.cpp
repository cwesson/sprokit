/**
 * @file TypeSymbols.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "TypeSymbols.h"
#include "FunctionSymbols.h"
#include <iomanip>

TypeSymbols::TypeSymbols(const std::string& name, SymbolTable* p) :
	SymbolTable("type " + name, p)
{}

std::ostream& TypeSymbols::print(std::ostream& os, unsigned int depth) const {
	SymbolTable::print(os, depth);
	
	os << "| Name            | Type       | Unit       | const |" << std::endl;
	os << "|-----------------|------------|------------|-------|" << std::endl;
	for(auto sym : vars) {
		os << std::left << "| " << std::setw(15) << sym.first << " | " << std::setw(10) << sym.second->type << " | " << std::setw(10) << sym.second->unit
			<< " | " << std::setw(5) << (sym.second->constant ? "true " : "false") << " |" << std::endl;
	}
	for(auto sym : funcs) {
		os << std::left << "| " << std::setw(15) << (sym.first + "()") << " | " << std::setw(10) << sym.second->type << " | " << std::setw(10) << sym.second->unit
			<< " | " << std::setw(5) << "n/a" << " |" << std::endl;
	}
	os << std::endl;

	for(auto sym : funcs) {
		sym.second->table->print(os, depth+1);
	}

	return os;
}

SymbolTable::variable* TypeSymbols::addVariable(const std::string& n) {
	if(vars.contains(n)){
		return nullptr;
	}else{
		vars[n] = new SymbolTable::variable();
		return vars[n];
	}
}

SymbolTable::function* TypeSymbols::addFunction(const std::string& n) {
	if(funcs.contains(n)){
		return nullptr;
	}else{
		funcs[n] = new SymbolTable::function();
		funcs[n]->table = new FunctionSymbols(name + "::" + n, this);
		return funcs[n];
	}
}
