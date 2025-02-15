/**
 * @file PrintAST.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "PrintAST.h"
#include "AST.h"
#include <iostream>
#include <iomanip>

PrintAST::PrintAST() :
	indent(0)
{}

void PrintAST::printIndent(AST::ASTNode& v) {
	std::cout << std::endl << std::setw(4) << v.lineno << std::setw(1) << ": ";
	for(unsigned int i = 0; i < indent; ++i){
		std::cout << "  ";
	}
}

void PrintAST::visit(AST::Addition& v) {
	printIndent(v);
	std::cout << "ADD";
	++indent;
		v.left->accept(*this);
		v.right->accept(*this);
	--indent;
}

void PrintAST::visit(AST::Assignment& v) {
	printIndent(v);
	std::cout << "ASSIGN " << v.name;
	++indent;
		if(v.expression != nullptr){
			v.expression->accept(*this);
		}
	--indent;
}

void PrintAST::visit(AST::Conversion& v) {
	printIndent(v);
	std::cout << "CONVERSION " <<v.to << " " << v.name;
	++indent;
		v.expression->accept(*this);
	--indent;
}

void PrintAST::visit(AST::Division& v) {
	printIndent(v);
	std::cout << "DIV";
	++indent;
		v.left->accept(*this);
		v.right->accept(*this);
	--indent;
}

void PrintAST::visit(AST::Equal& v) {
	printIndent(v);
	std::cout << "EQUAL";
	++indent;
		v.left->accept(*this);
		v.right->accept(*this);
	--indent;
}

void PrintAST::visit(AST::Exponent& v) {
	printIndent(v);
	std::cout << "EXP";
	++indent;
		v.left->accept(*this);
		v.right->accept(*this);
	--indent;
}

void PrintAST::visit(AST::FunctionCall& v) {
	printIndent(v);
	std::cout << "CALL " << v.name;
	++indent;
		if(v.params){
			v.params->accept(*this);
		}
	--indent;
}

void PrintAST::visit(AST::FunctionDeclaration& v) {
	printIndent(v);
	std::cout << "FUNC " << v.type << " " << v.name << " " << v.unit;
	++indent;
		if(v.params){
			v.params->accept(*this);
		}
	--indent;
	++indent;
		v.body->accept(*this);
	--indent;
}

void PrintAST::visit(AST::IfStatement& v) {
	printIndent(v);
	std::cout << "IF ";
	++indent;
		v.condition->accept(*this);
	--indent;
	++indent;
		v.body->accept(*this);
	--indent;
	if(v.elsebody != nullptr){
		++indent;
			v.elsebody->accept(*this);
		--indent;
	}
}

void PrintAST::visit(AST::IntegerLiteral& v) {
	printIndent(v);
	std::cout << v.value << " " << v.unit;
}

void PrintAST::visit(AST::List& v) {
	if(v.node != nullptr){
		v.node->accept(*this);
	}
	if(v.next != nullptr){
		v.next->accept(*this);
	}
}

void PrintAST::visit(AST::Modulo& v) {
	printIndent(v);
	std::cout << "MOD";
	++indent;
		v.left->accept(*this);
		v.right->accept(*this);
	--indent;
}

void PrintAST::visit(AST::Multiplication& v) {
	printIndent(v);
	std::cout << "MUL";
	++indent;
		v.left->accept(*this);
		v.right->accept(*this);
	--indent;
}

void PrintAST::visit(AST::ParameterDeclaration& v) {
	printIndent(v);
	std::cout << "PARAM ";
	if(v.constant){
		std::cout << "CONST ";
	}else{
		std::cout << "VAR   ";
	}
	std::cout << v.type << " " << v.name << " " << v.unit;
}

void PrintAST::visit(AST::Return& v) {
	printIndent(v);
	std::cout << "RETURN";
	++indent;
		if(v.expression != nullptr){
			v.expression->accept(*this);
		}
	--indent;
}

void PrintAST::visit(AST::Subtraction& v) {
	printIndent(v);
	std::cout << "SUB";
	++indent;
		v.left->accept(*this);
		v.right->accept(*this);
	--indent;
}

void PrintAST::visit(AST::TypeDeclaration& v) {
	printIndent(v);
	std::cout << "TYPE " << v.name;
	++indent;
		v.list->accept(*this);
	--indent;
}

void PrintAST::visit(AST::UnitDeclaration& v) {
	printIndent(v);
	std::cout << "UNIT " << v.unit;
	++indent;
		v.list->accept(*this);
	--indent;
}

void PrintAST::visit(AST::Variable& v) {
	printIndent(v);
	std::cout << "VARIABLE " << v.name;
}

void PrintAST::visit(AST::VariableDeclaration& v) {
	printIndent(v);
	std::cout << "DECLARE ";
	if(v.constant){
		std::cout << "CONST ";
	}else{
		std::cout << "VAR   ";
	}
	std::cout << v.type << " " << v.name << " " << v.unit;
	++indent;
		if(v.initial != nullptr){
			v.initial->accept(*this);
		}
	--indent;
}
