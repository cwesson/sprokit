/**
 * @file ASTNode.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

class Visitor;
class SymbolTable;

/**
 * @defgroup ast Abstract Syntax Tree
 * @{
 */

/**
 * Abstract Syntax Tree
 */
namespace AST {

/**
 * Abstract AST node.
 * @ingroup ast
 */
class ASTNode {
	public:
		/**
		 * Constructor.
		 * @param l Line number.
		 */
		ASTNode(unsigned int l) :
			lineno(l),
			table(nullptr) {}
		
		/**
		 * Destructor.
		 */
		virtual ~ASTNode() {}
		
		/**
		 * Call from semantic Visitor.
		 * @param v Visitor that is calling.
		 * @note This must be implemented in every concrete ASTNode in order for
		 *       the double-dispatch to find to correct callback.
		 * 
		 * @code
		 * void accept(Visitor& v) {
		 *     v.visit(*this);
		 * }
		 * @endcode
		 */
		virtual void accept(Visitor& v) = 0;
		
		unsigned int lineno; ///< Line number.
		SymbolTable* table;  ///< Symbol table at the time this node is executed.
};

}

/** @} */
