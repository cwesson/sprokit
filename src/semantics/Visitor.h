/**
 * @file Visitor.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "AST.h"
#include <string>

/**
 * @defgroup semantics Semantics
 * @{
 */

/**
 * Visitor pattern for processing the AST using double-dispatch.
 */
class Visitor {
	public:
		/**
		 * Constructor.
		 */
		Visitor();

		/**
		 * Destructor.
		 */
		virtual ~Visitor() = default;

		/**
		 * X-macro declaring visit functions for each ASTNode.
		 */
		#define X(_x) virtual void visit(AST::_x& v) = 0;
		AST_LIST
		#undef X

		/**
		 * Print an error to stderr and increment the @ref error_count.
		 * @param node AST node in which the error was found.
		 * @param msg Error message.
		 */
		void printError(AST::ASTNode& node, const std::string& msg);

		/**
		 * Print a warning to stderr.
		 * @param node AST node in which the warning was found.
		 * @param msg Warning message.
		 */
		void printWarning(AST::ASTNode& node, const std::string& msg);

		unsigned int error_count; ///< Count of errors encountered by the visitor.
};

/** @} */
