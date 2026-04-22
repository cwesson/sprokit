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
 */
class DimensionalAnalysis : public Visitor {
	private:
		Dimensions constructed_unit;       ///< Unit dimensions found from recursive visits.
		mutable UnitParser parser;         ///< Parser for unit names.
		AST::FunctionDeclaration* in_func; ///< Function currently being checked.
		Dimensions unit;                   ///< Dimensions of unit declaration being processed.
		std::string con_symbol;            ///< Unit conversion variable name.

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
		/**
		 * Check if two units are equivalent at the given AST node.
		 * @param v AST node to check at.
		 * @param a,b Units to check.
		 * @return true if the two units are equivalent, false otherwise.
		 */
		bool equal(AST::ASTNode& v, const Dimensions& a, const Dimensions& b) const;

		/**
		 * Expand a unit to its base units.
		 * @param v AST node to check at.
		 * @param dim Unit to expand.
		 * @return Expanded unit.
		 */
		Dimensions expand(AST::ASTNode& v, const Dimensions& dim) const;
};

/** @} */
