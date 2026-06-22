/**
 * @file Negation.h
 * @author Conlan Wesson
 * @copyright (c) 2026, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Expression.h"
#include <sstream>

namespace AST {

/**
 * ASTNode for unary `-` operator.
 * @ingroup ast
 */
class Negation : public Expression {
	public:
		/**
		 * Constructor.
		 * @param pos Position in source file.
		 * @param r Right-hand side of `-`.
		 */
		Negation(yy::position pos, Expression* r) :
			Expression(pos),
			right(r) {}

		/**
		 * Destructor.
		 */
		virtual ~Negation() override {
			delete right;
		}

		virtual void accept(Visitor& v) override;

		virtual bool is_constexpr() const override {
			return right->is_constexpr();
		}

		virtual ADT::Type& getType() const override {
			ADT::Type& type = right->getType();
			if(type.isSigned()){
				return type;
			}else if(type.isInt()){
				return ADT::Type::findType((std::stringstream() << "sint" << type.size()*8).str());
			}else{
				return ADT::Type::findType("$unknown");
			}
		}

		Expression* right; ///< Right-hand side of the operator.
};

}
