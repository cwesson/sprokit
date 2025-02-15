/**
 * @file Addition.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "BinaryOperator.h"

namespace AST {

/**
 * ASTNode for `+` operator.
 * @ingroup ast
 */
class Addition : public BinaryOperator {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param l Left-hand side of `+`.
		 * @param r Right-hand side of `+`.
		 */
		Addition(unsigned int line, Expression* l, Expression* r) :
			BinaryOperator(line, l, r) {}

		virtual void accept(Visitor& v) override;
};

}
