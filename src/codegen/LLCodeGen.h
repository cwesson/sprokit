/**
 * @file LLCodeGen.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "CodeGen.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>
#include <iostream>
#include <memory>
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
		 * @param filename Name of the spkt file being parsed.
		 */
		LLCodeGen(std::ostream& o, const char* filename);

		/**
		 * Destructor.
		 */
		virtual ~LLCodeGen() override;

		/**
		 * X-macro declaring visit functions for each ASTNode.
		 */
		#define X(_x) virtual void visit(AST::_x& v) override;
		AST_LIST
		#undef X

		virtual std::string translateType(const ADT::BoolType& t) const override;
		virtual std::string translateType(const ADT::IntType& t) const override;
		virtual std::string translateType(const ADT::FloatType& t) const override;
		virtual std::string translateType(const ADT::StructType& t) const override;
		virtual std::string translateType(const ADT::PointerType& t) const override;

	private:
		llvm::raw_os_ostream os; ///< Output stream to write code to.
		bool inparams;           ///< true if processing function parameters.
		bool islast;             ///< true if processing last element of a List.
		bool collect_values;     ///< true to collect llvm::Value in values_list.

		std::unique_ptr<llvm::LLVMContext> context;      ///< LLVM context.
		std::unique_ptr<llvm::Module> module;            ///< LLVM module.
		std::unique_ptr<llvm::IRBuilder<>> builder;      ///< LLVM builder.
		std::map<std::string, llvm::Value*> namedValues; ///< Map of names values.
		llvm::Value* last_value;                         ///< Previous generated value.
		mutable llvm::Type* translated_type;             ///< LLVM type from translateType.
		std::vector<llvm::Type*> arg_types;              ///< Function argument types, must match arg_names.
		std::vector<std::string> arg_names;              ///< Function argument names, must match arg_types.
		std::map<std::string, unsigned int> counters;    ///< Counters for getCount.
		std::vector<llvm::Value*> values_list;           ///< Collected values.
		ADT::Type* ret_type;                             ///< Return type of function being processed.

		/**
		 * Get a counter of the given name.  The counter is incremented each
		 * time the function is called with a given name.
		 * @param name Named counter.
		 * @return Current count for the given name.
		 */
		unsigned int getCount(const char* name);

		/**
		 * Generate a type promotion.
		 * @param val Original value.
		 * @param type Required type.
		 * @param isSigned true if type is signed, false otherwise.
		 * @return Type promotion value.
		 */
		llvm::Value* typePromotion(llvm::Value* val, llvm::Type* type, bool isSigned);

		/**
		 *  Operands for binary operators.
		 */
		struct operands {
			llvm::Value* left;   ///< Left-hand side.
			llvm::Value* right;  ///< Right-hand side.
		};

		/**
		 * Visit a binary operator, and perform appropriate type promotions.
		 * @param l Left-hand side of the operator.
		 * @param r Right-hand side of the operator.
		 * @param type Expected type of the result.
		 * @return Type-promoted operands for the operator.
		 */
		operands visitBinaryOp(AST::Expression* l, AST::Expression* r, ADT::Type& type);
};

/** @} */
