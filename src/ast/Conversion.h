/**
 * @file Conversion.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Expression.h"
#include <string>

namespace AST {

/**
 * ASTNode for unit conversion operators.
 * @ingroup ast
 */
class Conversion : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param n Name of the variable.
		 * @param t Unit this converts to.
		 * @param e Right-hand side.
		 */
		Conversion(yy::position pos, const char* n, const char* t, Expression* e) :
			ASTNode(pos),
			name(n),
			to(t),
			expression(e) {}

		/**
		 * Destructor.
		 */
		virtual ~Conversion() {
			delete expression;
		}
		
		virtual void accept(Visitor& v);
		
		std::string name;       ///< Name of the variable in the expression.
		std::string to;         ///< Unit this converts to.
		Expression* expression; ///< Conversion expression.
};

}
