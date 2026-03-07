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
			ASTNode(yy::position()),
			node(n),
			next(nullptr) {}
		
		/**
		 * Destructor.
		 */
		virtual ~List() {
			delete node;
			if(next != nullptr){
				delete next;
			}
		}
		
		virtual void accept(Visitor& v) override;

		virtual bool allPathsReturn() const override {
			if(node != nullptr && node->allPathsReturn()){
				return true;
			}else if(next != nullptr){
				return next->allPathsReturn();
			}else{
				return false;
			}
		}
		
		ASTNode* node; ///< ASTNode at this position in the list.
		List* next;    ///< Next link in the list.
};

}
