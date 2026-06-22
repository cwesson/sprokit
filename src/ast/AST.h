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
	X(BoolAnd) \
	X(BoolLiteral) \
	X(BoolNot) \
	X(BoolOr) \
	X(Conversion) \
	X(Division) \
	X(Equal) \
	X(Exponent) \
	X(FloatLiteral) \
	X(ForStatement) \
	X(FunctionCall) \
	X(FunctionDeclaration) \
	X(GreaterEqual) \
	X(GreaterThan) \
	X(IfStatement) \
	X(LessEqual) \
	X(LessThan) \
	X(IntegerLiteral) \
	X(List) \
	X(Member) \
	X(Modulo) \
	X(Multiplication) \
	X(Negation) \
	X(NotEqual) \
	X(Pointer) \
	X(Property) \
	X(Return) \
	X(Subtraction) \
	X(TypeDeclaration) \
	X(UnitDeclaration) \
	X(Variable) \
	X(VariableDeclaration) \
	X(WithStatement) \

#include "Addition.h"
#include "Array.h"
#include "Assignment.h"
#include "BoolAnd.h"
#include "BoolLiteral.h"
#include "BoolNot.h"
#include "BoolOr.h"
#include "Conversion.h"
#include "Division.h"
#include "Equal.h"
#include "Exponent.h"
#include "FloatLiteral.h"
#include "ForStatement.h"
#include "FunctionCall.h"
#include "FunctionDeclaration.h"
#include "GreaterEqual.h"
#include "GreaterThan.h"
#include "IfStatement.h"
#include "IntegerLiteral.h"
#include "LessEqual.h"
#include "LessThan.h"
#include "List.h"
#include "Member.h"
#include "Modulo.h"
#include "Multiplication.h"
#include "Negation.h"
#include "NotEqual.h"
#include "Pointer.h"
#include "Property.h"
#include "Return.h"
#include "Subtraction.h"
#include "TypeDeclaration.h"
#include "UnitDeclaration.h"
#include "Variable.h"
#include "VariableDeclaration.h"
#include "WithStatement.h"
