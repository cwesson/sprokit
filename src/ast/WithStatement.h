/**
 * @file WithStatement.h
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
 * ASTNode for `with` statements.
 * @ingroup ast
 */
class WithStatement : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param i `for` initializer.
		 * @param b `for` body.
		 */
		WithStatement(yy::position pos, VariableDeclaration* i, List* b) :
			ASTNode(pos),
			init(i),
			body(b) {}
		
		/**
		 * Destructor.
		 */
		virtual ~WithStatement() {
			delete init;
			delete body;
		}
		
		virtual void accept(Visitor& v) override;
		
		VariableDeclaration* init; ///< `with` initializer.
		List* body;            ///< Body of the `with` statement.
};

}
