/**
 * @file IfStatement.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Expression.h"

namespace AST {

/**
 * ASTNode for `if` statements.
 * @ingroup ast
 */
class IfStatement : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param c `if` condition.
		 * @param b `if` body.
		 * @param e `else` body.
		 */
		IfStatement(unsigned int line, Expression* c, List* b, List* e) :
			ASTNode(line),
			condition(c),
			body(b),
			elsebody(e) {}
		
		/**
		 * Destructor.
		 */
		virtual ~IfStatement() {
			delete condition;
			delete body;
			if(elsebody != nullptr){
				delete elsebody;
			}
		}
		
		virtual void accept(Visitor& v) override;
		
		Expression* condition; ///< `if` condition.
		List* body;            ///< Body of the `if` statement.
		List* elsebody;        ///< Body of the `else`.
};

}
