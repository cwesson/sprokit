/**
 * @file GlobalSymbols.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "GlobalSymbols.h"
#include "FunctionSymbols.h"
#include "OrderedSymbol.h"
#include <iomanip>

GlobalSymbols::GlobalSymbols() :
	SymbolTable("$global", nullptr)
{}

GlobalSymbols::~GlobalSymbols() {
	for(auto f : funcs){
		delete f.second;
	}
	funcs.clear();
	for(auto t : types){
		delete t.second;
	}
	types.clear();
}

std::ostream& GlobalSymbols::print(std::ostream& os, unsigned int depth) const {
	SymbolTable::print(os, depth);
	
	os << "| Name            | Type       | Unit       | const | point | used  | modif |" << std::endl;
	os << "|-----------------|------------|------------|-------|-------|-------|-------|" << std::endl;
	for(auto sym : funcs) {
		os << std::left << "| " << std::setw(15) << (sym.first + "()") << " | " << std::setw(10) << (std::string)*sym.second->type << " | " << std::setw(10) << sym.second->unit
			<< " | n/a  "
			<< " | " << std::setw(5) << (sym.second->pointer ? "true " : "false")
			<< " | n/a   | n/a   |" << std::endl;
	}
	os << std::endl;

	os << "| Name            | Expanded        |" << std::endl;
	os << "|-----------------|-----------------|" << std::endl;

	for(auto sym : units) {
		os << std::left << "| " << std::setw(15) << sym.first << " | " << std::setw(15) << sym.second->expanded << " |" << std::endl;
	}
	os << std::endl;

	for(auto sym : children){
		sym->print(os, depth+1);
	}

	for(auto sym : funcs) {
		sym.second->table->print(os, depth+1);
	}

	for(auto sym : types) {
		sym.second->print(os, depth+1);
	}

	for(auto sym : units) {
		for(unsigned int i = 0; i < depth+1; ++i){
			os << "#";
		}
		os << " " << sym.first << std::endl;
		os << "| To              | Symbol          |" << std::endl;
		os << "|-----------------|-----------------|" << std::endl;
		for(auto con : sym.second->conversions){
			os << "| " << std::setw(15) << con.first << " | " << std::setw(15) << con.second.var << " |" << std::endl;
		}
		os << std::endl;
	}

	return os;
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

SymbolTable::unit* GlobalSymbols::addUnit(const std::string& name)
{
	if(!units.contains(name)){
		units[name] = new unit();
	}
	return units[name];
}

SymbolTable::function* GlobalSymbols::findFunction(const std::string& n) {
	if(funcs.contains(n)){
		return funcs[n];
	}else{
		return nullptr;
	}
}

SymbolTable* GlobalSymbols::findType(const std::string& n) {
	if(types.contains(n)){
		return types[n];
	}else{
		return nullptr;
	}
}

SymbolTable::unit* GlobalSymbols::findUnit(const std::string& n) {
	if(units.contains(n)){
		return units[n];
	}else{
		return nullptr;
	}
}
