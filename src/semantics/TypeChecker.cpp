/**
 * @file TypeChecker.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "TypeChecker.h"
#include "sym/TypeSymbols.h"

TypeChecker::TypeChecker() :
	func_type(nullptr),
	func(nullptr),
	expect_type(nullptr),
	type_table(nullptr)
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

void TypeChecker::visit(AST::BitAnd& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot AND ") + std::string(left) + " and " + std::string(right));
	}
	if(!left.isInt()){
		printError(*v.left, std::string("Cannot convert ") + std::string(left) + " to integer");
	}
	if(!right.isInt()){
		printError(*v.right, std::string("Cannot convert ") + std::string(right) + " to integer");
	}
}

void TypeChecker::visit(AST::BitNot& v) {
	v.right->accept(*this);
	ADT::Type& right = v.right->getType();
	if(!right.isInt()){
		printError(v, std::string("Cannot convert ") + std::string(right) + " to integer");
	}
}

void TypeChecker::visit(AST::BitOr& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot OR ") + std::string(left) + " and " + std::string(right));
	}
	if(!left.isInt()){
		printError(*v.left, std::string("Cannot convert ") + std::string(left) + " to integer");
	}
	if(!right.isInt()){
		printError(*v.right, std::string("Cannot convert ") + std::string(right) + " to integer");
	}
}

void TypeChecker::visit(AST::BitXor& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot XOR ") + std::string(left) + " and " + std::string(right));
	}
	if(!left.isInt()){
		printError(*v.left, std::string("Cannot convert ") + std::string(left) + " to integer");
	}
	if(!right.isInt()){
		printError(*v.right, std::string("Cannot convert ") + std::string(right) + " to integer");
	}
}

void TypeChecker::visit(AST::BoolAnd& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	ADT::Type& btype = ADT::Type::findType("bool");
	if(!right.convertibleTo(btype)){
		printError(*v.right, std::string("Cannot convert ") + std::string(right) + " to " + std::string(btype));
	}
	if(!left.convertibleTo(btype)){
		printError(*v.left, std::string("Cannot convert ") + std::string(left) + " to " + std::string(btype));
	}
}

void TypeChecker::visit(AST::BoolLiteral& v) {
}

void TypeChecker::visit(AST::BoolNot& v) {
	v.right->accept(*this);
	ADT::Type& right = v.right->getType();
	ADT::Type& btype = ADT::Type::findType("bool");
	if(!right.convertibleTo(btype)){
		printError(*v.right, std::string("Cannot convert ") + std::string(right) + " to " + std::string(btype));
	}
}

void TypeChecker::visit(AST::BoolOr& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	ADT::Type& btype = ADT::Type::findType("bool");
	if(!right.convertibleTo(btype)){
		printError(*v.right, std::string("Cannot convert ") + std::string(right) + " to " + std::string(btype));
	}
	if(!left.convertibleTo(btype)){
		printError(*v.left, std::string("Cannot convert ") + std::string(left) + " to " + std::string(btype));
	}
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

void TypeChecker::visit(AST::FloatLiteral& v) {
}

void TypeChecker::visit(AST::ForStatement& v) {
	if(v.init != nullptr){
		v.init->accept(*this);
	}
	v.condition->accept(*this);
	ADT::Type& cond = v.condition->getType();
	if(!cond.convertibleTo(ADT::Type::findType("bool"))){
		printError(v, std::string("Cannot convert ") + std::string(cond) + " to bool");
	}
	if(v.increment != nullptr){
		v.increment->accept(*this);
	}
	v.body->accept(*this);
}

void TypeChecker::visit(AST::FunctionCall& v) {
	auto f = v.table->findFunction(v.name);
	if(f == nullptr){
		printError(v, std::string("Unkown function ") + v.name);
	}else{
		auto last_func = func;
		func = f;
			v.params->accept(*this);
		func = last_func;
	}
}

void TypeChecker::visit(AST::FunctionDeclaration& v) {
	auto table = v.table->findFunction(v.name);
	func_type = table->type;
	v.body->accept(*this);
	func_type = nullptr;
	if(!v.body->allPathsReturn()){
		printError(v, std::string("Reached end of function `") + std::string(v.name) + "` without return");
	}
}

void TypeChecker::visit(AST::GreaterEqual& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot compare ") + std::string(left) + " and " + std::string(right));
	}
}

void TypeChecker::visit(AST::GreaterThan& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot compare ") + std::string(left) + " and " + std::string(right));
	}
}

void TypeChecker::visit(AST::IfStatement& v) {
	if(v.init != nullptr){
		v.init->accept(*this);
	}
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

void TypeChecker::visit(AST::LessEqual& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot compare ") + std::string(left) + " and " + std::string(right));
	}
}

void TypeChecker::visit(AST::LessThan& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	if(!right.convertibleTo(left) && !left.convertibleTo(right)){
		printError(v, std::string("Cannot compare ") + std::string(left) + " and " + std::string(right));
	}
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

void TypeChecker::visit(AST::MemberInitialization& v) {
	v.initial->accept(*this);
	SymbolTable::variable* var = type_table->findVariable(v.name);
	if(var == nullptr){
		printError(v, std::string("No member named `") + std::string(v.name) + "` in " + std::string(*expect_type));
	}
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

void TypeChecker::visit(AST::Negation& v) {
	v.right->accept(*this);
	ADT::Type& right = v.right->getType();
	if(!right.isSigned()){
		printError(v, std::string("Cannot negate ") + std::string(right));
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

void TypeChecker::visit(AST::ShiftLeft& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	ADT::Type& stype = ADT::Type::findType("uint8");
	if(!right.convertibleTo(stype)){
		printError(*v.right, std::string("Cannot convert ") + std::string(right) + " to " + std::string(stype));
	}
	if(!left.isInt()){
		printError(*v.left, std::string("Cannot convert ") + std::string(left) + " to integer");
	}
}

void TypeChecker::visit(AST::ShiftRight& v) {
	v.left->accept(*this);
	v.right->accept(*this);
	ADT::Type& left = v.left->getType();
	ADT::Type& right = v.right->getType();
	ADT::Type& stype = ADT::Type::findType("uint8");
	if(!right.convertibleTo(stype)){
		printError(*v.right, std::string("Cannot convert ") + std::string(right) + " to " + std::string(stype));
	}
	if(!left.isInt()){
		printError(*v.left, std::string("Cannot convert ") + std::string(left) + " to integer");
	}
}

void TypeChecker::visit(AST::StructInitializer& v) {
	type_table = v.table->findType(*expect_type);
		v.list->accept(*this);
		for(auto& mem : dynamic_cast<TypeSymbols*>(type_table)->vars){
			/// @todo check for uninitialized members
			bool init = false;
			for(AST::List* head = v.list; head != nullptr && head->node != nullptr; head = head->next){
				AST::MemberInitialization* check = dynamic_cast<AST::MemberInitialization*>(head->node);
				if(check->name == mem.first){
					init = true;
				}
			}
			if(!init){
				printError(v, std::string("Uninitialized member ") + mem.first);
			}
		}
	type_table = nullptr;
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
		ADT::Type& left = v.type;
		expect_type = &left;
			v.initial->accept(*this);
			auto var = v.table->findVariable(v.name);
			if(var != nullptr){
				ADT::Type& right = v.initial->getType();
				if(!right.convertibleTo(left)){
					printError(*v.initial, std::string("Cannot initialize ") + std::string(left) + " with " + std::string(right));
				}
			}
		expect_type = nullptr;
	}
}

void TypeChecker::visit(AST::VariableLoad& v) {
	v.var->accept(*this);
}

void TypeChecker::visit(AST::WithStatement& v) {
	v.init->accept(*this);
	v.body->accept(*this);
}
