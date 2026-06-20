/**
 * @file ForStatement.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "VariableDeclaration.h"
#include "Expression.h"
#include "List.h"

namespace AST {

/**
 * ASTNode for `for` statements.
 * @ingroup ast
 */
class ForStatement : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param i `for` initializer.
		 * @param c `for` condition.
		 * @param n `for` increment.
		 * @param b `for` body.
		 */
		ForStatement(yy::position pos, VariableDeclaration* i, Expression* c, Assignment* n, List* b) :
			ASTNode(pos),
			init(i),
			condition(c),
			increment(n),
			body(b) {}
		
		/**
		 * Destructor.
		 */
		virtual ~ForStatement() {
			if(init != nullptr){
				delete init;
			}
			delete condition;
			if(increment != nullptr){
				delete increment;
			}
			delete body;
		}
		
		virtual void accept(Visitor& v) override;

		VariableDeclaration* init; ///< `for` initializer.
		Expression* condition;     ///< `for` condition.
		Assignment* increment;     ///< `for` increment.
		List* body;                ///< Body of the `for` statement.
};

}
