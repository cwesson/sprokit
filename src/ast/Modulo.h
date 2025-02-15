/**
 * @file Modulo.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "BinaryOperator.h"

namespace AST {

class Modulo : public BinaryOperator {
	public:
		Modulo(unsigned int line, Expression* l, Expression* r) :
			BinaryOperator(line, l, r) {}
		
		virtual void accept(Visitor& v) override;
};

}
