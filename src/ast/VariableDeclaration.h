/**
 * @file VariableDeclaration.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Expression.h"
#include "Array.h"
#include "Type.h"
#include <string>

namespace AST {

/**
 * ASTNode for `const` and `var` declarations.
 * @ingroup ast
 */
class VariableDeclaration : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param n Variable name.
		 * @param t Variable type.
		 * @param u Variable unit.
		 * @param a Array dimensions.
		 */
		VariableDeclaration(yy::position pos, const char* n, ADT::Type& t, const char* u, Array* a) :
			ASTNode(pos),
			name(n),
			type(t),
			unit(u),
			array(a),
			initial(nullptr),
			constant(true) {}
		
		/**
		 * Destructor.
		 */
		virtual ~VariableDeclaration() {
			if(initial != nullptr) {
				delete initial;
			}
		}
		
		virtual void accept(Visitor& v) override;
		
		std::string name;    ///< Variable name.
		ADT::Type& type;     ///< Variable type.
		std::string unit;    ///< Variable unit.
		Array* array;        ///< Array dimensions.
		Expression* initial; ///< Variable initializer.
		bool constant;       ///< true if declared `const`.
};

}
