/**
 * @file Assignment.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Expression.h"
#include "Variable.h"

namespace AST {

/**
 * ASTNode for `=` operator.
 * @ingroup ast
 */
class Assignment : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param v Variable on left-hand side.
		 * @param e Right-hand side.
		 */
		Assignment(unsigned int line, Variable* v, Expression* e) :
			ASTNode(line),
			var(v),
			expression(e) {}

		/**
		 * Destructor.
		 */
		virtual ~Assignment() {
			delete var;
			delete expression;
		}

		virtual void accept(Visitor& v);

		Variable* var;          ///< Left-hand side of assignment.
		Expression* expression; ///< Right-hand side of assignment.
};

}
