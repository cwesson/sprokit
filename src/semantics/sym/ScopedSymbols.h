/**
 * @file ScopedSymbols.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "OrderedSymbol.h"

/**
 * Nested scope symbol table.
 * @ingroup sym
 */
class ScopedSymbols : public OrderedSymbol {
	public:
		/**
		 * Constructor.
		 * @param n Table name. 
		 * @param p Parent symbol table.
		 */
		ScopedSymbols(const std::string& n, SymbolTable* p);

		/**
		 * Destructor.
		 */
		virtual ~ScopedSymbols() = default;
		
		virtual std::ostream& print(std::ostream& os, unsigned int depth) const override;

		virtual bool isScope() const override;
};
