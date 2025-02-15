/**
 * @file GlobalSymbols.CPP
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "GlobalSymbols.h"
#include "FunctionSymbols.h"
#include <iomanip>

GlobalSymbols::GlobalSymbols() :
	SymbolTable("$global", nullptr)
{}

std::ostream& GlobalSymbols::print(std::ostream& os, unsigned int depth) const {
	SymbolTable::print(os, depth);
	
	os << "| Name            | Type       | Unit       | const | used  | modif |" << std::endl;
	os << "|-----------------|------------|------------|-------|-------|-------|" << std::endl;
	for(auto sym : vars) {
		os << std::left << "| " << std::setw(15) << sym.first << " | " << std::setw(10) << sym.second->type << " | " << std::setw(10) << sym.second->unit
			<< " | " << std::setw(5) << (sym.second->constant ? "true " : "false")
			<< " | " << std::setw(5) << (sym.second->used ? "true " : "false")
			<< " | " << std::setw(5) << (sym.second->modified ? "true " : "false") << " |" << std::endl;
	}
	for(auto sym : funcs) {
		os << std::left << "| " << std::setw(15) << (sym.first + "()") << " | " << std::setw(10) << sym.second->type << " | " << std::setw(10) << sym.second->unit
			<< " | n/a   | n/a   | n/a   |" << std::endl;
	}
	os << std::endl;

	for(auto sym : funcs) {
		sym.second->table->print(os, depth+1);
	}

	for(auto sym : types) {
		sym.second->print(os, depth+1);
	}

	return os;
}

SymbolTable::variable* GlobalSymbols::addVariable(const std::string& n) {
	if(vars.contains(n)){
		return nullptr;
	}else{
		vars[n] = new SymbolTable::variable();
		return vars[n];
	}
}

SymbolTable::function* GlobalSymbols::addFunction(const std::string& n) {
	if(funcs.contains(n)){
		return nullptr;
	}else{
		funcs[n] = new SymbolTable::function();
		funcs[n]->table = new FunctionSymbols(n, this);
		return funcs[n];
	}
}

SymbolTable* GlobalSymbols::addType(const std::string& n) {
	if(types.contains(n)){
		return nullptr;
	}else{
		types[n] = new TypeSymbols(n, this);
		return types[n];
	}
}

