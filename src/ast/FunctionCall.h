/**
 * @file FunctionCall.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Variable.h"
#include "List.h"
#include <string>

namespace AST {

/**
 * ASTNode for function calls.
 * @ingroup ast
 */
class FunctionCall : public Variable {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param n Function name.
		 * @param p Function parameters.
		 */
		FunctionCall(unsigned int line, const char* n, List* p) :
			Variable(line, n, nullptr),
			params(p) {}
		
		/**
		 * Destructor.
		 */
		virtual ~FunctionCall() {
			delete params;
		}
		
		virtual void accept(Visitor& v) override;

		virtual ADT::Type& getType() const override {
			auto f = table->findFunction(name);
			if(f != nullptr){
				return *f->type;
			}else{
				return ADT::Type::findType("$unknown");
			}
		}
		
		List* params;     ///< Function parameters.
};

}
