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
		 * @param pos Position in source file.
		 * @param e Return value.
		 */
		Return(yy::position pos, Expression* e) :
			ASTNode(pos),
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
