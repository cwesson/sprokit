/**
 * @file DimensionalAnalysis.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "DimensionalAnalysis.h"
#include "AST.h"
#include "sym/SymbolTable.h"
#include "sym/FunctionSymbols.h"

DimensionalAnalysis::DimensionalAnalysis() :
	constructed_unit(),
	parser(),
	in_func(nullptr)
{}

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

void DimensionalAnalysis::visit(AST::Array& v) {
	if(v.expression != nullptr){
		v.expression->accept(*this);
	}
	if(constructed_unit != Dimensions{}){
		printError(v, "Array index must be dimensionless, was " + (std::string)constructed_unit);
	}
	if(v.array != nullptr){
		v.array->accept(*this);
	}
}

void DimensionalAnalysis::visit(AST::Assignment& v) {
	v.var->accept(*this);
	Dimensions left_unit = constructed_unit;
	v.expression->accept(*this);
	if(left_unit != constructed_unit){
		printError(v, "Mismatched units in assignment, was " + (std::string)constructed_unit + ", expected " + (std::string)left_unit);
	}
}

void DimensionalAnalysis::visit(AST::BoolLiteral& v) {}

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

void DimensionalAnalysis::visit(AST::Exponent& v) {
	v.left->accept(*this);
	Dimensions left_unit = constructed_unit;
	v.right->accept(*this);
	if(constructed_unit != Dimensions{}){
		printError(v, "Exponent must be dimensionless, was " + (std::string)constructed_unit);
	}
	if(!v.right->is_constexpr()){
		printWarning(v, "Cannot evaluate dimensions with non-constant exponent");
	}
}

void DimensionalAnalysis::visit(AST::FunctionCall& v) {
	auto func = v.table->findFunction(v.name);
	if(func != nullptr){
		unsigned int i = 0;
		for(auto param = v.params; param != nullptr; param = param->next){
			if(param->node != nullptr){
				param->node->accept(*this);
				Dimensions param_unit = parser.parse(func->table->params[i]->unit);
				if(constructed_unit != param_unit){
					printError(v, "Mismatched units in parameter, was " + (std::string)constructed_unit + ", expected " + (std::string)param_unit);
				}
			}
			++i;
		}
		constructed_unit = parser.parse(func->unit);
	}
}

void DimensionalAnalysis::visit(AST::FunctionDeclaration& v) {
	in_func = &v;
		v.body->accept(*this);
	in_func = nullptr;
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

void DimensionalAnalysis::visit(AST::Member& v) {
	v.left->accept(*this);
	v.right->accept(*this);
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

void DimensionalAnalysis::visit(AST::Pointer& v) {
	v.var->accept(*this);
}

void DimensionalAnalysis::visit(AST::Property& v) {
	v.var->accept(*this);
}

void DimensionalAnalysis::visit(AST::Return& v) {
	if(v.expression != nullptr){
		v.expression->accept(*this);
		Dimensions ret_unit = parser.parse(in_func->unit);
		if(constructed_unit != ret_unit){
			printError(v, "Mismatched units in return, was " + (std::string)constructed_unit + ", expected " + (std::string)ret_unit);
		}
	}
}

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
	if(v.array != nullptr){
		v.array->accept(*this);
	}
	auto sym = v.table->findVariable(v.name);
	if(sym != nullptr){
		constructed_unit = parser.parse(sym->unit);
		v.dim = constructed_unit;
	}
}

void DimensionalAnalysis::visit(AST::VariableDeclaration& v) {
	if(v.array != nullptr){
		v.array->accept(*this);
	}
	if(v.initial != nullptr){
		v.initial->accept(*this);
		if(v.unit == ""){
			// Unit was not specifid, derive from initializer
			v.unit = (std::string)constructed_unit;
			auto sym = v.table->findVariable(v.name);
			if(sym != nullptr){
				sym->unit = constructed_unit;
			}
		}
		if(constructed_unit != parser.parse(v.unit)){
			printError(v, "Mismatched units in initialization, was " + (std::string)constructed_unit + ", expected " + v.unit);
		}
	}else if(v.unit == ""){
		printError(v, "Units must be specified for function parameters and type members.");
	}
}
