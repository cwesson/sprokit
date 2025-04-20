/**
 * @file UserType.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Type.h"
#include <vector>

/**
 * @ingroup adt
 * @{
 */

namespace ADT {

/**
 * Placeholder Type for user defined types.
 */
class UserType : public Type {
	public:
		virtual bool accept(const Type* t) const override {
			return t->visit(this);
		}

		virtual bool visit(const BoolType* t) const override {
			return false;
		}

		virtual bool visit(const IntType* t) const override {
			return false;
		}

		virtual bool visit(const FloatType* t) const override {
			return false;
		}

		virtual bool visit(const UserType* t) const override {
			return false;
		}

		/**
		 * Add a member to the type.
		 * @param type Type of the member to add.
		 */
		void addMember(ADT::Type* type) {
			members.push_back(type);
		}

		virtual unsigned int size() const override {
			unsigned int s = 0;
			for(auto m : members){
				s += m->size();
			}
			return s;
		}

	protected:
		/**
		 * Constructor.
		 * @param name Type name.
		 */
		UserType(const std::string& name) :
			Type(name)
		{}

		std::vector<ADT::Type*> members; ///< List of members.

		friend class Type;
};

}

/** @} */
