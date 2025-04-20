/**
 * @file Member.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Variable.h"
#include "Array.h"
#include <string>

namespace AST {

/**
 * ASTNode for variable member access.
 * @ingroup ast
 */
class Member : public Variable {
	public:
		/**
		 * Constructor.
		 * @param line Line number.
		 * @param l Left-hand side of `.`.
		 * @param r Right-hand side of `.`.
		 */
		Member(unsigned int line, Variable* l, Variable* r) :
			Variable(line, "$member", nullptr),
			left(l),
			right(r) {}

		/**
		 * Destructor.
		 */
		virtual ~Member() {
			if(array != nullptr){
				delete array;
			}
		}
		
		virtual void accept(Visitor& v) override;

		/**
		 * Determine if this is the last member access is a chain of member accesses.
		 * @return true If this is the last member access, false otherwise.
		 */
		bool isLast() {
			return right->name != "$member";
		}
		
		Variable* left;  ///< Left side of the member.
		Variable* right; ///< Right side of the member.
};

}
