/**
 * @file UnitDeclaration.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "List.h"
#include <string>

namespace AST {

/**
 * ASTNode for `unit` declarations.
 * @ingroup ast
 */
class UnitDeclaration : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param n Unit name.
		 * @param l List of unit member declarations.
		 * @param a Long-form unit name.
		 */
		UnitDeclaration(yy::position pos, const char* n, List* l, const char* a) :
			ASTNode(pos),
			unit(n),
			alias(a),
			list(l) {}
		
		/**
		 * Destructor.
		 */
		virtual ~UnitDeclaration() {
			delete list;
		}
		
		virtual void accept(Visitor& v) override;
		
		std::string unit;  ///< Unit name.
		std::string alias; ///< Long-form unit name.
		List* list;        ///< Member declarations.
};

}
