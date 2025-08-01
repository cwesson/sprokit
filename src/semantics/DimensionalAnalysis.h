/**
 * @file DimensionalAnalysis.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 * 
 * @defgroup dim Dimensional Analysis
 * @ingroup semantics
 * @{
 */

#pragma once

#include "Visitor.h"
#include "AST.h"
#include "dim/UnitParser.h"
#include <string>

/**
 * Visitor to perform dimensional analysis.
 * @ingroup dim
 */
class DimensionalAnalysis : public Visitor {
	private:
		Dimensions constructed_unit; ///< Unit dimensions found from recursive visits.
		mutable UnitParser parser;   ///< Parser for unit names.
		AST::FunctionDeclaration* in_func; ///< Function currently being checked.
		Dimensions unit;
		std::string con_symbol;

	public:
		/**
		 * Constructor.
		 */
		DimensionalAnalysis();

		/**
		 * X-macro declaring visit functions for each ASTNode.
		 */
		#define X(_x) virtual void visit(AST::_x& v) override;
		AST_LIST
		#undef X
	
	private:
		bool equal(AST::ASTNode& v, const Dimensions& a, const Dimensions& b) const;
		Dimensions expand(AST::ASTNode& v, const Dimensions& dim) const;
};

/** @} */
