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
		 */
		LLCodeGen(std::ostream& o, const char* filename);

		/**
		 * Destructor.
		 */
		virtual ~LLCodeGen();

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
		llvm::raw_os_ostream os; ///< Output stream to write code to.
		bool inparams;
		bool islast;
		bool collect_values;

		std::unique_ptr<llvm::LLVMContext> context;
		std::unique_ptr<llvm::Module> module;
		std::unique_ptr<llvm::IRBuilder<>> builder;
		std::map<std::string, llvm::Value*> namedValues;
		llvm::Value* last_value;
		mutable llvm::Type* translated_type;
		std::vector<llvm::Type*> arg_types;
		std::vector<std::string> arg_names;
		std::map<std::string, unsigned int> counters;
		std::vector<llvm::Value*> values_list;
		ADT::Type* ret_type;

		unsigned int getCount(const char* name);

		llvm::Value* typePromotion(llvm::Value* val, llvm::Type* type, bool isSigned);

		struct operands {
			llvm::Value* left;
			llvm::Value* right;
		};

		operands visitBinaryOp(AST::Expression* l, AST::Expression* r, ADT::Type& type);
};

/** @} */
