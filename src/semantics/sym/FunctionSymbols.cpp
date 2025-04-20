/**
 * @file FunctionSymbols.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "FunctionSymbols.h"
#include <iomanip>

FunctionSymbols::FunctionSymbols(const std::string& name, SymbolTable* p) :
	SymbolTable(name + "()", p)
{}

FunctionSymbols::~FunctionSymbols(){
	while(!params.empty()){
		delete params.back();
		params.pop_back();
	}
	for(auto v : vars){
		delete v.second;
	}
	vars.clear();
}

std::ostream& FunctionSymbols::print(std::ostream& os, unsigned int depth) const {
	SymbolTable::print(os, depth);
	
	os << "|    # | Name            | Type       | Unit       | const | point | used  | modif |" << std::endl;
	os << "|-----:|-----------------|------------|------------|-------|-------|-------|-------|" << std::endl;
	unsigned int index = 1;
	for(auto sym : params) {
		os << "| " << std::right << std::setw(4) << index++ << " | " << std::left << std::setw(15) << sym->name
			<< " | " << std::setw(10) << (std::string)*sym->type << " | " << std::setw(10) << sym->unit
			<< " | " << std::setw(5) << (sym->constant ? "true " : "false")
			<< " | " << std::setw(5) << (sym->pointer ? "true " : "false")
			<< " | " << std::setw(5) << (sym->used ? "true " : "false")
			<< " | " << std::setw(5) << (sym->modified ? "true " : "false") << " |" << std::endl;
	}
	for(auto sym : vars) {
		os << std::left << "|      | " << std::setw(15) << sym.first
			<< " | " << std::setw(10) << (std::string)*sym.second->type << " | " << std::setw(10) << sym.second->unit
			<< " | " << std::setw(5) << (sym.second->constant ? "true " : "false")
			<< " | " << std::setw(5) << (sym.second->pointer ? "true " : "false")
			<< " | " << std::setw(5) << (sym.second->used ? "true " : "false")
			<< " | " << std::setw(5) << (sym.second->modified ? "true " : "false") << " |" << std::endl;
	}
	os << std::endl;

	return os;
}

SymbolTable::variable* FunctionSymbols::addVariable(const std::string& n) {
	if(vars.contains(n)){
		return nullptr;
	}else{
		for(auto p : params) {
			if(p->name == n){
				return nullptr;
			}
		}
		vars[n] = new SymbolTable::variable();
		return vars[n];
	}
}

SymbolTable::parameter* FunctionSymbols::addParameter(const std::string& n) {
	if(vars.contains(n)){
		return nullptr;
	}else{
		for(auto p : params) {
			if(p->name == n){
				return nullptr;
			}
		}
		auto p = new SymbolTable::parameter(n);
		params.push_back(p);
		return p;
	}
}

SymbolTable::variable* FunctionSymbols::findVariable(const std::string& n) {
	for(auto p : params) {
		if(p->name == n) {
			return p;
		}
	}
	if(vars.contains(n)){
		return vars[n];
	}
	return parent->findVariable(n);
}
