/**
 * @file Assignment.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Expression.h"
#include <string>

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
		 * @param n Name on left-hand side.
		 * @param e Right-hand side.
		 */
		Assignment(unsigned int line, const char* n, Expression* e) :
			ASTNode(line),
			name(n),
			expression(e) {}

		/**
		 * Destructor.
		 */
		virtual ~Assignment() {
			delete expression;
		}

		virtual void accept(Visitor& v);

		std::string name;       ///< Left-hand side of assignment.
		Expression* expression; ///< Right-hand side of assignment.
};

}
