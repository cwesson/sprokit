/**
 * @file Variable.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include <string>

namespace AST {

class Variable : public Expression {
	public:
		Variable(unsigned int line, const char* n) :
			Expression(line),
			name(n) {}
		
		virtual void accept(Visitor& v) override;
		
		std::string name;
};

}
