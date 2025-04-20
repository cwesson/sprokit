/**
 * @file Expression.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "dim/Dimensions.h"
#include "Type.h"
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
		 * @param line Line number.
		 */
		Expression(unsigned int line) :
			ASTNode(line),
			dim() {}
		
		/**
		 * Destructor.
		 */
		virtual ~Expression() = default;
		
		virtual void accept(Visitor& v) = 0;

		/**
		 * Check if the expression is compile-time constant.
		 * @return true if expression is compile-time constant, false otherwise.
		 */
		virtual bool is_constexpr() const {
			return false;
		}

		/**
		 * Get the resulting type of the expression
		 * @return ADT type.
		 */
		virtual ADT::Type* getType() const {
			return ADT::Type::findType("$unknown");
		}

		Dimensions dim;   ///< Resulting unit of the operation.
};

}
