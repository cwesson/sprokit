/**
 * @file UnknownType.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Type.h"

/**
 * @ingroup adt
 * @{
 */

namespace ADT {

/**
 * Placeholder Type for unknown types.
 */
class UnknownType : public Type {
	public:
		virtual bool accept(const Type& t) const override {
			return t.visit(*this);
		}

		virtual std::string translate(const CodeGen& g) const override {
			return name;
		}

		virtual bool visit(const BoolType& t) const override {
			return false;
		}

		virtual bool visit(const IntType& t) const override {
			return false;
		}

		virtual bool visit(const FloatType& t) const override {
			return false;
		}

		virtual bool visit(const UserType& t) const override {
			return false;
		}

		virtual bool isSigned() const override {
			return false;
		}

		virtual bool isInt() const override {
			return false;
		}

		virtual bool isFloat() const override {
			return false;
		}

		virtual unsigned int size() const override {
			return 0;
		}

		virtual operator std::string() const override {
			return "unknown type";
		}

	protected:
		/**
		 * Constructor.
		 */
		UnknownType() :
			Type("$unknown")
		{}

		friend class Type;
};

}

/** @} */
