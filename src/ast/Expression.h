/**
 * @file Expression.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "dim/Dimensions.h"
#include "Type.h"
#include "Comptime.h"
#include <string>

namespace AST {

/**
 * Abstract ASTNode for expressions.
 * @ingroup ast
 */
class Expression : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 */
		Expression(yy::position pos) :
			ASTNode(pos),
			dim() {}
		
		/**
		 * Destructor.
		 */
		virtual ~Expression() override = default;
		
		virtual void accept(Visitor& v) override = 0;

		/**
		 * Check if the expression is compile-time constant.
		 * @return true if expression is compile-time constant, false otherwise.
		 */
		virtual bool is_constexpr() const {
			return false;
		}

		/// Type for compile-time evaluation.
		typedef CompTime<uint64_t, int64_t, double> comptime_eval_t;

		/**
		 * Evaluate an expression at compile-time.
		 * @return Value of the expression.
		 */
		virtual comptime_eval_t eval() const {
			return comptime_eval_t{};
		}

		/**
		 * Get the resulting type of the expression
		 * @return ADT type.
		 */
		virtual ADT::Type& getType() const {
			return ADT::Type::findType("$unknown");
		}

		Dimensions dim;   ///< Resulting unit of the operation.
};

}
