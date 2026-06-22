/**
 * @file BoolNot.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include <sstream>

namespace AST {

/**
 * ASTNode for unary `!` operator.
 * @ingroup ast
 */
class BoolNot : public Expression {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param r Right-hand side of `!`.
		 */
		BoolNot(yy::position pos, Expression* r) :
			Expression(pos),
			right(r) {}

		/**
		 * Destructor.
		 */
		virtual ~BoolNot() override {
			delete right;
		}

		virtual void accept(Visitor& v) override;

		virtual bool is_constexpr() const override {
			return right->is_constexpr();
		}

		virtual ADT::Type& getType() const override {
			return ADT::Type::findType("bool");
		}

		Expression* right; ///< Right-hand side of the operator.
};

}
