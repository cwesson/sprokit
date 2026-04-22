/**
 * @file Type.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 * @defgroup adt Abstract Data Types
 * @{
 */

#pragma once

#include <map>
#include <string>

class CodeGen;

/**
 * Abstract Data Types.
 */
namespace ADT {

class UnknownType;
class StructType;
class BoolType;
class FloatType;
class IntType;
class PointerType;

/**
 * Base class for ADT.
 */
class Type {
	public:
		/**
		 * Look up the specified type.
		 * @param type Type name.
		 * @return Type object.
		 */
		static Type& findType(const std::string& type);

		/**
		 * Look up the specified type.
		 * @param type Type name.
		 * @return Type object.
		 */
		static Type& findPointerType(const std::string& type);

		/**
		 * Create a new StructType.
		 * @param type Type name.
		 * @return Type object, or nullptr if the type already exists.
		 */
		static StructType* createType(const std::string& type);

		/**
		 * Get the base type.
		 * @return Reference to the base type.
		 */
		virtual Type& baseType() {
			return *this;
		}

		/**
		 * Determine if this Type can be converted to another type.
		 * @param other	Type to convert to.
		 * @return true if this Type can be converted to other.
		 */
		bool convertibleTo(const Type& other) const {
			return other.accept(*this);
		}

		/**
		 * Check if types are not equivalent.
		 * @param other Other type to check.
		 * @return true if the two types are not equivalent, false otherwise.
		 */
		bool operator!=(const Type& other) const {
			return &other != this;
		}

		/**
		 * Get the size of the type.
		 * @return Size in bytes.
		 */
		virtual unsigned int size() const = 0;

		/**
		 * Visitor pattern to get derived Type for convertibleTo.
		 * @warning Do not call this directly, call @ref convertibleTo().
		 * @param t Type being converted from.
		 * @return true if t is convertible to this Type.
		 */
		virtual bool accept(const Type& t) const = 0;

		/**
		 * Visitor pattern to allow code generation to generate the type name.
		 * @param g Code generation visitor.
		 * @return Type name to be use by the code generator.
		 */
		virtual std::string translate(const CodeGen& g) const = 0;

		/**
		 * Visitor pattern, check is UnknownType is convertible to this.
		 * @warning Do not call this directly, call @ref convertibleTo().
		 * @param t Type being converted to.
		 * @return true if this Type is convertible to UnknownType.
		 */
		virtual bool visit(const UnknownType& t) const final {
			return false;
		}

		/**
		 * Visitor pattern, check if BoolType is convertible to this.
		 * @warning Do not call this directly, call @ref convertibleTo().
		 * @param t Type being converted to.
		 * @return true if this Type is convertible to BoolType.
		 */
		virtual bool visit(const BoolType& t) const = 0;

		/**
		 * Visitor pattern, check if IntType is convertible to this.
		 * @warning Do not call this directly, call @ref convertibleTo().
		 * @param t Type being converted to.
		 * @return true if this Type is convertible to IntType.
		 */
		virtual bool visit(const IntType& t) const = 0;

		/**
		 * Visitor pattern, check if FloatType is convertible to this.
		 * @warning Do not call this directly, call @ref convertibleTo().
		 * @param t Type being converted to.
		 * @return true if this Type is convertible to FloatType.
		 */
		virtual bool visit(const FloatType& t) const = 0;

		/**
		 * Visitor pattern, check if StructType is convertible to this.
		 * @warning Do not call this directly, call @ref convertibleTo().
		 * @param t Type being converted to.
		 * @return true if this Type is convertible to UserTpe.
		 */
		virtual bool visit(const StructType& t) const = 0;

		/**
		 * Check if the type can hold a signed value.
		 * @return true if the type can hold a signed value, false otherwise.
		 */
		virtual bool isSigned() const = 0;

		/**
		 * Check if the type holds an integer value.
		 * @return true if the type holds an integer value, false otherwise.
		 */
		virtual bool isInt() const = 0;

		/**
		 * Check if the type holds a floating-point value.
		 * @return true if the type holds a floating-point value, false otherwise.
		 */
		virtual bool isFloat() const = 0;

		/**
		 * Get the string name.
		 * @return String rname of the type.
		 */
		virtual operator std::string() const;
		
		/**
		 * Table to hold types.
		 */
		class TypeTable {
			public:
				/**
				 * Constructor.
				 */
				TypeTable();

				/**
				 * Destructor.
				 */
				~TypeTable();

				/**
				 * Add a type to the table.
				 * @tparam TYPE Type to construct.
				 * @tparam ARGS Arguments for TYPE constructor.
				 * @param args Arguments for TYPE constructor.
				 */
				template<typename TYPE, typename... ARGS>
				void insert(ARGS... args);

				/**
				 * Check if the table is empty.
				 * @return true if the table is empty, false otherwise.
				 */
				bool empty();

				/**
				 * Check if the table contains the specified type.
				 * @param name Type name to check.
				 * @return true if the table contains the specified type, false otherwise.
				 */
				bool contains(const std::string& name);

				/**
				 * Get the specified type.
				 * @param name Type name ot get.
				 * @return Type or nullptr if the type does not exist.
				 */
				Type* at(const std::string& name);
			private:
				std::map<std::string, Type*> table; ///< Map of names to Type objects.
		};
	
	protected:
		/**
		 * Constructor.
		 * @param n Type name.
		 */
		Type(const std::string& n);

		/**
		 * Destructor.
		 */
		virtual ~Type() = default;
	
		std::string name; ///< Type name.
	
	private:
		static TypeTable table; ///< Table of known types.

		/**
		 * Add a type to the table.
		 * @param type Type to add.
		 */
		static void add(Type* type);

		/**
		 * Initialize static table.
		 */
		static void init();
};

}

/** @} */
