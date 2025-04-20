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
		/**
		 * Constructor.
		 * @param name Declared function name.
		 * @param p Parent symbol table.
		 */
		FunctionSymbols(const std::string& name, SymbolTable* p);

		/**
		 * Destructor.
		 */
		virtual ~FunctionSymbols();
		
		virtual std::ostream& print(std::ostream& os, unsigned int depth) const override;
		
		virtual variable* addVariable(const std::string& n) override;
		
		virtual parameter* addParameter(const std::string& n) override;

		virtual variable* findVariable(const std::string& n) override;

		std::vector<parameter*> params;        ///< List of declared function parameters.
		std::map<std::string, variable*> vars; ///< Map of variables in the function.
};
