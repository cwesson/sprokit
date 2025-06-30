/**
 * @file SymbolTable.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 * 
 * @defgroup sym Symbol Table
 * @{
 */

#include "SymbolTable.h"
#include "FunctionSymbols.h"
#include "OrderedSymbol.h"
#include "ScopedSymbols.h"
#include <string>
#include <iostream>

SymbolTable::SymbolTable(const std::string& n, SymbolTable* p) :
	parent(p),
	name(n)
{}


SymbolTable::function::~function() {
	delete table;
}

std::ostream& SymbolTable::print(std::ostream& os, unsigned int depth) const {
	for(unsigned int i = 0; i < depth; ++i){
		os << "#";
	}
	os << " " << name << std::endl;
	return os;
}

SymbolTable::variable* SymbolTable::addVariable(const std::string& n, SymbolTable** outtable) {
	variable* var = findVariable(n);
	if(var != nullptr){
		return nullptr;
	}else{
		OrderedSymbol* sym = new OrderedSymbol(n, this);
		children.push_back(sym);
		if(outtable != nullptr){
			*outtable = sym;
		}
		return &sym->var;
	}
}

ScopedSymbols* SymbolTable::addScope(const std::string& n) {
	ScopedSymbols* t = new ScopedSymbols(n, this);
	children.push_back(t);
	return t;
}

SymbolTable::parameter* SymbolTable::addParameter(const std::string& n) {
	return nullptr;
}

SymbolTable::function* SymbolTable::addFunction(const std::string& n) {
	return nullptr;
}

SymbolTable::unit* SymbolTable::addUnit(const std::string& n) {
	if(parent != nullptr){
		return parent->addUnit(n);
	}
	return nullptr;
}

SymbolTable* SymbolTable::addType(const std::string& n) {
	return nullptr;
}

SymbolTable::variable* SymbolTable::findVariable(const std::string& n) {
	if(parent != nullptr){
		return parent->findVariable(n);
	}else{
		return nullptr;
	}
}

SymbolTable::function* SymbolTable::findFunction(const std::string& n) {
	if(parent != nullptr){
		return parent->findFunction(n);
	}else{
		return nullptr;
	}
}

SymbolTable* SymbolTable::findType(const std::string& n) {
	if(parent != nullptr){
		return parent->findType(n);
	}else{
		return nullptr;
	}
}

bool SymbolTable::isScope() const {
	return false;
}

std::ostream& operator<<(std::ostream& os, const SymbolTable& table) {
    table.print(os, 1);
    return os;
}

/** @} */
