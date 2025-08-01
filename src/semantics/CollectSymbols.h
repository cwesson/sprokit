/**
 * @file CollectSymbols.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Visitor.h"
#include "sym/SymbolTable.h"

/**
 * Visitor to collect all symbols.
 * @ingroup semantics
 */
class CollectSymbols : public Visitor {
	public:
		SymbolTable* table;       ///< Current symbol table.
		SymbolTable* type_table;  ///< Symbol table when processing member accesses.
		ADT::UserType* user_type; ///< ADT type when processing member declarations.
		SymbolTable::unit* unit;
		std::string con_symbol;

	private:
		/**
		 * Ensure that there is a symbol table.
		 * @param v ASTNode being visited.
		 */
		void prepTable(AST::ASTNode& v);

		/**
		 * Replace the current symbol table with its parent.
		 */
		void removeTable();

		bool collect_param; ///< Set to true when visiting parameter declarations.

	public:
		/**
		 * Constructor.
		 */
		CollectSymbols();

		/**
		 * Destructor.
		 */
		virtual ~CollectSymbols();

		/**
		 * X-macro declaring visit functions for each ASTNode.
		 */
		#define X(_x) virtual void visit(AST::_x& v) override;
		AST_LIST
		#undef X
};
