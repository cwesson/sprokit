/**
 * @file CollectSymbols.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "CollectSymbols.h"
#include "AST.h"
#include "sym/GlobalSymbols.h"
#include "sym/FunctionSymbols.h"
#include "sym/OrderedSymbol.h"
#include "sym/ScopedSymbols.h"
#include "UserType.h"

CollectSymbols::CollectSymbols() :
	table(new GlobalSymbols()),
	type_table(nullptr),
	user_type(nullptr),
	collect_param(false)
{}

CollectSymbols::~CollectSymbols() {
	delete table;
}

void CollectSymbols::prepTable(AST::ASTNode& v) {
	v.table = table;
}

void CollectSymbols::removeTable() {
	while(!table->isScope()){
		table = table->parent;
	}
	if(table->parent != nullptr){
		table = table->parent;
	}
}

void CollectSymbols::visit(AST::Addition& v) {
	prepTable(v);
	v.left->accept(*this);
	v.right->accept(*this);
}

void CollectSymbols::visit(AST::Array& v) {
	prepTable(v);
	if(v.length_var != nullptr){
		v.length_var->accept(*this);
	}else if(v.expression != nullptr){
		v.expression->accept(*this);
	}
	if(v.array != nullptr){
		v.array->accept(*this);
	}
}

void CollectSymbols::visit(AST::Assignment& v) {
	prepTable(v);
	v.var->accept(*this);
	auto sym = table->findVariable(v.var->name);
	if(sym != nullptr){
		sym->modified = true;
		if(sym->constant){
			printError(v, "Attempt to modify constant " + v.var->name);
		}
	}

	v.expression->accept(*this);
}

void CollectSymbols::visit(AST::BoolLiteral& v) {
	prepTable(v);
}

void CollectSymbols::visit(AST::Conversion& v) {
	prepTable(v);
	v.expression->accept(*this);
}

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

void CollectSymbols::visit(AST::FunctionCall& v) {
	prepTable(v);
	v.params->accept(*this);
}

void CollectSymbols::visit(AST::FunctionDeclaration& v) {
	prepTable(v);
	
	auto function = table->addFunction(v.name);
	if(function == nullptr){
		printError(v, "Duplicate function declaration " + v.name);
	}else{
		function->type = &ADT::Type::findType(v.type);
		function->unit = v.unit;
		function->pointer = v.pointer;

		table = function->table;
		collect_param = true;
			v.params->accept(*this);
		collect_param = false;
			v.body->accept(*this);
		removeTable();
	}
}

void CollectSymbols::visit(AST::IfStatement& v) {
	prepTable(v);
	v.condition->accept(*this);
	table = table->addScope("$if");
		v.body->accept(*this);
	removeTable();
	if(v.elsebody != nullptr){
		table = table->addScope("$else");
			v.elsebody->accept(*this);
		removeTable();
	}
}

void CollectSymbols::visit(AST::IntegerLiteral& v) {
	prepTable(v);
}

void CollectSymbols::visit(AST::List& v) {
	prepTable(v);
	if(v.node != nullptr){
		v.node->accept(*this);
	}
	if(v.next != nullptr){
		v.next->accept(*this);
	}
};

void CollectSymbols::visit(AST::Member& v) {
	prepTable(v);
	v.left->accept(*this);

	auto sym = v.table->findVariable(v.left->name);
	if(type_table != nullptr){
		sym = type_table->findVariable(v.left->name);
	}
	if(sym != nullptr){
		type_table = v.table->findType((std::string)*sym->type);
	}

	v.right->table = type_table;
	v.right->accept(*this);
	type_table = nullptr;
}

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

void CollectSymbols::visit(AST::NotEqual& v) {
	prepTable(v);
	v.left->accept(*this);
	v.right->accept(*this);
}

void CollectSymbols::visit(AST::Pointer& v) {
	prepTable(v);
	if(v.var != nullptr){
		v.var->accept(*this);
	}
}

void CollectSymbols::visit(AST::Property& v) {
	prepTable(v);
	if(v.var != nullptr){
		v.var->accept(*this);
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
		ADT::UserType* type = ADT::Type::createType(v.name);
		if(type != nullptr){
			user_type = type;
			table = t;
				v.list->accept(*this);
			removeTable();
			user_type = nullptr;
		}else{
			printError(v, "Duplicate type declaration " + v.name);
		}
	}
}

void CollectSymbols::visit(AST::UnitDeclaration& v) {
	prepTable(v);
	auto sym = table->addUnit(v.unit);
	if(sym != nullptr){
		sym->alias = v.alias;
	}
	v.list->accept(*this);
}

void CollectSymbols::visit(AST::Variable& v) {
	prepTable(v);
	if(type_table != nullptr){
		v.table = type_table;
	}

	auto sym = v.table->findVariable(v.name);
	if(sym == nullptr) {
		printError(v, "Variable " + v.name + " not declared.");
	}else{
		sym->used = true;
	}

	if(v.array != nullptr){
		v.array->accept(*this);
	}
}

void CollectSymbols::visit(AST::VariableDeclaration& v) {
	prepTable(v);
	
	if(collect_param){
		auto var = table->addParameter(v.name);
		if(var == nullptr){
			printError(v, "Declaration of parameter " + v.name + " would shadow previous declaration.");
		}else{
			var->type = &ADT::Type::findType(v.type);
			var->unit = v.unit;
			var->constant = v.constant;
			var->pointer = v.pointer;
		}
	}else{
		SymbolTable* updated = nullptr;
		auto sym = table->addVariable(v.name, &updated);
		if(sym == nullptr){
			printError(v, "Declaration of variable " + v.name + " would shadow previous declaration.");
		}else{
			sym->type = &ADT::Type::findType(v.type);
			sym->unit = v.unit;
			sym->constant = v.constant;
			sym->pointer = v.pointer;
			if(user_type != nullptr){
				user_type->addMember(sym->type);
			}
			table = updated;
			v.table = updated;
		}
	}
	
	if(v.initial != nullptr){
		v.initial->accept(*this);
	}
}
