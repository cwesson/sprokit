/**
 * @file OrderedSymbol.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "OrderedSymbol.h"
#include "ScopedSymbols.h"
#include <iomanip>

OrderedSymbol::OrderedSymbol(const std::string& n, SymbolTable* p) :
	SymbolTable(n, p),
	var()
{}

std::ostream& OrderedSymbol::print(std::ostream& os, unsigned int depth) const {
	for(unsigned int i = 0; i < depth; ++i){
		os << "#";
	}
	os << " " << name << std::endl;

	os << "| Name            | Type       | Unit       | const | point | used  | modif |" << std::endl;
	os << "|-----------------|------------|------------|-------|-------|-------|-------|" << std::endl;
	
	os << std::left << "| " << std::setw(15) << name
		<< " | " << std::setw(10) << (std::string)*var.type << " | " << std::setw(10) << var.unit
		<< " | " << std::setw(5) << (var.constant ? "true " : "false")
		<< " | " << std::setw(5) << (var.pointer ? "true " : "false")
		<< " | " << std::setw(5) << (var.used ? "true " : "false")
		<< " | " << std::setw(5) << (var.modified ? "true " : "false") << " |" << std::endl;
	os << std::endl;

	for(auto sym : children){
		sym->print(os, depth+1);
	}

	return os;
}

SymbolTable::function* OrderedSymbol::addFunction(const std::string& n) {
	return parent->addFunction(n);
}

SymbolTable* OrderedSymbol::addType(const std::string& n) {
	return parent->addType(n);
}

SymbolTable::unit* OrderedSymbol::addUnit(const std::string& name) {
	return parent->addUnit(name);
}

SymbolTable::parameter* OrderedSymbol::addParameter(const std::string& n) {
	return parent->addParameter(n);
}

SymbolTable::variable* OrderedSymbol::findVariable(const std::string& n) {
	if(name == n){
		return &var;
	}
	return parent->findVariable(n);
}
