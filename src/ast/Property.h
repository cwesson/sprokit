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
 * ASTNode for `\`` operator.
 * @ingroup ast
 */
class Property : public Expression {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param v Variable pointed to.
		 * @param n Property name.
		 */
		Property(yy::position pos, Variable* v, const char* n) :
			Expression(pos),
			var(v),
			name(n) {}
		
		/**
		 * Destructor.
		 */
		virtual ~Property() {
			if(var != nullptr){
				delete var;
			}
		}
		
		virtual void accept(Visitor& v) override;

		virtual bool is_constexpr() const {
			return true;
		}

		virtual ADT::Type& getType() const {
			if(name == "size"){
				return ADT::Type::findType("uint8");
			}else if(name == "length"){
				return ADT::Type::findType("uint32");
			}else if(name == "max"){
				return var->getType();
			}else if(name == "min"){
				return var->getType();
			}else if(name == "align"){
				return ADT::Type::findType("uint8");
			}
			return ADT::Type::findType("$unknown");
		}

		Variable*   var;  ///< Variable pointed to.
		std::string name; ///< Property name.
};

}
