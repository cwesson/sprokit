/**
 * @file List.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"

namespace AST {

/**
 * Linked list of ASTNode.
 */
class List : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param n ASTNode at this position in the list.
		 */
		List(ASTNode* n) :
			ASTNode(0),
			node(n),
			next(nullptr) {}
		
		virtual ~List() {
			delete node;
			if(next != nullptr){
				delete next;
			}
		}
		
		virtual void accept(Visitor& v) override;
		
		ASTNode* node;
		List* next;
};

}
