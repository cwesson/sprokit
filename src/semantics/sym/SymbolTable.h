/**
 * @file SymbolTable.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 * 
 * @defgroup sym Symbol Table
 * @{
 */

#pragma once

#include "Expression.h"
#include "Type.h"
#include <iostream>
#include <map>
#include <string>

class FunctionSymbols;

/**
 * Abstract symbol table.
 */
class SymbolTable {
	public:
		/**
		 * Variable declaration entry for symbol tables.
		 * Variables are indexed by name.
		 */
		struct variable {
			ADT::Type* type;  ///< Declared type.
			std::string unit; ///< Declared unit.
			bool constant;    ///< True if declared constant.
			bool pointer;     ///< True if declared as a pointer.
			bool used;        ///< True if the variable is accessed.
			bool modified;    ///< True if the variable is modified.

			/**
			 * Constructor.
			 */
			variable() :
				type(),
				unit(),
				constant(true),
				pointer(false),
				used(false),
				modified(false) {}
		};

		/**
		 * Function parameter entry for symbol tables.
		 * Parameters are indexed by position.
		 */
		struct parameter : variable {
			std::string name; ///< Parameter name.

			/**
			 * Constructor.
			 * @param n Parameter name.
			 */
			parameter(const std::string& n) :
				variable(),
				name(n) {}
		};

		/**
		 * Function declaration entry for symbol tables.
		 * Functions are indexed by name.
		 */
		struct function {
			ADT::Type* type;  ///< Declared return type.
			std::string unit; ///< Declared return unit.
			FunctionSymbols* table; ///< Symbol table for parameters.
			bool pointer;     ///< true if the function returns a pointer.

			/**
			 * Constructor.
			 */
			function() :
				type(),
				unit(),
				table(nullptr) {}
			
			~function();
		};

		/**
		 * Unit declaration entry for symbol tables.
		 * Units are indexed by short name.
		 */
		struct unit {
			std::string alias; ///< Expanded unit name.
			std::map<std::string, AST::Expression*> conversion; ///< Map of unit conversion expressions.
		};

		/**
		 * Constructor.
		 * @param n Symbol table name.
		 * @param p Parent symbol table.
		 */
		SymbolTable(const std::string& n, SymbolTable* p);

		/**
		 * Destructor.
		 */
		virtual ~SymbolTable() = default;

		/**
		 * Print the symbol table.
		 * @param os Output stream to print to.
		 * @param depth Depth in the symbol table tree.
		 * @return Output stream.
		 */
		virtual std::ostream& print(std::ostream& os, unsigned int depth) const;

		/**
		 * Attempt to add a variable declaration.
		 * @param n Variable name.
		 * @return The new variable entry, or nullptr if the table already contains a variable of the given name.
		 */
		virtual variable* addVariable(const std::string& n);

		/**
		 * Attempt to add a function parameter declaration.
		 * @param n Parameter name.
		 * @return The new parameter entry, or nullptr if the table already contains a variable of the given name.
		 */
		virtual parameter* addParameter(const std::string& n);

		/**
		 * Attempt to add a function declaration.
		 * @param n Function name.
		 * @return The new function entry, or nullptr if the table already contains a function of the given name.
		 */
		virtual function* addFunction(const std::string& n);

		/**
		 * Attempt to add a type declaration.
		 * @param n Type name.
		 * @return The new symbol table, or nullptr if the table already contains a type of the given name.
		 */
		virtual SymbolTable* addType(const std::string& n);

		/**
		 * Add a unit declaration.
		 * @param n Unit name.
		 * @return The symbol table entry.
		 * @note The same unit may be declared multiple times.
		 */
		virtual unit* addUnit(const std::string& n);

		/**
		 * Recursively search for the variable.
		 * @param n Variable name.
		 * @return The variable entry, or nullptr if no variable with the given name was found.
		 */
		virtual variable* findVariable(const std::string& n);

		/**
		 * Recursively search for the function.
		 * @param n Function name.
		 * @return The function entry, or nullptr if no function with the given name was found.
		 */
		virtual function* findFunction(const std::string& n);

		/**
		 * Recursively search for the type.
		 * @param n Type name.
		 * @return The type symbol table, or nullptr if no type with the given name was found.
		 */
		virtual SymbolTable* findType(const std::string& n);
		
		SymbolTable* parent; ///< Parent symbol table, nullptr if there is no parent.
		std::string name;    ///< Name of the symbol table.
};

/**
 * Output stream operator, prints the symbol table.
 * @param os Ouput stream to print to.
 * @param table SymbolTable to print.
 * @return Output stream.
 */
std::ostream& operator<<(std::ostream& os, const SymbolTable& table);

/** @} */
