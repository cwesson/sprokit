/**
 * @file PrintAST.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Visitor.h"
#include <iostream>
#include <iomanip>

/**
 * Visitor to print the AST.
 * @ingroup semantics
 */
class PrintAST : public Visitor {
	private:
		/**
		 * Depth of the visitor currently.
		 * @note This is incremented and decremented with every recursive visit.
		 */
		unsigned int indent;

		/**
		 * Print the line number and appropriate indentation.
		 * @param v ASTNode being visited.
		 */
		void printIndent(AST::ASTNode& v);

	public:
		/**
		 * Constructor.
		 */
		PrintAST();

		/**
		 * X-macro declaring visit functions for each ASTNode.
		 */
		#define X(_x) virtual void visit(AST::_x& v) override;
		AST_LIST
		#undef X
};
