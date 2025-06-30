/**
 * @file BoolLiteral.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include <string>

namespace AST {

/**
 * ASTNode for boolean literals.
 * @ingroup ast
 */
class BoolLiteral : public Expression {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param v Boolean value.
		 */
		BoolLiteral(unsigned int line, bool v) :
			Expression(line),
			value(!!v) {}

		/**
		 * Destructor.
		 */
		virtual ~BoolLiteral() = default;
		
		virtual void accept(Visitor& v) override;

		virtual bool is_constexpr() const override {
			return true;
		}

		virtual ADT::Type& getType() const override {
			return ADT::Type::findType("bool");
		}

		bool value; ///< Boolean value.
};

}
