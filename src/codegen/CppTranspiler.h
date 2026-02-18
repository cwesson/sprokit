/**
 * @file CppTranspiler.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
*/

#pragma once

#include "CodeGen.h"
#include <functional>
#include <iostream>

/**
 * @defgroup codegen Code Generation
 * @{
 */

/**
 * C++ Transpiler Visitor.
 */
class CppTranspiler : public CodeGen {
public:
		/**
		 * Constructor.
		 * @param o Output stream to write code to.
		 */
		CppTranspiler(std::ostream& o);

		/**
		 * Destructor.
		 */
		virtual ~CppTranspiler() = default;

		/**
		 * X-macro declaring visit functions for each ASTNode.
		 */
		#define X(_x) virtual void visit(AST::_x& v) override;
		AST_LIST
		#undef X

		virtual std::string translateType(const ADT::BoolType& t) const override;
		virtual std::string translateType(const ADT::IntType& t) const override;
		virtual std::string translateType(const ADT::FloatType& t) const override;
		virtual std::string translateType(const ADT::UserType& t) const override;
		virtual std::string translateType(const ADT::PointerType& t) const override;

		/**
		 * Helper class to handle indentation in C++ output.
		 */
		class Indent {
			public:
				/**
				 * Constructor.
				 */
				Indent() :
					value(0) {}
				
				/**
				 * Output stream operator.
				 * Prints indentation to the stream.
				 * @param o Output stream.
				 * @param indent Indentation to print.
				 * @return Output stream.
				 */
				friend std::ostream& operator<<(std::ostream& o, Indent indent);

				/**
				 * Increase indentation.
				 */
				void operator++(){
					++value;
				}

				/**
				 * Decrease indentation.
				 */
				void operator--(){
					--value;
				}
			
				private:
					unsigned int value; ///< Current amount of indentation.
		};

	private:
		std::ostream& os;    ///< Output stream to write code to.
		Indent indent;       ///< Amount of indentation.
		bool in_params;      ///< true if processing parameter declarations.
		bool is_last;        ///< true if processing the last element of a List.
		bool is_member;      ///< true if processing member accesses.
		unsigned int array_depth; ///< Number of array indicies deep.
		mutable unsigned int ptr_count;   ///< Number of pointer dereferences.
		/// Function to insert code before the last member access in a chain.
		std::function<std::ostream&(std::ostream&)> insert_last;

		/**
		 * Format unit strings to a C++ comment.
		 * @param u Unit string to format.
		 * @return Comment string or empty string.
		 */
		std::string formatUnit(std::string u);

		/**
		 * Print the arrlen variable prefix.
		 * @param os Stream to print to.
		 * @return The os stream.
		 */
		std::ostream& arrlen(std::ostream& os);
};

/** @} */
