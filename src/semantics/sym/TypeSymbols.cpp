/**
 * @file TypeSymbols.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "TypeSymbols.h"
#include "FunctionSymbols.h"
#include "OrderedSymbol.h"
#include <iomanip>

TypeSymbols::TypeSymbols(const std::string& name, SymbolTable* p) :
	SymbolTable("type " + name, p)
{}

TypeSymbols::~TypeSymbols() {
	for(auto f : funcs){
		delete f.second;
	}
	funcs.clear();
}

std::ostream& TypeSymbols::print(std::ostream& os, unsigned int depth) const {
	SymbolTable::print(os, depth);

	for(auto sym : children){
		sym->print(os, depth+1);
	}

	os << "| Name            | Type       | Unit       | const | point | used  | modif |" << std::endl;
	os << "|-----------------|------------|------------|-------|-------|-------|-------|" << std::endl;
	for(auto sym : vars){
		os << std::left << "| " << std::setw(15) << sym.first
			<< " | " << std::setw(10) << (std::string)*sym.second->type << " | " << std::setw(10) << sym.second->unit
			<< " | " << std::setw(5) << (sym.second->constant ? "true " : "false")
			<< " | " << std::setw(5) << (sym.second->pointer ? "true " : "false")
			<< " | " << std::setw(5) << (sym.second->used ? "true " : "false")
			<< " | " << std::setw(5) << (sym.second->modified ? "true " : "false") << " |" << std::endl;
		os << std::endl;
	}
	
	os << "| Name            | Type       | Unit       | const | point |" << std::endl;
	os << "|-----------------|------------|------------|-------|-------|" << std::endl;
	for(auto sym : funcs) {
		os << std::left << "| " << std::setw(15) << (sym.first + "()") << " | " << std::setw(10) << sym.second->type << " | " << std::setw(10) << sym.second->unit
			<< " | " << std::setw(5) << "n/a"
			<< " | " << std::setw(5) << (sym.second->pointer ? "true " : "false") << " |" << std::endl;
	}
	os << std::endl;

	for(auto sym : funcs) {
		sym.second->table->print(os, depth+1);
	}

	return os;
}

SymbolTable::variable* TypeSymbols::addVariable(const std::string& n, SymbolTable** outtable) {
	if(vars.contains(n) || funcs.contains(n)){
		return nullptr;
	}else{
		vars[n] = new SymbolTable::variable();
		if(outtable != nullptr){
			*outtable = this;
		}
		return vars[n];
	}
}

SymbolTable::function* TypeSymbols::addFunction(const std::string& n) {
	if(funcs.contains(n) || vars.contains(n)){
		return nullptr;
	}else{
		funcs[n] = new SymbolTable::function();
		funcs[n]->table = new FunctionSymbols(name + "::" + n, this);
		return funcs[n];
	}
}

SymbolTable::variable* TypeSymbols::findVariable(const std::string& n) {
	if(vars.contains(n)){
		return vars[n];
	}else if(parent != nullptr){
		return parent->findVariable(n);
	}else{
		return nullptr;
	}
}

bool TypeSymbols::isScope() const {
	return true;
}
