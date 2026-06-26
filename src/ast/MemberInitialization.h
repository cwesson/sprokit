/**
 * @file MemberInitialization.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "ASTNode.h"
#include "Expression.h"
#include "Array.h"
#include "Type.h"
#include <string>

namespace AST {

/**
 * ASTNode for struct member initialization.
 * @ingroup ast
 */
class MemberInitialization : public ASTNode {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param n Variable name.
		 * @param i Initializer value.
		 */
		MemberInitialization(yy::position pos, const char* n, Expression* i) :
			ASTNode(pos),
			name(n),
			initial(i) {}
		
		/**
		 * Destructor.
		 */
		virtual ~MemberInitialization() override {
			if(initial != nullptr) {
				delete initial;
			}
		}
		
		virtual void accept(Visitor& v) override;
		
		std::string name;    ///< Variable name.
		Expression* initial; ///< Variable initializer.
};

}
