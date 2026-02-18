/**
 * @file Subtraction.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "BinaryOperator.h"

namespace AST {

/**
 * ASTNode for binary `-` operator.
 * @ingroup ast
 */
class Subtraction : public BinaryOperator {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param l Left-hand side of `-`.
		 * @param r Right-hand side of `-`.
		 */
		Subtraction(yy::position pos, Expression* l, Expression* r) :
			BinaryOperator(pos, l, r) {}
		
		virtual void accept(Visitor& v) override;
};

}
