/**
 * @file Pointer.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Variable.h"

namespace AST {

/**
 * ASTNode for `@` operator.
 * @ingroup ast
 */
class Pointer : public Expression {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param v Variable pointed to.
		 */
		Pointer(yy::position pos, Variable* v) :
			Expression(pos),
			var(v) {}
		
		/**
		 * Destructor.
		 */
		virtual ~Pointer() {
			if(var != nullptr){
				delete var;
			}
		}
		
		virtual void accept(Visitor& v) override;
		
		Variable* var; ///< Variable pointed to.
};

}
