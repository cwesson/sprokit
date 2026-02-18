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
		 * @param pos Position in source file.
		 * @param v Variable on left-hand side.
		 * @param e Right-hand side.
		 */
		Assignment(yy::position pos, Variable* v, Expression* e) :
			ASTNode(pos),
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
