/**
 * @file ASTNode.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "AST.h"
#include "Visitor.h"

namespace AST {

/**
 * X-macro defining accept functions for each ASTNode.
 * @ingroup ast
 */
#define X(_x) void _x::accept(Visitor& v) { v.visit(*this); }
AST_LIST
#undef X

}
