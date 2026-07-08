/**
 * @file VariableStatement.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Variable.h"
#include "Array.h"
#include "sym/SymbolTable.h"
#include <string>

namespace AST {

/**
 * ASTNode for variable statements.
 * @ingroup ast
 */
class VariableStatement : public Expression {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param n Variable to access.
		 */
		VariableStatement(yy::position pos, Variable* n) :
			Expression(pos),
			var(n) {}

		/**
		 * Destructor.
		 */
		virtual ~VariableStatement() override {
			if(var != nullptr){
				delete var;
			}
		}

		virtual ADT::Type& getType() const override {
			if(var != nullptr){
				return var->getType();
			}else{
				return ADT::Type::findType("$unknown");
			}
		}
		
		virtual void accept(Visitor& v) override;
		
		Variable* var; ///< Variable to load.
};

}
