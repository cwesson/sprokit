/**
 * @file LLCodeGen.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "LLCodeGen.h"
#include "PrimitiveType.h"
#include "UserType.h"
#include "TypeDecorator.h"
#include <llvm/IR/Verifier.h>
#include <sstream>

LLCodeGen::LLCodeGen(std::ostream& o, const char* filename) :
	os(o),
	inparams(false),
	islast(false),
	collect_values(false),
	context(),
	module(),
	builder(),
	namedValues(),
	last_value(nullptr),
	translated_type(nullptr),
	arg_types(),
	arg_names()
{
	// Open a new context and module.
	context = std::make_unique<llvm::LLVMContext>();
	module = std::make_unique<llvm::Module>(filename, *context);

	// Create a new builder for the module.
	builder = std::make_unique<llvm::IRBuilder<>>(*context);
}

LLCodeGen::~LLCodeGen() {
	// Print out all of the generated code.
	module->print(os, nullptr);
}

std::string LLCodeGen::translateType(const ADT::BoolType& t) const {
	translated_type = llvm::IntegerType::get(*context, 1);
	return "i1";
}

std::string LLCodeGen::translateType(const ADT::IntType& t) const {
	translated_type = llvm::IntegerType::get(*context, t.length);
	std::stringstream s;
	s << "i" << t.length;
	return s.str();
}

std::string LLCodeGen::translateType(const ADT::FloatType& t) const {
	unsigned int size = t.size();
	switch(size){
		case 2:
			translated_type = llvm::Type::getHalfTy(*context);
			return "f16";
		case 4:
			translated_type = llvm::Type::getFloatTy(*context);
			return "f32";
		case 8:
			translated_type = llvm::Type::getDoubleTy(*context);
			return "f64";
		case 16:
			translated_type = llvm::Type::getFP128Ty(*context);
			return "f128";
		default:
			return "$unknown";
	}
}

std::string LLCodeGen::translateType(const ADT::UserType& t) const {
	std::stringstream s;
	s << "%" << (std::string)t;
	translated_type = llvm::StructType::getTypeByName(*context, (std::string)t);
	return s.str();
}

std::string LLCodeGen::translateType(const ADT::PointerType& t) const {
	return t.type.translate(*this);
}

unsigned int LLCodeGen::getCount(const char* name) {
	unsigned int ret = 0;
	if(counters.contains(name)){
		ret = counters[name];
		++counters[name];
	}else{
		counters[name] = 1;
	}
	return ret;
}

llvm::Value* LLCodeGen::typePromotion(llvm::Value* val, llvm::Type* type, bool isSigned) {
	llvm::Type* from = val->getType();
	if(from != type){
		if(from->isFloatingPointTy() && type->isFloatingPointTy()){
			// Float to Float
			return builder->CreateFPCast(val, type);
		}else if(!from->isFloatingPointTy() && type->isFloatingPointTy()){
			// Int to Float
			if(isSigned){
				return builder->CreateSIToFP(val, type);
			}else{
				return builder->CreateUIToFP(val, type);
			}
		}else if(from->isFloatingPointTy() && !type->isFloatingPointTy()){
			// Float to Int
			return builder->CreateFPToSI(val, type);
		}else{
			// Int to Int
			return builder->CreateIntCast(val, type, isSigned);
		}
	}
	return val;
}

LLCodeGen::operands LLCodeGen::visitBinaryOp(AST::Expression* l, AST::Expression* r, ADT::Type& type) {
	l->accept(*this);
	llvm::Value* left = last_value;
	r->accept(*this);
	llvm::Value* right = last_value;
	type.translate(*this);
	left = typePromotion(left, translated_type, type.isSigned());
	right = typePromotion(right, translated_type, type.isSigned());
	return operands{left, right};
}

void LLCodeGen::visit(AST::Addition& v) {
	ADT::Type& type = v.getType();
	operands op = visitBinaryOp(v.left, v.right, type);
	if(type.isFloat()){
		last_value = builder->CreateFAdd(op.left, op.right);
	}else{
		last_value = builder->CreateAdd(op.left, op.right);
	}
}

void LLCodeGen::visit(AST::Array& v) {
}

void LLCodeGen::visit(AST::Assignment& v) {
	v.var->accept(*this);
	v.expression->accept(*this);
	namedValues[v.var->name] = last_value;
}

void LLCodeGen::visit(AST::BoolLiteral& v) {
	last_value = llvm::ConstantInt::get(*context, llvm::APInt(1, v.value ? 1 : 0));
}

void LLCodeGen::visit(AST::Conversion& v) {
}

void LLCodeGen::visit(AST::Division& v) {
	ADT::Type& type = v.getType();
	operands op = visitBinaryOp(v.left, v.right, type);
	if(type.isFloat()){
		last_value = builder->CreateFDiv(op.left, op.right);
	}else if(type.isSigned()){
		last_value = builder->CreateSDiv(op.left, op.right);
	}else{
		last_value = builder->CreateUDiv(op.left, op.right);
	}
}

void LLCodeGen::visit(AST::Equal& v) {
	v.left->accept(*this);
	llvm::Value* left = last_value;
	v.right->accept(*this);
	llvm::Value* right = last_value;
	ADT::Type& tleft = v.left->getType();
	ADT::Type& tright = v.right->getType();
	ADT::Type& type = tleft;
	if(tright.convertibleTo(tleft)){
		tleft.translate(*this);
		right = typePromotion(right, translated_type, tleft.isSigned());
	}else if(tleft.convertibleTo(tright)){
		tright.translate(*this);
		left = typePromotion(left, translated_type, tright.isSigned());
		type = tright;
	}
	if(type.isFloat()){
		last_value = builder->CreateFCmpOEQ(left, right);
	}else{
		last_value = builder->CreateICmpEQ(left, right);
	}
}

void LLCodeGen::visit(AST::Exponent& v) {
	v.left->accept(*this);
	llvm::Value* left = last_value;
	v.right->accept(*this);
	llvm::Value* right = last_value;
	auto id = llvm::Intrinsic::lookupIntrinsicID("llvm.pow.f64");
	builder->CreateBinaryIntrinsic(id, left, right);
}

void LLCodeGen::visit(AST::FloatLiteral& v) {
	last_value = llvm::ConstantFP::get(*context, llvm::APFloat(v.value));
}

void LLCodeGen::visit(AST::FunctionCall& v) {
	collect_values = true;
		v.params->accept(*this);
	collect_values = false;

	// Look up the name in the global module table.
	llvm::Function *callee = module->getFunction(v.name);
	last_value = builder->CreateCall(callee, values_list);
	values_list.clear();
}

void LLCodeGen::visit(AST::FunctionDeclaration& v) {
	// Make the function type
	inparams = true;
		v.params->accept(*this);
	inparams = false;
	ADT::Type& type = ADT::Type::findType(v.type);
	ret_type = &type;
	type.translate(*this);
	llvm::FunctionType *ft = llvm::FunctionType::get(translated_type, arg_types, false);
	llvm::Function *func = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, v.name, module.get());

	// Set names for all arguments.
	unsigned i = 0;
	for (auto &arg : func->args()){
		namedValues[arg_names[i]] = &arg;
		arg.setName(arg_names[i++]);
	}

	arg_types.clear();
	arg_names.clear();

	// Create a new basic block to start insertion into.
	llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "$entry", func);
	builder->SetInsertPoint(bb);
	v.body->accept(*this);
	//builder->CreateUnreachable();

	// Validate the generated code, checking for consistency.
	llvm::verifyFunction(*func);
}

void LLCodeGen::visit(AST::IfStatement& v) {
	v.condition->accept(*this);
	llvm::Value* condv = last_value;
	
	llvm::Function *func = builder->GetInsertBlock()->getParent();

	// Create blocks for the then and else cases.  Insert the 'then' block at the
	// end of the function.
	unsigned int count = getCount("if");
	std::stringstream ss;
	ss << count;
	llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(*context, std::string("if$")+ss.str(), func);
	llvm::BasicBlock *elseBB = llvm::BasicBlock::Create(*context, std::string("else$")+ss.str());
	llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(*context, std::string("if$cont")+ss.str());

	if(v.elsebody != nullptr){
		builder->CreateCondBr(condv, thenBB, elseBB);
	}else{
		builder->CreateCondBr(condv, thenBB, mergeBB);
	}

	// Emit then value.
	builder->SetInsertPoint(thenBB);
	v.body->accept(*this);

	if(!v.body->allPathsReturn()){
		builder->CreateBr(mergeBB);
	}
	// Codegen of 'Then' can change the current block, update ThenBB for the PHI.
	thenBB = builder->GetInsertBlock();

	if(v.elsebody != nullptr){
		// Emit else block.
		func->insert(func->end(), elseBB);
		builder->SetInsertPoint(elseBB);

		v.elsebody->accept(*this);

		if(!v.elsebody->allPathsReturn()){
			builder->CreateBr(mergeBB);
		}
	}

	// Codegen of 'Else' can change the current block, update ElseBB for the PHI.
	elseBB = builder->GetInsertBlock();

	// Emit merge block.
	func->insert(func->end(), mergeBB);
	builder->SetInsertPoint(mergeBB);
}

void LLCodeGen::visit(AST::IntegerLiteral& v) {
	last_value = llvm::ConstantInt::get(*context, llvm::APInt(v.getType().size()*8, v.value));
}

void LLCodeGen::visit(AST::List& v) {
	if(v.node != nullptr){
		islast = v.next == nullptr;
		v.node->accept(*this);
		if(collect_values){
			values_list.push_back(last_value);
		}
	}
	if(v.next != nullptr){
		v.next->accept(*this);
	}
}

void LLCodeGen::visit(AST::Member& v) {
	v.left->accept(*this);
	llvm::Value* var = last_value;
	ADT::Type& t = v.left->getType();
	t.translate(*this);
	last_value = builder->CreateStructGEP(translated_type, var, 0);
}

void LLCodeGen::visit(AST::Modulo& v) {
	ADT::Type& type = v.getType();
	operands op = visitBinaryOp(v.left, v.right, type);
	if(type.isFloat()){
		last_value = builder->CreateFRem(op.left, op.right);
	}else if(type.isSigned()){
		last_value = builder->CreateSRem(op.left, op.right);
	}else{
		last_value = builder->CreateURem(op.left, op.right);
	}
}

void LLCodeGen::visit(AST::Multiplication& v) {
	ADT::Type& type = v.getType();
	operands op = visitBinaryOp(v.left, v.right, type);
	if(type.isFloat()){
		last_value = builder->CreateFMul(op.left, op.right);
	}else{
		last_value = builder->CreateMul(op.left, op.right);
	}
}

void LLCodeGen::visit(AST::NotEqual& v) {
	ADT::Type& type = v.getType();
	operands op = visitBinaryOp(v.left, v.right, type);
	if(type.isFloat()){
		last_value = builder->CreateFCmpONE(op.left, op.right);
	}else{
		last_value = builder->CreateICmpNE(op.left, op.right);
	}
}

void LLCodeGen::visit(AST::Pointer& v) {
}

void LLCodeGen::visit(AST::Property& v) {
}

void LLCodeGen::visit(AST::Return& v) {
	v.expression->accept(*this);
	ret_type->translate(*this);
	llvm::Value* ret = typePromotion(last_value, translated_type, ret_type->isSigned());
	builder->CreateRet(ret);
}

void LLCodeGen::visit(AST::Subtraction& v) {
	ADT::Type& type = v.getType();
	operands op = visitBinaryOp(v.left, v.right, type);
	if(type.isFloat()){
		last_value = builder->CreateFSub(op.left, op.right);
	}else{
		last_value = builder->CreateSub(op.left, op.right);
	}
}

void LLCodeGen::visit(AST::TypeDeclaration& v) {
	inparams = true;
		v.list->accept(*this);
	inparams = false;

	llvm::StructType::create(*context, arg_types, v.name);

	arg_types.clear();
	arg_names.clear();
}

void LLCodeGen::visit(AST::UnitDeclaration& v) {
}

void LLCodeGen::visit(AST::Variable& v) {
	last_value = namedValues[v.name];
}

void LLCodeGen::visit(AST::VariableDeclaration& v) {
	if(inparams){
		ADT::Type& t = *v.table->findVariable(v.name)->type;
		t.translate(*this);
		arg_types.push_back(translated_type);
		arg_names.push_back(v.name);
	}else if(v.initial != nullptr){
		v.initial->accept(*this);
		namedValues[v.name] = last_value;
	}
}
