/**
 * @file EnumValue.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include <string>

namespace AST {

/**
 * ASTNode for enums.
 * @ingroup ast
 */
class EnumValue : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param n Enum value name.
		 * @param e Enum value.
		 */
		EnumValue(yy::position pos, const char* n, Expression* e) :
			ASTNode(pos),
			name(n),
			value(e) {}
		
		/**
		 * Destructor.
		 */
		virtual ~EnumValue() override {
			if(value != nullptr){
				delete value;
			}
		}
		
		virtual void accept(Visitor& v) override;
		
		std::string name;   ///< Enum value name.
		Expression* value;  ///< Enum value.
};

}
