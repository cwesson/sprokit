/**
 * @file CollectSymbols.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "CollectSymbols.h"
#include "AST.h"
#include "sym/GlobalSymbols.h"
#include "sym/FunctionSymbols.h"

CollectSymbols::CollectSymbols() :
	table(new GlobalSymbols())
{}

CollectSymbols::~CollectSymbols() {
	delete table;
}

void CollectSymbols::prepTable(AST::ASTNode& v) {
	v.table = table;
}

void CollectSymbols::removeTable() {
	table = table->parent;
}

void CollectSymbols::visit(AST::Addition& v) {
	prepTable(v);
	v.left->accept(*this);
	v.right->accept(*this);
}

void CollectSymbols::visit(AST::Assignment& v) {
	prepTable(v);
	auto sym = table->findVariable(v.name);
	if(sym == nullptr) {
		printError(v, "Unknown variable " + v.name);
	}else{
		sym->modified = true;
		if(sym->constant){
			printError(v, "Attempt to modify constant " + v.name);
		}
	}

	v.expression->accept(*this);
}

void CollectSymbols::visit(AST::Conversion& v) {}

void CollectSymbols::visit(AST::Division& v) {
	prepTable(v);
	v.left->accept(*this);
	v.right->accept(*this);
}

void CollectSymbols::visit(AST::Equal& v) {
	prepTable(v);
	v.left->accept(*this);
	v.right->accept(*this);
}

void CollectSymbols::visit(AST::Exponent& v) {
	prepTable(v);
	v.left->accept(*this);
	v.right->accept(*this);
}

void CollectSymbols::visit(AST::FunctionCall& v) {}

void CollectSymbols::visit(AST::FunctionDeclaration& v) {
	prepTable(v);
	
	auto function = table->addFunction(v.name);
	if(function == nullptr){
		printError(v, "Duplicate function declaration " + v.name);
	}else{
		function->type = v.type;
		function->unit = v.unit;

		table = function->table;
			v.params->accept(*this);
			v.body->accept(*this);
		removeTable();
	}
}

void CollectSymbols::visit(AST::IfStatement& v) {
	v.condition->accept(*this);
	v.body->accept(*this);
	if(v.elsebody != nullptr){
		v.elsebody->accept(*this);
	}
}

void CollectSymbols::visit(AST::IntegerLiteral& v) {}

void CollectSymbols::visit(AST::List& v) {
	prepTable(v);
	if(v.node != nullptr){
		v.node->accept(*this);
	}
	if(v.next != nullptr){
		v.next->accept(*this);
	}
};

void CollectSymbols::visit(AST::Modulo& v) {
	prepTable(v);
	v.left->accept(*this);
	v.right->accept(*this);
}

void CollectSymbols::visit(AST::Multiplication& v) {
	prepTable(v);
	v.left->accept(*this);
	v.right->accept(*this);
}

void CollectSymbols::visit(AST::ParameterDeclaration& v) {
	prepTable(v);
	
	auto var = table->addParameter(v.name);
	if(var == nullptr){
		printError(v, "Duplicate parameter declaration " + v.name);
	}else{
		var->type = v.type;
		var->unit = v.unit;
		var->constant = v.constant;
	}
}

void CollectSymbols::visit(AST::Return& v) {
	prepTable(v);
	if(v.expression != nullptr){
		v.expression->accept(*this);
	}
}

void CollectSymbols::visit(AST::Subtraction& v) {
	prepTable(v);
	v.left->accept(*this);
	v.right->accept(*this);
}

void CollectSymbols::visit(AST::TypeDeclaration& v) {
	prepTable(v);

	SymbolTable* t = table->addType(v.name);
	if(t == nullptr){
		printError(v, "Duplicate type declaration " + v.name);
	}else{
		table = t;
			v.list->accept(*this);
		removeTable();
	}
}

void CollectSymbols::visit(AST::UnitDeclaration& v) {}

void CollectSymbols::visit(AST::Variable& v) {
	prepTable(v);

	auto sym = table->findVariable(v.name);
	if(sym == nullptr) {
		printError(v, "Unknown variable " + v.name);
	}else{
		sym->used = true;
	}
}

void CollectSymbols::visit(AST::VariableDeclaration& v) {
	prepTable(v);
	
	auto sym = table->addVariable(v.name);
	if(sym == nullptr){
		printError(v, "Duplicate variable declaration " + v.name);
	}else{
		sym->type = v.type;
		sym->unit = v.unit;
		sym->constant = v.constant;
	}
	
	if(v.initial != nullptr){
		v.initial->accept(*this);
	}
}
