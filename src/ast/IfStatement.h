/**
 * @file IfStatement.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Expression.h"

namespace AST {

class IfStatement : public ASTNode {
	public:
		IfStatement(unsigned int line, Expression* c, List* b, ASTNode* e) :
			ASTNode(line),
			condition(c),
			body(b),
			elsebody(e) {}
		
		virtual void accept(Visitor& v) override;
		
		Expression* condition;
		List* body;
		ASTNode* elsebody;
};

}
