/**
 * @file FunctionSymbols.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "SymbolTable.h"
#include <map>
#include <vector>

/**
 * Function symbol table.
 * @ingroup sym
 */
class FunctionSymbols : public SymbolTable {
	public:
		FunctionSymbols(const std::string& name, SymbolTable* p);
		
		virtual std::ostream& print(std::ostream& os, unsigned int depth) const override;
		
		virtual variable* addVariable(const std::string& n) override;
		
		virtual parameter* addParameter(const std::string& n) override;

		virtual variable* findVariable(const std::string& n) override;

		std::vector<parameter*> params;
		std::map<std::string, variable*> vars;
};
