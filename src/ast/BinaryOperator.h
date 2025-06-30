/**
 * @file BinaryOperator.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"

namespace AST {

/**
 * Abstract ASTNode for binary operators.
 * @ingroup ast
 */
class BinaryOperator : public Expression {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param l Left-hand side of the operator.
		 * @param r Right-hand side of the operator.
		 */
		BinaryOperator(unsigned int line, Expression* l, Expression* r) :
			Expression(line),
			left(l),
			right(r) {}

		/**
		 * Destructor.
		 */
		virtual ~BinaryOperator() {
			delete left;
			delete right;
		}

		virtual void accept(Visitor& v) override = 0;

		virtual bool is_constexpr() const override {
			return left->is_constexpr() && right->is_constexpr();
		}

		virtual ADT::Type& getType() const override {
			ADT::Type& ltype = left->getType();
			ADT::Type& rtype = right->getType();
			if(rtype.convertibleTo(ltype)){
				return ltype;
			}else if(ltype.convertibleTo(rtype)){
				return rtype;
			}else{
				return Expression::getType();
			}
		}

		Expression* left;  ///< Left-hand side of the operator.
		Expression* right; ///< Right-hand side of the operator.
};

}
