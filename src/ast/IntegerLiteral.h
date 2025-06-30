/**
 * @file IntegerLiteral.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include <string>
#include <cmath>

namespace AST {

/**
 * ASTNode for integer literals.
 * @ingroup ast
 */
class IntegerLiteral : public Expression {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param v Integer value.
		 * @param u Integer unit.
		 */
		IntegerLiteral(unsigned int line, unsigned long long v, const char* u) :
			Expression(line),
			value(v),
			unit(u) {}

		/**
		 * Destructor.
		 */
		virtual ~IntegerLiteral() = default;
		
		virtual void accept(Visitor& v) override;

		virtual bool is_constexpr() const override {
			return true;
		}

		virtual ADT::Type& getType() const override {
			if(value < std::pow(2, 8)){
				return ADT::Type::findType("int8");
			}else if(value < std::pow(2, 16)){
				return ADT::Type::findType("int16");
			}else if(value < std::pow(2, 32)){
				return ADT::Type::findType("int32");
			}else{
				return ADT::Type::findType("int64");
			}
		}
		
		unsigned long long value; ///< Integer value.
		std::string unit;         ///< Integer unit.
};

}
