/**
 * @file Expression.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "dim/Dimensions.h"
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
			type(),
			dim() {}
		
		/**
		 * Destructor.
		 */
		virtual ~Expression() = default;
		
		virtual void accept(Visitor& v) = 0;
		
		std::string type;     ///< Resulting type of the operation.
		Dimensions dim; ///< Resulting unit of the operation.
};

}
