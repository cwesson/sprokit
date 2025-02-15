/**
 * @file TypeDeclaration.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "List.h"
#include <string>

namespace AST {

class TypeDeclaration : public ASTNode {
	public:
		TypeDeclaration(unsigned int line, const char* n, List* l) :
			ASTNode(line),
			name(n),
			list(l) {}
		
		virtual void accept(Visitor& v) override;
		
		std::string name;
		List* list;
};

}
