/**
 * @file ParameterDeclaration.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include <string>

namespace AST {

class ParameterDeclaration : public ASTNode {
	public:
		ParameterDeclaration(unsigned int line, const char* n, const char* t, const char* u) :
			ASTNode(line),
			name(n),
			type(t),
			unit(u) {}
		
		virtual void accept(Visitor& v) override;
		
		std::string name;
		std::string type;
		std::string unit;
		bool constant;
};

}
