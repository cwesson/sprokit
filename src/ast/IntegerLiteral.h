/**
 * @file IntegerLiteral.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include <string>

namespace AST {

class IntegerLiteral : public Expression {
	public:
		IntegerLiteral(unsigned int line, unsigned long long v, const char* u) :
			Expression(line),
			value(v),
			unit(u) {}
		
		virtual void accept(Visitor& v) override;
		
		unsigned long long value;
		std::string unit;
};

}
