/**
 * @file Visitor.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "Visitor.h"
#include "ASTNode.h"
#include "BinaryOperator.h"
#include <iostream>

Visitor::Visitor() :
	error_count(0)
{}

void Visitor::printError(AST::ASTNode& node, const std::string& msg) {
	++error_count;
	std::cerr << "Error at " << node.pos << ": " << msg << std::endl;
}

void Visitor::printWarning(AST::ASTNode& node, const std::string& msg) {
	++error_count;
	std::cerr << "Warning at " << node.pos << ": " << msg << std::endl;
}
