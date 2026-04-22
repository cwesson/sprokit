/**
 * @file TypeDecorator.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 * @ingroup adt
 * @{
 */

#pragma once

#include "Type.h"
#include "CodeGen.h"

namespace ADT {

/**
 * Decorator for Types.
 */
class TypeDecorator : public Type {
	public:
		/**
		 * Constructor.
		 * @param t Next type down.
		 * @param dec Decorator string.
		 */
		TypeDecorator(Type& t, const std::string& dec) :
			Type((std::string)t + dec),
			type(t) {}
		
		/**
		 * Destructor.
		 */
		virtual ~TypeDecorator() override = default;

		virtual Type& baseType() override = 0;
		
		Type& type;  ///< Next type down.
};

/**
 * Pointer Types.
 */
class PointerType : public TypeDecorator {
	public:
		/**
		 * Constructor.
		 * @param t Type pointed to.
		 */
		explicit PointerType(Type& t) :
			TypeDecorator(t, "@")
			{}
		
		/**
		 * Destructor.
		 */
		virtual ~PointerType() override = default;

		virtual Type& baseType() override{
			return type.baseType();
		}

		virtual unsigned int size() const override {
			return 8;
		}

		virtual bool accept(const Type& t) const override {
			return type.accept(t);
		}

		virtual std::string translate(const CodeGen& g) const override {
			return g.translateType(*this);
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

		virtual bool visit(const StructType& t) const override {
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

};

/**
 * Array types.
 */
class ArrayType : public TypeDecorator {
	public:
		/**
		 * Constructor.
		 * @param t Type contained in array.
		 * @param len Array length.
		 */
		ArrayType(Type& t, unsigned int len) :
			TypeDecorator(t, std::string("[]")),
			length(len)
			{}
		
		/**
		 * Destructor.
		 */
		virtual ~ArrayType() override = default;
	
		unsigned int length;  ///< Array length.
};

} // namespace ADT

/** @} */
