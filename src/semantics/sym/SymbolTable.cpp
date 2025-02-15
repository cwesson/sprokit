/**
 * @file SymbolTable.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 * 
 * @defgroup sym Symbol Table
 * @{
 */

#include "SymbolTable.h"
#include <string>
#include <iostream>

SymbolTable::SymbolTable(const std::string& n, SymbolTable* p) :
	name(n),
	parent(p)
{}

std::ostream& SymbolTable::print(std::ostream& os, unsigned int depth) const {
	for(unsigned int i = 0; i < depth; ++i){
		os << "#";
	}
	os << " " << name << std::endl;
	return os;
}

SymbolTable::variable* SymbolTable::addVariable(const std::string& n) {
	return nullptr;
}

SymbolTable::parameter* SymbolTable::addParameter(const std::string& n) {
	return nullptr;
}

SymbolTable::function* SymbolTable::addFunction(const std::string& n) {
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

std::ostream& operator<<(std::ostream& os, const SymbolTable& table) {
    table.print(os, 1);
    return os;
}

/** @} */
