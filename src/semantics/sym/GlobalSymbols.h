/**
 * @file GlobalSymbols.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "SymbolTable.h"
#include "TypeSymbols.h"
#include <map>

/**
 * Global symbol table.
 * @ingroup sym
 */
class GlobalSymbols : public SymbolTable {
	public:
		/**
		 * Constructor.
		 */
		GlobalSymbols();

		/**
		 * Destructor.
		 */
		virtual ~GlobalSymbols();
		
		virtual std::ostream& print(std::ostream& os, unsigned int depth) const override;
		
		virtual variable* addVariable(const std::string& n) override;

		virtual function* addFunction(const std::string& n) override;

		virtual SymbolTable* addType(const std::string& n) override;

		virtual unit* addUnit(const std::string& name) override;

		virtual variable* findVariable(const std::string& n) override;

		virtual function* findFunction(const std::string& n) override;

		virtual SymbolTable* findType(const std::string& n) override;

		std::map<std::string, variable*> vars;     ///< Map of declared global variables.
		std::map<std::string, function*> funcs;    ///< Map of declared global functions.
		std::map<std::string, TypeSymbols*> types; ///< Map of declared types.
		std::map<std::string, unit*> units;        ///< Map of declared units.
};
