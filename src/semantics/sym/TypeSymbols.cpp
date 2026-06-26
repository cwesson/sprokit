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

	for(const auto* sym : children){
		sym->print(os, depth+1);
	}

	os << "|    # | Name            | Type       | Unit       | const | used  | modif |" << std::endl;
	os << "|-----:|-----------------|------------|------------|-------|-------|-------|" << std::endl;
	
	for(unsigned int i = 0; i < vars.size(); ++i){
		const auto& sym = vars[i];
		os << std::right << "|" << std::setw(5) << i << std::left << " | " << std::setw(15) << sym->name
			<< " | " << std::setw(10) << (std::string)*sym->type << " | " << std::setw(10) << sym->unit
			<< " | " << std::setw(5) << (sym->constant ? "true " : "false")
			<< " | " << std::setw(5) << (sym->used ? "true " : "false")
			<< " | " << std::setw(5) << (sym->modified ? "true " : "false") << " |" << std::endl;

	}
	os << std::endl;
	
	os << "| Name            | Type       | Unit       | const | point |" << std::endl;
	os << "|-----------------|------------|------------|-------|-------|" << std::endl;
	for(const auto& sym : funcs) {
		os << std::left << "| " << std::setw(15) << (sym.first + "()") << " | " << std::setw(10) << sym.second->type << " | " << std::setw(10) << sym.second->unit
			<< " | " << std::setw(5) << "n/a"
			<< " | " << std::setw(5) << (sym.second->pointer ? "true " : "false") << " |" << std::endl;
	}
	os << std::endl;

	for(const auto& sym : funcs) {
		sym.second->table->print(os, depth+1);
	}

	return os;
}

SymbolTable::variable* TypeSymbols::addVariable(const std::string& n, SymbolTable** outtable) {
	for(auto* mem : vars){
		if(mem->name == n){
			return nullptr;
		}
	}
	if(funcs.contains(n)){
		return nullptr;
	}else{
		member* var = new SymbolTable::member(n);
		vars.push_back(var);
		if(outtable != nullptr){
			*outtable = this;
		}
		return var;
	}
}

SymbolTable::function* TypeSymbols::addFunction(const std::string& n) {
	for(auto* mem : vars){
		if(mem->name == n){
			return nullptr;
		}
	}
	if(funcs.contains(n)){
		return nullptr;
	}else{
		funcs[n] = new SymbolTable::function();
		funcs[n]->table = new FunctionSymbols(name + "::" + n, this);
		return funcs[n];
	}
}

SymbolTable::variable* TypeSymbols::findVariable(const std::string& n) {
	for(auto* mem : vars){
		if(mem->name == n){
			return mem;
		}
	}
	if(parent != nullptr){
		return parent->findVariable(n);
	}else{
		return nullptr;
	}
}

bool TypeSymbols::isScope() const {
	return true;
}
