/**
 * @file UnitDeclaration.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "List.h"
#include <string>

namespace AST {

class List;

class UnitDeclaration : public ASTNode {
	public:
		UnitDeclaration(unsigned int line, const char* u, List* l) :
			ASTNode(line),
			unit(u),
			list(l) {}
		
		virtual void accept(Visitor& v) override;
		
		std::string unit;
		List* list;
};

}
