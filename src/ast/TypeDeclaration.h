/**
 * @file TypeDeclaration.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "List.h"
#include <string>

namespace AST {

/**
 * ASTNode for `type` declarations.
 * @ingroup ast
 */
class TypeDeclaration : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param n Type name.
		 * @param l List of type member declarations.
		 */
		TypeDeclaration(yy::position pos, const char* n, List* l) :
			ASTNode(pos),
			name(n),
			list(l) {}
		
		/**
		 * Destructor.
		 */
		virtual ~TypeDeclaration() {
			delete list;
		}
		
		virtual void accept(Visitor& v) override;
		
		std::string name; ///< Type name.
		List* list;       ///< Member declarations.
};

}
