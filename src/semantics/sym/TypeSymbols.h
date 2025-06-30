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
		/**
		 * Constructor.
		 * @param name Declared type name.
		 * @param p Parent symbol table.
		 */
		TypeSymbols(const std::string& name, SymbolTable* p);

		/**
		 * Destructor.
		 */
		~TypeSymbols();
		
		virtual std::ostream& print(std::ostream& os, unsigned int depth) const override;

		virtual SymbolTable::variable* addVariable(const std::string& n, SymbolTable** outtable=nullptr) override;

		virtual function* addFunction(const std::string& n) override;

		virtual variable* findVariable(const std::string& n) override;

		virtual bool isScope() const override;

		std::map<std::string, variable*> vars;  ///< Map of declared member variables.
		std::map<std::string, function*> funcs; ///< Map of declared member functions.
};
