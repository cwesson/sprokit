/**
 * @file BitNot.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include <sstream>

namespace AST {

/**
 * ASTNode for unary `~` operator.
 * @ingroup ast
 */
class BitNot : public Expression {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param r Right-hand side of `~`.
		 */
		BitNot(yy::position pos, Expression* r) :
			Expression(pos),
			right(r) {}

		/**
		 * Destructor.
		 */
		virtual ~BitNot() override {
			delete right;
		}

		virtual void accept(Visitor& v) override;

		virtual bool is_constexpr() const override {
			return right->is_constexpr();
		}

		virtual ADT::Type& getType() const override {
			return right->getType();
		}

		Expression* right; ///< Right-hand side of the operator.
};

}
