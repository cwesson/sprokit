/**
 * @file Variable.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include "Array.h"
#include "sym/SymbolTable.h"
#include <string>

namespace AST {

/**
 * ASTNode for variable access.
 * @ingroup ast
 */
class Variable : public Expression {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param n Variable name.
		 * @param a Array access.
		 */
		Variable(unsigned int line, const char* n, Array* a) :
			Expression(line),
			name(n),
			array(a) {}

		/**
		 * Destructor.
		 */
		virtual ~Variable() {
			if(array != nullptr){
				delete array;
			}
		}

		virtual ADT::Type& getType() const override {
			auto sym = table->findVariable(name);
			if(sym != nullptr){
				return *sym->type;
			}else{
				return ADT::Type::findType("$unknown");
			}
		}
		
		virtual void accept(Visitor& v) override;
		
		std::string name; ///< Variable name.
		Array* array;     ///< Array access.
};

}
