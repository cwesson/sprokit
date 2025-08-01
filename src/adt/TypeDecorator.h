/**
 * @file TypeDecorator.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 * @ingroup adt Abstract Data Types
 * @{
 */

#pragma once

#include "Type.h"
#include "CodeGen.h"

namespace ADT {

class TypeDecorator : public Type {
	public:
		TypeDecorator(Type& t, const std::string& dec) :
			Type((std::string)t + dec),
			type(t) {}
		
		virtual ~TypeDecorator() = default;
		
		Type& type;
};

class PointerType : public TypeDecorator {
	public:
		PointerType(Type& t) :
			TypeDecorator(t, "@")
			{}
		
		virtual ~PointerType() = default;

		virtual unsigned int size() const override {
			return 8;
		}

		virtual bool accept(const Type& t) const override {
			return type.accept(*this);
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

};

class ArrayType : public TypeDecorator {
	public:
		ArrayType(Type& t, unsigned int len) :
			TypeDecorator(t, std::string("[]"))
			{}
		
			virtual ~ArrayType() = default;
};

} // namespace ADT

/** @} */
