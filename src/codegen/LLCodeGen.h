/**
 * @file LLCodeGen.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "CodeGen.h"
#include <iostream>
#include <string>

/**
 * @ingroup codegen
 * @{
 */

/**
 * LLVM code generation Visitor.
 */
class LLCodeGen : public CodeGen {
	public:
		/**
		 * Constructor.
		 * @param o Output stream to write code to.
		 */
		LLCodeGen(std::ostream& o);

		/**
		 * Destructor.
		 */
		virtual ~LLCodeGen() = default;

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

	private:
		std::ostream& os; ///< Output stream to write code to.
		unsigned int temp_count; ///< Counter for generating temporaries.
		std::string last_temp;
		bool inparams;
		bool islast;

		std::string getTemporary();
};

/** @} */
