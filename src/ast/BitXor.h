/**
 * @file BitXor.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "BinaryOperator.h"

namespace AST {

/**
 * ASTNode for `^|` operator.
 * @ingroup ast
 */
class BitXor : public BinaryOperator {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param l Left-hand side of `^|`.
		 * @param r Right-hand side of `^|`.
		 */
		BitXor(yy::position pos, Expression* l, Expression* r) :
			BinaryOperator(pos, l, r) {}

		virtual void accept(Visitor& v) override;

		virtual ADT::Type& getType() const override {
			unsigned int lsize = left->getType().size();
			unsigned int rsize = right->getType().size();
			if(rsize > lsize){
				return right->getType();
			}else{
				return left->getType();
			}
		}
};

}
