/**
 * @file Return.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Expression.h"

namespace AST {

/**
 * ASTNode for `return` statements.
 * @ingroup ast
 */
class Return : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param e Return value.
		 */
		Return(unsigned int line, Expression* e) :
			ASTNode(line),
			expression(e) {}
		
		/**
		 * Destructor.
		 */
		virtual ~Return() {
			if(expression != nullptr){
				delete expression;
			}
		}
		
		virtual void accept(Visitor& v) override;
		
		Expression* expression; ///< Return value.
};

}
