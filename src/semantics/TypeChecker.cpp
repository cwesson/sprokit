/**
 * @file TypeChecker.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "TypeChecker.h"
#include "sym/SymbolTable.h"

TypeChecker::TypeChecker() :
	func_type(nullptr),
	func(nullptr)
{}

void TypeChecker::visit(AST::Addition& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot add ") + std::string(left) + " and " + std::string(right));
	}
}

void TypeChecker::visit(AST::Array& v) {
	v.expression->accept(*this);
	ADT::Type& exp = v.expression->getType();
	if(!exp.convertibleTo(ADT::Type::findType("uint64"))){
		printError(v, std::string("Cannot use ") + std::string(exp) + " as array index");
	}
}

void TypeChecker::visit(AST::Assignment& v) {
	v.expression->accept(*this);
	auto var = v.table->findVariable(v.var->name);
	if(var != nullptr){
		ADT::Type& left = *var->type;
		ADT::Type& right = v.expression->getType();
		if(!right.convertibleTo(left)){
			printError(v, std::string("Cannot assign ") + std::string(right) + " to " + std::string(left));
		}
	}
}

void TypeChecker::visit(AST::BoolLiteral& v) {
}

void TypeChecker::visit(AST::Conversion& v) {
}

void TypeChecker::visit(AST::Division& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot divide ") + std::string(left) + " by " + std::string(right));
	}
}

void TypeChecker::visit(AST::Equal& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot compare ") + std::string(left) + " and " + std::string(right));
	}
}

void TypeChecker::visit(AST::Exponent& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot raise ") + std::string(left) + " to " + std::string(right));
	}
}

void TypeChecker::visit(AST::FunctionCall& v) {
	auto f = v.table->findFunction(v.name);
	if(f == nullptr){
		printError(v, std::string("Unkown function ") + v.name);
	}else{
		func = f;
		v.params->accept(*this);
		func = nullptr;
		func_type = f->type;
	}
}

void TypeChecker::visit(AST::FunctionDeclaration& v) {
	auto table = v.table->findFunction(v.name);
	func_type = table->type;
	v.body->accept(*this);
	func_type = nullptr;
}

void TypeChecker::visit(AST::IfStatement& v) {
	v.condition->accept(*this);
	ADT::Type& cond = v.condition->getType();
	if(!cond.convertibleTo(ADT::Type::findType("bool"))){
		printError(v, std::string("Cannot convert ") + std::string(cond) + " to bool");
	}
	if(v.body != nullptr){
		v.body->accept(*this);
	}
	if(v.elsebody != nullptr){
		v.elsebody->accept(*this);
	}
}

void TypeChecker::visit(AST::IntegerLiteral& v) {
}

void TypeChecker::visit(AST::List& v) {
	if(v.node != nullptr){
		v.node->accept(*this);
	}
	if(v.next != nullptr){
		v.next->accept(*this);
	}
}

void TypeChecker::visit(AST::Member& v) {
	v.left->accept(*this);
	v.right->accept(*this);
}

void TypeChecker::visit(AST::Modulo& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot modulo ") + std::string(left) + " by " + std::string(right));
	}
}

void TypeChecker::visit(AST::Multiplication& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot multiply ") + std::string(left) + " by " + std::string(right));
	}
}

void TypeChecker::visit(AST::NotEqual& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot compare ") + std::string(left) + " and " + std::string(right));
	}
}

void TypeChecker::visit(AST::Pointer& v) {
}

void TypeChecker::visit(AST::Property& v) {
}

void TypeChecker::visit(AST::Return& v) {
	v.expression->accept(*this);
	ADT::Type& ret = v.expression->getType();
	if(!ret.convertibleTo(*func_type)){
		printError(v, "Cannot return " + std::string(ret) + ", expecting " + std::string(*func_type));
	}
}

void TypeChecker::visit(AST::Subtraction& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot subtract ") + std::string(left) + " and " + std::string(right));
	}
}

void TypeChecker::visit(AST::TypeDeclaration& v) {
	v.list->accept(*this);
}

void TypeChecker::visit(AST::UnitDeclaration& v) {
	v.list->accept(*this);
}

void TypeChecker::visit(AST::Variable& v) {
}

void TypeChecker::visit(AST::VariableDeclaration& v) {
	if(v.initial != nullptr){
		v.initial->accept(*this);
		auto var = v.table->findVariable(v.name);
		if(var != nullptr){
			ADT::Type& left = *var->type;
			ADT::Type& right = v.initial->getType();
			if(!right.convertibleTo(left)){
				printError(v, std::string("Cannot assign ") + std::string(right) + " to " + std::string(left));
			}
		}
	}
}
