/**
 * @file Return.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Expression.h"

namespace AST {

class Return : public ASTNode {
	public:
		Return(unsigned int line, Expression* e) :
			ASTNode(line),
			expression(e) {}
		
		virtual void accept(Visitor& v) override;
		
		Expression* expression;
};

}
