/**
 * @file FunctionCall.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include "List.h"
#include <string>

namespace AST {

/**
 * ASTNode for function calls.
 * @ingroup ast
 */
class FunctionCall : public Expression {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param n Function name.
		 * @param p Function parameters.
		 */
		FunctionCall(unsigned int line, const char* n, List* p) :
			Expression(line),
			name(n),
			params(p) {}
		
		/**
		 * Destructor.
		 */
		virtual ~FunctionCall() {
			delete params;
		}
		
		virtual void accept(Visitor& v) override;
		
		std::string name; ///< Function name.
		List* params;     ///< Function parameters.
};

}
