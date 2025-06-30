/**
 * @file OrderedSymbol.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "SymbolTable.h"

/**
 * Function symbol table.
 * @ingroup sym
 */
class OrderedSymbol : public SymbolTable {
	public:
		/**
		 * Constructor.
		 * @param n Table name. 
		 * @param p Parent symbol table.
		 */
		OrderedSymbol(const std::string& n, SymbolTable* p);

		/**
		 * Destructor.
		 */
		virtual ~OrderedSymbol() = default;
		
		virtual std::ostream& print(std::ostream& os, unsigned int depth) const override;

		virtual function* addFunction(const std::string& n) override;

		virtual SymbolTable* addType(const std::string& n) override;

		virtual unit* addUnit(const std::string& name) override;
		
		virtual parameter* addParameter(const std::string& n) override;

		virtual variable* findVariable(const std::string& n) override;

		variable var; ///< The variable symbol.
};
