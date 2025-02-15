/**
 * @file TypeSymbols.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "SymbolTable.h"
#include <map>

/**
 * Type symbol table.
 * @ingroup sym
 */
class TypeSymbols : public SymbolTable {
	public:
		TypeSymbols(const std::string& name, SymbolTable* p);
		
		virtual std::ostream& print(std::ostream& os, unsigned int depth) const override;
		
		virtual variable* addVariable(const std::string& n) override;

		virtual function* addFunction(const std::string& n) override;

		std::map<std::string, variable*> vars;
		std::map<std::string, function*> funcs;
};
