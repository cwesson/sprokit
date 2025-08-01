/**
 * @file LLCodeGen.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "LLCodeGen.h"
#include "PrimitiveType.h"
#include "UserType.h"
#include "TypeDecorator.h"
#include <sstream>

LLCodeGen::LLCodeGen(std::ostream& o) :
	os(o),
	temp_count(1),
	inparams(false),
	islast(false)
{}

std::string LLCodeGen::translateType(const ADT::BoolType& t) const {
	return "i1";
}

std::string LLCodeGen::translateType(const ADT::IntType& t) const {
	std::stringstream s;
	s << "i" << t.length;
	return s.str();
}

std::string LLCodeGen::translateType(const ADT::FloatType& t) const {
	unsigned int size = t.size();
	switch(size){
		case 2:
			return "half";
		case 4:
			return "float";
		case 8:
			return "double";
		case 16:
			return "fp128";
		default:
			return "$unknown";
	}
}

std::string LLCodeGen::translateType(const ADT::UserType& t) const {
	std::stringstream s;
	s << "%" << (std::string)t;
	return s.str();
}

std::string LLCodeGen::translateType(const ADT::PointerType& t) const {
	return t.type.translate(*this);
}

std::string LLCodeGen::getTemporary() {
	std::stringstream ss;
	ss << "%" << temp_count++;
	return ss.str();
}

void LLCodeGen::visit(AST::Addition& v) {
	v.left->accept(*this);
	std::string left = last_temp;
	v.right->accept(*this);
	std::string right = last_temp;
	ADT::Type& type = v.getType();
	last_temp = getTemporary();
	os << last_temp << " = ";
	if(type.isFloat()){
		os << "f";
	}
	os << "add " << type.translate(*this) << " " << left << ", " << right << std::endl;
}

void LLCodeGen::visit(AST::Array& v) {
}

void LLCodeGen::visit(AST::Assignment& v) {
	v.var->accept(*this);
	std::string var = last_temp;
	v.expression->accept(*this);
	os << var << " = " << last_temp << std::endl;
}

void LLCodeGen::visit(AST::BoolLiteral& v) {
	last_temp = v.value ? "true" : "false";
}

void LLCodeGen::visit(AST::Conversion& v) {
}

void LLCodeGen::visit(AST::Division& v) {
	v.left->accept(*this);
	std::string left = last_temp;
	v.right->accept(*this);
	std::string right = last_temp;
	ADT::Type& type = v.getType();
	last_temp = getTemporary();
	os << last_temp << " = ";
	if(type.isFloat()){
		os << "f";
	}else if(type.isSigned()){
		os << "s";
	}else{
		os << "u";
	}
	os << "div " << type.translate(*this) << " " << left << ", " << right << std::endl;
}

void LLCodeGen::visit(AST::Equal& v) {
}

void LLCodeGen::visit(AST::Exponent& v) {
}

void LLCodeGen::visit(AST::FloatLiteral& v) {
	std::stringstream s;
	s << v.value;
	last_temp = s.str();
}

void LLCodeGen::visit(AST::FunctionCall& v) {
	last_temp = getTemporary();
	os << last_temp << " = call " << v.getType().translate(*this) << " @" << v.name << "()" << std::endl;
}

void LLCodeGen::visit(AST::FunctionDeclaration& v) {
	ADT::Type& type = ADT::Type::findType(v.type);
	os << "define " << type.translate(*this) << " @" << v.name << "(";
	inparams = true;
		v.params->accept(*this);
	inparams = false;
	os << ") {" << std::endl;
	v.body->accept(*this);
	os << "}" << std::endl;
}

void LLCodeGen::visit(AST::IfStatement& v) {
}

void LLCodeGen::visit(AST::IntegerLiteral& v) {
	std::stringstream s;
	s << v.value;
	last_temp = s.str();
}

void LLCodeGen::visit(AST::List& v) {
	if(v.node != nullptr){
		islast = v.next == nullptr;
		v.node->accept(*this);
	}
	if(v.next != nullptr){
		v.next->accept(*this);
	}
}

void LLCodeGen::visit(AST::Member& v) {
}

void LLCodeGen::visit(AST::Modulo& v) {
	v.left->accept(*this);
	std::string left = last_temp;
	v.right->accept(*this);
	std::string right = last_temp;
	ADT::Type& type = v.getType();
	last_temp = getTemporary();
	os << last_temp << " = ";
	if(type.isFloat()){
		os << "f";
	}else if(type.isSigned()){
		os << "s";
	}else{
		os << "u";
	}
	os << "rem " << type.translate(*this) << " " << left << ", " << right << std::endl;
}

void LLCodeGen::visit(AST::Multiplication& v) {
	v.left->accept(*this);
	std::string left = last_temp;
	v.right->accept(*this);
	std::string right = last_temp;
	ADT::Type& type = v.getType();
	last_temp = getTemporary();
	os << last_temp << " = ";
	if(type.isFloat()){
		os << "f";
	}
	os << "mul " << type.translate(*this) << " " << left << ", " << right << std::endl;
}

void LLCodeGen::visit(AST::NotEqual& v) {
}

void LLCodeGen::visit(AST::Pointer& v) {
}

void LLCodeGen::visit(AST::Property& v) {
}

void LLCodeGen::visit(AST::Return& v) {
	v.expression->accept(*this);
	ADT::Type& t = v.expression->getType();
	os << "ret " << t.translate(*this) << " " << last_temp << std::endl;
}

void LLCodeGen::visit(AST::Subtraction& v) {
	v.left->accept(*this);
	std::string left = last_temp;
	v.right->accept(*this);
	std::string right = last_temp;
	ADT::Type& type = v.getType();
	last_temp = getTemporary();
	os << last_temp << " = ";
	if(type.isFloat()){
		os << "f";
	}
	os << "sub " << type.translate(*this) << " " << left << ", " << right << std::endl;
}

void LLCodeGen::visit(AST::TypeDeclaration& v) {
	os << "%" << v.name << " = type {" << std::endl;
	inparams = true;
		v.list->accept(*this);
	inparams = false;
	os << "}" << std::endl;
}

void LLCodeGen::visit(AST::UnitDeclaration& v) {
}

void LLCodeGen::visit(AST::Variable& v) {
	last_temp = std::string("%") + v.name;
}

void LLCodeGen::visit(AST::VariableDeclaration& v) {
	if(inparams){
		ADT::Type& t = *v.table->findVariable(v.name)->type;
		os << t.translate(*this) << " %" << v.name;
		if(!islast){
			os << ", ";
		}
	}else if(v.initial != nullptr){
		v.initial->accept(*this);
		os << "%" << v.name << " = " << last_temp << std::endl;
	}
}
