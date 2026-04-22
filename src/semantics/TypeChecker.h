/**
 * @file TypeChecker.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 * 
 * @defgroup tc Type Checker
 * @ingroup semantics
 * @{
 */

#pragma once

#include "Visitor.h"
#include "Type.h"

/**
 * Visitor to check type safety.
 * @ingroup semantics
 */
class TypeChecker : public Visitor {
	public:
		/**
		 * Constructor.
		 */
		TypeChecker();

		/**
		 * Destructor.
		 */
		virtual ~TypeChecker() override = default;

		/**
		 * X-macro declaring visit functions for each ASTNode.
		 */
		#define X(_x) virtual void visit(AST::_x& v) override;
		AST_LIST
		#undef X
	
	private:
		ADT::Type* func_type;        ///< Return type when processing function declarations.
		SymbolTable::function* func; ///< Symbol table when processing member accesses.
};

/** @} */
