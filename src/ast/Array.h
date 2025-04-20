/**
 * @file Array.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include <string>

namespace AST {

class VariableDeclaration;

/**
 * ASTNode for array access.
 * @ingroup ast
 */
class Array : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param e Array index.
		 * @param a Nested array access
		 */
		Array(unsigned int line, Expression* e, Array* a) :
			ASTNode(line),
			expression(e),
			length_var(nullptr),
			array(a) {}

		/**
		 * Constructor.
		 * @param line Line number.
		 * @param v Array index.
		 * @param a Nested array access
		 */
		Array(unsigned int line, VariableDeclaration* v, Array* a) :
			ASTNode(line),
			expression(nullptr),
			length_var(v),
			array(a) {}

		/**
		 * Constructor.
		 * @param line Line number.
		 * @param a Nested array access
		 */
		Array(unsigned int line, Array* a) :
			ASTNode(line),
			expression(nullptr),
			length_var(nullptr),
			array(a) {}

		/**
		 * Destructor.
		 */
		virtual ~Array() {
			delete expression;
			if(array != nullptr){
				delete array;
			}
		}
		
		virtual void accept(Visitor& v) override;
		
		Expression* expression; ///< Array index.
		VariableDeclaration* length_var; ///< Array length variable declaration.
		Array* array;           ///< Nested array access.
};

}
