/**
 * @file VoidType.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Type.h"

/**
 * @ingroup adt
 * @{
 */

namespace ADT {

/**
 * Placeholder Type for void types.
 */
class VoidType : public Type {
	public:
		virtual bool accept(const Type& t) const override {
			return t.visit(*this);
		}

		virtual std::string translate(const CodeGen& g) const override {
			(void)g;
			return name;
		}

		virtual bool visit(const BoolType& t) const override {
			(void)t;
			return false;
		}

		virtual bool visit(const IntType& t) const override {
			(void)t;
			return false;
		}

		virtual bool visit(const FloatType& t) const override {
			(void)t;
			return false;
		}

		virtual bool isNumeric() const override {
			return true;
		}

		virtual bool visit(const StructType& t) const override {
			(void)t;
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

		virtual bool isStruct() const override {
			return false;
		}

		virtual unsigned int size() const override {
			return 0;
		}

		virtual operator std::string() const override {
			return "void type";
		}

	protected:
		/**
		 * Constructor.
		 */
		VoidType() :
			Type("$void")
		{}

		friend class Type;
};

}

/** @} */
