/**
 * @file FloatLiteral.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include <string>
#include <cmath>

namespace AST {

/**
 * ASTNode for Floating point literals.
 * @ingroup ast
 */
class FloatLiteral : public Expression {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param v Floating point value.
		 * @param u Floating point unit.
		 */
		FloatLiteral(yy::position pos, double v, const char* u) :
			Expression(pos),
			value(v),
			unit(u) {}

		/**
		 * Destructor.
		 */
		virtual ~FloatLiteral() = default;
		
		virtual void accept(Visitor& v) override;

		virtual bool is_constexpr() const override {
			return true;
		}

		virtual ADT::Type& getType() const override {
			return ADT::Type::findType("float64");
		}
		
		double value;      ///< Floating point value.
		std::string unit;  ///< Floating point unit.
};

}
