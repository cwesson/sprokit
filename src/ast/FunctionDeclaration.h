/**
 * @file FunctionDeclaration.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "List.h"
#include <string>

namespace AST {

/**
 * ASTNode for function declarations.
 * @ingroup ast
 */
class FunctionDeclaration : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param n Function name.
		 * @param t Declared return type.
		 * @param u Declared return unit.
		 * @param b Function body.
		 */
		FunctionDeclaration(unsigned int line, const char* n, const char* t, const char* u, List* b) :
			ASTNode(line),
			name(n),
			type(t),
			unit(u),
			body(b) {}
		
		/**
		 * Destructor.
		 */
		virtual ~FunctionDeclaration() {
			delete params;
			delete body;
		}
		
		virtual void accept(Visitor& v) override;
		
		std::string name; ///< Function name.
		std::string type; ///< Declared return type.
		std::string unit; ///< Declared return unit.
		List* params;     ///< Function parameters.
		List* body;       ///< Function body.
};

}
