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
		virtual ~FunctionSymbols() override;
		
		virtual std::ostream& print(std::ostream& os, unsigned int depth) const override;

		virtual parameter* addParameter(const std::string& n) override;

		virtual variable* findVariable(const std::string& n) override;

		virtual bool isScope() const override;

		std::vector<parameter*> params;        ///< List of declared function parameters.
};
