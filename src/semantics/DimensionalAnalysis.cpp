/**
 * @file DimensionalAnalysis.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "DimensionalAnalysis.h"
#include "AST.h"
#include "sym/SymbolTable.h"

void DimensionalAnalysis::visit(AST::Addition& v) {
	v.left->accept(*this);
	Dimensions left_unit = constructed_unit;
	v.right->accept(*this);
	Dimensions right_unit = constructed_unit;
	if(left_unit != right_unit){
		printError(v, "Mismatched units in addition, " + (std::string)left_unit + " and " + (std::string)right_unit);
	}
	constructed_unit = left_unit;
	v.dim = constructed_unit;
}

void DimensionalAnalysis::visit(AST::Assignment& v) {
	auto sym = v.table->findVariable(v.name);
	if(sym != nullptr){
		Dimensions left_unit = parser.parse(sym->unit);
		v.expression->accept(*this);
		if(left_unit != constructed_unit){
			printError(v, "Mismatched units in assignment, was " + (std::string)constructed_unit + ", expected " + sym->unit);
		}
	}
}

void DimensionalAnalysis::visit(AST::Conversion& v) {}

void DimensionalAnalysis::visit(AST::Division& v) {
	v.left->accept(*this);
	Dimensions left_unit = constructed_unit;
	v.right->accept(*this);
	Dimensions right_unit = constructed_unit;
	constructed_unit = left_unit /= right_unit;
	v.dim = constructed_unit;
}

void DimensionalAnalysis::visit(AST::Equal& v) {
	v.left->accept(*this);
	Dimensions left_unit = constructed_unit;
	v.right->accept(*this);
	if(left_unit != constructed_unit){
		printError(v, "Mismatched units in equality, " + (std::string)left_unit + ", and " + (std::string)constructed_unit);
	}
}

void DimensionalAnalysis::visit(AST::Exponent& v) {}

void DimensionalAnalysis::visit(AST::FunctionCall& v) {
	v.params->accept(*this);
}

void DimensionalAnalysis::visit(AST::FunctionDeclaration& v) {
	v.body->accept(*this);
}

void DimensionalAnalysis::visit(AST::IfStatement& v) {
	v.condition->accept(*this);
	v.body->accept(*this);
	if(v.elsebody != nullptr){
		v.elsebody->accept(*this);
	}
}

void DimensionalAnalysis::visit(AST::IntegerLiteral& v) {
	constructed_unit = parser.parse(v.unit);
}

void DimensionalAnalysis::visit(AST::List& v) {
	if(v.node != nullptr){
		v.node->accept(*this);
	}
	if(v.next != nullptr){
		v.next->accept(*this);
	}
}

void DimensionalAnalysis::visit(AST::Modulo& v) {}

void DimensionalAnalysis::visit(AST::Multiplication& v) {
	v.left->accept(*this);
	Dimensions left_unit = constructed_unit;
	v.right->accept(*this);
	Dimensions right_unit = constructed_unit;
	constructed_unit = left_unit *= right_unit;
	v.dim = constructed_unit;
}

void DimensionalAnalysis::visit(AST::ParameterDeclaration& v) {}

void DimensionalAnalysis::visit(AST::Return& v) {}

void DimensionalAnalysis::visit(AST::Subtraction& v) {
	v.left->accept(*this);
	Dimensions left_unit = constructed_unit;
	v.right->accept(*this);
	Dimensions right_unit = constructed_unit;
	if(left_unit != right_unit){
		printError(v, "Mismatched units in subtraction, " + (std::string)left_unit + " and " + (std::string)right_unit);
	}
	constructed_unit = left_unit;
	v.dim = constructed_unit;
}

void DimensionalAnalysis::visit(AST::TypeDeclaration& v) {}

void DimensionalAnalysis::visit(AST::UnitDeclaration& v) {}

void DimensionalAnalysis::visit(AST::Variable& v) {
	auto sym = v.table->findVariable(v.name);
	if(sym != nullptr){
		constructed_unit = parser.parse(sym->unit);
		v.dim = constructed_unit;
	}
}

void DimensionalAnalysis::visit(AST::VariableDeclaration& v) {
	v.initial->accept(*this);
	if(constructed_unit != parser.parse(v.unit)){
		printError(v, "Mismatched units in initialization, was " + (std::string)constructed_unit + ", expected " + v.unit);
	}
}
