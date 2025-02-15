/**
 * @file VariableDeclaration.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Expression.h"
#include <string>

namespace AST {

class VariableDeclaration : public ASTNode {
	public:
		VariableDeclaration(unsigned int line, const char* n, const char* t, const char* u, Expression* i) :
			ASTNode(line),
			name(n),
			type(t),
			unit(u),
			initial(i) {}
		
		virtual void accept(Visitor& v) override;
		
		std::string name;
		std::string type;
		std::string unit;
		Expression* initial;
		bool constant;
};

}
