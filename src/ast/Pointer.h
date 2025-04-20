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
		 * @param line Line number.
		 * @param v Variable pointed to.
		 */
		Pointer(unsigned int line, Variable* v) :
			Expression(line),
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
