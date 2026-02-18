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
		 * @param pos Position in source file.
		 * @param l Left-hand side of `==`.
		 * @param r Right-hand side of `==`.
		 */
		Equal(yy::position pos, Expression* l, Expression* r) :
			BinaryOperator(pos, l, r) {}

		virtual ADT::Type& getType() const override {
			return ADT::Type::findType("bool");
		}
		
		virtual void accept(Visitor& v);
};

}
