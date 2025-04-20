/**
 * @file PrimitiveType.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Type.h"
#include <sstream>

/**
 * @ingroup adt
 * @{
 */

namespace ADT {

/**
 * Base Type for primitive types.
 */
class PrimitiveType : public Type {
	public:
		virtual bool accept(const Type* t) const = 0;

		virtual bool visit(const BoolType* t) const override = 0;
		virtual bool visit(const IntType* t) const = 0;
		virtual bool visit(const FloatType* t) const = 0;
		virtual bool visit(const UserType* t) const = 0;

	protected:
		/**
		 * Constructor.
		 * @param name Type name.
		 */
		PrimitiveType(const std::string& name) :
			Type(name)
		{}

		friend class Type;
};

/**
 * Boolean type.
 */
class BoolType : public PrimitiveType {
	public:
		virtual bool accept(const Type* t) const override {
			return t->visit(this);
		}

		virtual bool visit(const BoolType* t) const override {
			return true;
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

		virtual unsigned int size() const override {
			return 1;
		}
	
	protected:
		/**
		 * Constructor.
		 */
		BoolType() :
			PrimitiveType("bool")
		{}

		friend class Type;
};

class IntType;

/**
 * Floating-point types.
 * @verbatim
 *    int64  uint64  float64
 *    |    \   |    /    |
 *   int32  uint32--  float32
 *   |    \   |      /   |
 *  int16  uint16----  float16
 *  |    \   |        /
 * int8   uint8-------
 * @endverbatim
 */
class FloatType : public PrimitiveType {
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
			return t->significand >= significand;
		}

		virtual bool visit(const UserType* t) const override {
			return false;
		}

		virtual unsigned int size() const override {
			return (exponent+significand)/8;
		}
	
	protected:
		/**
		 * Constructor.
		 * @param exp Number of bits for the exponent.
		 * @param sig Number of bits for the significand.
		 */
		FloatType(unsigned int exp, unsigned int sig) :
			PrimitiveType((std::stringstream() << "float" << (exp+sig)).str()),
			exponent(exp),
			significand(sig)
		{}

		unsigned int exponent;    ///< Number of bits in the exponent.
		unsigned int significand; ///< Number of bits in the significand.

		friend class Type;
		friend class IntType;
};

/**
 * Integer types.
 * @verbatim
 *    int64  uint64  float64
 *    |    \   |    /    |
 *   int32  uint32--  float32
 *   |    \   |      /   |
 *  int16  uint16----  float16
 *  |    \   |        /
 * int8   uint8-------
 * @endverbatim
 */
class IntType : public PrimitiveType {
	public:
		virtual bool accept(const Type* t) const override {
			return t->visit(this);
		}

		virtual bool visit(const BoolType* t) const override {
			return false;
		}

		virtual bool visit(const IntType* t) const override {
			if(sign && !t->sign){
				// Cannot convert signed to unsigned
				return false;
			}else{
				return t->length >= length;
			}
		}

		virtual bool visit(const FloatType* t) const override {
			return t->significand >= length;
		}

		virtual bool visit(const UserType* t) const override {
			return false;
		}

		virtual unsigned int size() const override {
			return length/8;
		}
	
	protected:
		/**
		 * Constructor.
		 * @param len Number of bits for the integer.
		 * @param sign true if signed, false if unsigned.
		 */
		IntType(unsigned int len, bool sign) :
			PrimitiveType((std::stringstream() << (sign ? "" : "u") << "int" << len).str()),
			length(len),
			sign(sign)
		{}

		unsigned int length; ///< Number of bits in the integer.
		bool sign;           ///< true if signed, false if unsigned.

		friend class Type;
};

}

/** @} */
