/**
 * @file AST.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 * @brief Short-hand header that includes all ASTNode implementations.
 * @ingroup ast
 */

#pragma once

/**
 * X-macro list of concrete ASTNode types.
 */
#define AST_LIST \
	X(Addition) \
	X(Array) \
	X(Assignment) \
	X(BoolLiteral) \
	X(Conversion) \
	X(Division) \
	X(Equal) \
	X(Exponent) \
	X(FloatLiteral) \
	X(FunctionCall) \
	X(FunctionDeclaration) \
	X(IfStatement) \
	X(IntegerLiteral) \
	X(List) \
	X(Member) \
	X(Modulo) \
	X(Multiplication) \
	X(NotEqual) \
	X(Pointer) \
	X(Property) \
	X(Return) \
	X(Subtraction) \
	X(TypeDeclaration) \
	X(UnitDeclaration) \
	X(Variable) \
	X(VariableDeclaration) \

#include "Addition.h"
#include "Array.h"
#include "Assignment.h"
#include "BoolLiteral.h"
#include "Conversion.h"
#include "Division.h"
#include "Equal.h"
#include "Exponent.h"
#include "FloatLiteral.h"
#include "FunctionCall.h"
#include "FunctionDeclaration.h"
#include "IfStatement.h"
#include "IntegerLiteral.h"
#include "List.h"
#include "Member.h"
#include "Modulo.h"
#include "Multiplication.h"
#include "NotEqual.h"
#include "Pointer.h"
#include "Property.h"
#include "Return.h"
#include "Subtraction.h"
#include "TypeDeclaration.h"
#include "UnitDeclaration.h"
#include "Variable.h"
#include "VariableDeclaration.h"
