/**
 * @file LLCodeGen.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Visitor.h"
#include <string>

/**
 * @defgroup codegen Code Generation
 * @{
 */

class CodeGen : public Visitor {
	public:
		CodeGen() = default;
		virtual ~CodeGen() = default;

		virtual std::string translateType(const ADT::BoolType& t) const = 0;
		virtual std::string translateType(const ADT::IntType& t) const = 0;
		virtual std::string translateType(const ADT::FloatType& t) const = 0;
		virtual std::string translateType(const ADT::UserType& t) const = 0;
		virtual std::string translateType(const ADT::PointerType& t) const = 0;
};

/** @} */
