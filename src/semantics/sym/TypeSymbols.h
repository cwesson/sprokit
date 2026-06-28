/**
 * @file TypeSymbols.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "SymbolTable.h"
#include <map>
#include <vector>

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
		~TypeSymbols() override;
		
		virtual std::ostream& print(std::ostream& os, unsigned int depth) const override;

		virtual SymbolTable::variable* addVariable(const std::string& n, SymbolTable** outtable=nullptr) override;

		virtual function* addFunction(const std::string& n) override;

		virtual variable* findVariable(const std::string& n) override;

		/**
		 * Search for the member variable.
		 * @param n Variable name.
		 * @return The variable entry, or nullptr if no variable with the given name was found.
		 */
		virtual variable* findMember(const std::string& n);

		virtual bool isScope() const override;

		std::vector<member*> vars;  ///< List of declared member variables.
		std::map<std::string, function*> funcs; ///< Map of declared member functions.
};
