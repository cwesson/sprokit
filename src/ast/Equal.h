/**
 * @file Equal.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "BinaryOperator.h"

namespace AST {

/**
 * ASTNode for `==` operator.
 * @ingroup ast
 */
class Equal : public BinaryOperator {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param l Left-hand side of `==`.
		 * @param r Right-hand side of `==`.
		 */
		Equal(unsigned int line, Expression* l, Expression* r) :
			BinaryOperator(line, l, r) {
				type = "bool";
			}
		
		virtual void accept(Visitor& v);
};

}
