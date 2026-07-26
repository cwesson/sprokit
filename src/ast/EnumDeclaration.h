/**
 * @file EnumDeclaration.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "List.h"
#include <string>

namespace AST {

/**
 * ASTNode for enum `type` declarations.
 * @ingroup ast
 */
class EnumDeclaration : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param n Type name.
		 * @param l List of type member declarations.
		 */
		EnumDeclaration(yy::position pos, const char* n, List* l) :
			ASTNode(pos),
			name(n),
			list(l) {}
		
		/**
		 * Destructor.
		 */
		virtual ~EnumDeclaration() override {
			delete list;
		}
		
		virtual void accept(Visitor& v) override;
		
		std::string name; ///< Type name.
		List* list;       ///< Member declarations.
};

}
