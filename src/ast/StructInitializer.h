/**
 * @file StructInitializer.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include "List.h"

namespace AST {

/**
 * ASTNode for struct initializers.
 * @ingroup ast
 */
class StructInitializer : public Expression {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param l List of member initializations.
		 */
		StructInitializer(yy::position pos, List* l) :
			Expression(pos),
			list(l),
			type(&ADT::Type::findType("$unknown")) {}

		/**type
		 * Destructor.
		 */
		virtual ~StructInitializer() override {
			delete list;
		}
		
		virtual void accept(Visitor& v) override;

		virtual ADT::Type& getType() const override {
			return *type;
		}
		
		List* list;      ///< Initializer list.
		ADT::Type* type; ///< Expected type.
};

}
