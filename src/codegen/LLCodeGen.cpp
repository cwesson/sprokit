/**
 * @file LLCodeGen.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "LLCodeGen.h"
#include "PrimitiveType.h"
#include "StructType.h"
#include "TypeDecorator.h"
#include "sym/TypeSymbols.h"
#include <llvm/IR/Verifier.h>
#include <llvm/Transforms/Utils/ModuleUtils.h>
#include <sstream>

LLCodeGen::LLCodeGen(std::ostream& o, const char* filename) :
	os(o),
	inparams(false),
	islast(false),
	collect_values(false),
	context(std::make_unique<llvm::LLVMContext>()),
	module(),
	builder(),
	globalValues(),
	allocaValues(),
	last_value(nullptr),
	last_ptr(nullptr),
	translated_type(nullptr),
	arg_types(),
	arg_names(),
	counters(),
	values_list(),
	in_func(nullptr),
	ret_type(nullptr),
	expect_type(nullptr)
{
	// Open a new context and module.
	module = std::make_unique<llvm::Module>(filename, *context);

	// Create a new builder for the module.
	builder = std::make_unique<llvm::IRBuilder<>>(*context);
	llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "$global");
	builder->SetInsertPoint(bb);
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

std::string LLCodeGen::translateType(const ADT::StructType& t) const {
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
	return operands{left, right, &type};
}

LLCodeGen::operands LLCodeGen::visitCompareOp(AST::Expression* l, AST::Expression* r) {
	l->accept(*this);
	llvm::Value* left = last_value;
	r->accept(*this);
	llvm::Value* right = last_value;
	ADT::Type& tleft = l->getType();
	ADT::Type& tright = r->getType();
	ADT::Type& type = tleft;
	if(tright.convertibleTo(tleft)){
		tleft.translate(*this);
		right = typePromotion(right, translated_type, tleft.isSigned());
	}else if(tleft.convertibleTo(tright)){
		tright.translate(*this);
		left = typePromotion(left, translated_type, tright.isSigned());
		type = tright;
	}
	return operands{left, right, &type};
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
	llvm::Value* alloca = last_value;
	v.expression->accept(*this);
	ADT::Type& t = v.var->getType();
	t.translate(*this);
	last_value = typePromotion(last_value, translated_type, t.isSigned());
	builder->CreateStore(last_value, alloca);
}

void LLCodeGen::visit(AST::BitAnd& v) {
	ADT::Type& type = v.getType();
	operands op = visitBinaryOp(v.left, v.right, type);
	last_value = builder->CreateAnd(op.left, op.right);
}

void LLCodeGen::visit(AST::BitNot& v) {
	v.right->accept(*this);
	last_value = builder->CreateNot(last_value);
}

void LLCodeGen::visit(AST::BitOr& v) {
	ADT::Type& type = v.getType();
	operands op = visitBinaryOp(v.left, v.right, type);
	last_value = builder->CreateOr(op.left, op.right);
}

void LLCodeGen::visit(AST::BitXor& v) {
	ADT::Type& type = v.getType();
	operands op = visitBinaryOp(v.left, v.right, type);
	last_value = builder->CreateXor(op.left, op.right);
}

void LLCodeGen::visit(AST::BoolAnd& v) {
	v.left->accept(*this);
	llvm::Value* left = last_value;

	unsigned int count = getCount("and");
	std::stringstream ss;
	ss << count;
	// Make the new basic block for the loop header, inserting after current block.
	llvm::Function *func = builder->GetInsertBlock()->getParent();
	llvm::BasicBlock *trueBB = llvm::BasicBlock::Create(*context, std::string("$and$true")+ss.str(), func);
	llvm::BasicBlock *falseBB = llvm::BasicBlock::Create(*context, std::string("$and$false")+ss.str(), func);
	llvm::BasicBlock *endBB = llvm::BasicBlock::Create(*context, std::string("$and$end")+ss.str(), func);

	builder->CreateCondBr(left, trueBB, falseBB);

	// Continue if true
	builder->SetInsertPoint(trueBB);

	v.right->accept(*this);
	llvm::Value* right = last_value;
	builder->CreateBr(endBB);

	// Skip if false
	builder->SetInsertPoint(falseBB);
	builder->CreateBr(endBB);

	builder->SetInsertPoint(endBB);

	llvm::PHINode *phi = builder->CreatePHI(llvm::IntegerType::get(*context, 1), 2);
	phi->addIncoming(right, trueBB);
	phi->addIncoming(left, falseBB);
	last_value = phi;
}

void LLCodeGen::visit(AST::BoolLiteral& v) {
	last_value = llvm::ConstantInt::get(*context, llvm::APInt(1, v.value ? 1 : 0));
}

void LLCodeGen::visit(AST::BoolNot& v) {
	v.right->accept(*this);
	last_value = builder->CreateNot(last_value);
}

void LLCodeGen::visit(AST::BoolOr& v) {
	v.left->accept(*this);
	llvm::Value* left = last_value;

	unsigned int count = getCount("or");
	std::stringstream ss;
	ss << count;
	// Make the new basic block for the loop header, inserting after current block.
	llvm::Function *func = builder->GetInsertBlock()->getParent();
	llvm::BasicBlock *trueBB = llvm::BasicBlock::Create(*context, std::string("$or$true")+ss.str(), func);
	llvm::BasicBlock *falseBB = llvm::BasicBlock::Create(*context, std::string("$or$false")+ss.str(), func);
	llvm::BasicBlock *endBB = llvm::BasicBlock::Create(*context, std::string("$or$end")+ss.str(), func);

	builder->CreateCondBr(left, trueBB, falseBB);

	// Continue if false
	builder->SetInsertPoint(falseBB);

	v.right->accept(*this);
	llvm::Value* right = last_value;
	builder->CreateBr(endBB);

	// Skip if true
	builder->SetInsertPoint(trueBB);
	builder->CreateBr(endBB);

	builder->SetInsertPoint(endBB);

	llvm::PHINode *phi = builder->CreatePHI(llvm::IntegerType::get(*context, 1), 2);
	phi->addIncoming(right, falseBB);
	phi->addIncoming(left, trueBB);
	last_value = phi;
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
	operands op = visitCompareOp(v.left, v.right);
	if(op.type->isFloat()){
		last_value = builder->CreateFCmpOEQ(op.left, op.right);
	}else{
		last_value = builder->CreateICmpEQ(op.left, op.right);
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

void LLCodeGen::visit(AST::ForStatement& v) {
	// Emit the start code first, without 'variable' in scope.
	if(v.init != nullptr){
		v.init->accept(*this);
	}

	unsigned int count = getCount("for");
	std::stringstream ss;
	ss << count;
	// Make the new basic block for the loop header, inserting after current block.
	llvm::Function *func = builder->GetInsertBlock()->getParent();
	llvm::BasicBlock *loopBB = llvm::BasicBlock::Create(*context, std::string("for$")+ss.str(), func);
	llvm::BasicBlock *contBB = llvm::BasicBlock::Create(*context, std::string("for$cont")+ss.str(), func);
	// Create the "after loop" block and insert it.
	llvm::BasicBlock *afterBB = llvm::BasicBlock::Create(*context, std::string("for$end")+ss.str(), func);

	// Insert an explicit fall through from the current block to the loop.
	builder->CreateBr(loopBB);

	// Start insertion in loop.
	builder->SetInsertPoint(loopBB);

	v.condition->accept(*this);
	builder->CreateCondBr(last_value, contBB, afterBB);

	builder->SetInsertPoint(contBB);
	v.body->accept(*this);
	if(!v.body->allPathsReturn()){
		if(v.increment != nullptr){
			v.increment->accept(*this);
		}
		builder->CreateBr(loopBB);
	}

	// Any new code will be inserted in AfterBB.
	builder->SetInsertPoint(afterBB);
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
	std::map<std::string, llvm::Value*> argValues;
	unsigned i = 0;
	for (auto &arg : func->args()){
		argValues[arg_names[i]] = &arg;
		arg.setName(arg_names[i++]);
	}

	arg_types.clear();
	arg_names.clear();

	// Create a new basic block to start insertion into.
	llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "$entry", func);
	builder->SetInsertPoint(bb);
	in_func = func;
		for (auto &arg : func->args()){
			std::string name(arg.getName().str());
			allocaValues[name] = builder->CreateAlloca(argValues[name]->getType(), nullptr, name);
			builder->CreateStore(argValues[name], allocaValues[name]);
		}

		v.body->accept(*this);
		builder->CreateUnreachable();

		// Validate the generated code, checking for consistency.
		llvm::verifyFunction(*func);
	in_func = nullptr;
}

void LLCodeGen::visit(AST::GreaterEqual& v) {
	operands op = visitCompareOp(v.left, v.right);
	if(op.type->isFloat()){
		last_value = builder->CreateFCmpOGE(op.left, op.right);
	}else if(op.type->isSigned()){
		last_value = builder->CreateICmpSGE(op.left, op.right);
	}else{
		last_value = builder->CreateICmpUGE(op.left, op.right);
	}
}

void LLCodeGen::visit(AST::GreaterThan& v) {
	operands op = visitCompareOp(v.left, v.right);
	if(op.type->isFloat()){
		last_value = builder->CreateFCmpOGT(op.left, op.right);
	}else if(op.type->isSigned()){
		last_value = builder->CreateICmpSGT(op.left, op.right);
	}else{
		last_value = builder->CreateICmpUGT(op.left, op.right);
	}
}

void LLCodeGen::visit(AST::IfStatement& v) {
	if(v.init != nullptr){
		v.init->accept(*this);
	}

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
	llvm::BasicBlock *mergeBB = llvm::BasicBlock::Create(*context, std::string("if$end")+ss.str());

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

	if(!v.allPathsReturn()){
		// Emit merge block.
		func->insert(func->end(), mergeBB);
		builder->SetInsertPoint(mergeBB);
	}
}

void LLCodeGen::visit(AST::IntegerLiteral& v) {
	last_value = llvm::ConstantInt::get(*context, llvm::APInt(v.getType().size()*8, v.value));
}

void LLCodeGen::visit(AST::LessEqual& v) {
	operands op = visitCompareOp(v.left, v.right);
	if(op.type->isFloat()){
		last_value = builder->CreateFCmpOLE(op.left, op.right);
	}else if(op.type->isSigned()){
		last_value = builder->CreateICmpSLE(op.left, op.right);
	}else{
		last_value = builder->CreateICmpULE(op.left, op.right);
	}
}

void LLCodeGen::visit(AST::LessThan& v) {
	operands op = visitCompareOp(v.left, v.right);
	if(op.type->isFloat()){
		last_value = builder->CreateFCmpOLT(op.left, op.right);
	}else if(op.type->isSigned()){
		last_value = builder->CreateICmpSLT(op.left, op.right);
	}else{
		last_value = builder->CreateICmpULT(op.left, op.right);
	}
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
	TypeSymbols* type_table = v.table->findType(t);
	unsigned int index = 0;
	for(auto mem : type_table->vars){
		if(mem.first == v.right->name){
			break;
		}
		++index;
	}
	t.translate(*this);
	last_value = builder->CreateStructGEP(translated_type, var, index);
}

void LLCodeGen::visit(AST::MemberInitialization& v) {
	llvm::Value* var = last_value;
	v.initial->accept(*this);
	llvm::Value* init = last_value;
	TypeSymbols* type_table = v.table->findType(*expect_type);
	unsigned int index = 0;
	ADT::Type* memtype = nullptr;
	for(auto mem : type_table->vars){
		if(mem.first == v.name){
			memtype = mem.second->type;
			break;
		}
		++index;
	}
	expect_type->translate(*this);
	llvm::Value* gep = builder->CreateStructGEP(translated_type, var, index);
	memtype->translate(*this);
	init = typePromotion(init, translated_type, memtype->isSigned());
	builder->CreateStore(init, gep);
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

void LLCodeGen::visit(AST::Negation& v) {
	ADT::Type& type = v.getType();
	v.right->accept(*this);
	if(type.isFloat()){
		last_value = builder->CreateFNeg(last_value);
	}else{
		last_value = builder->CreateNeg(last_value);
	}
}

void LLCodeGen::visit(AST::NotEqual& v) {
	operands op = visitCompareOp(v.left, v.right);
	if(op.type->isFloat()){
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

void LLCodeGen::visit(AST::ShiftLeft& v) {
	v.left->accept(*this);
	llvm::Value* left = last_value;
	v.right->accept(*this);
	llvm::Value* right = last_value;
	last_value = builder->CreateShl(left, right);
}

void LLCodeGen::visit(AST::ShiftRight& v) {
	v.left->accept(*this);
	llvm::Value* left = last_value;
	v.right->accept(*this);
	llvm::Value* right = last_value;
	if(v.left->getType().isSigned()){
		last_value = builder->CreateAShr(left, right);
	}else{
		last_value = builder->CreateLShr(left, right);
	}
}

void LLCodeGen::visit(AST::StructInitializer& v) {
	llvm::Value* alloca = last_value;
	for(AST::List* head = v.list; head != nullptr && head->node != nullptr; head = head->next){
		last_value = alloca;
		head->node->accept(*this);
	}
	last_value = nullptr;
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
	if(allocaValues.contains(v.name)){
		last_value = allocaValues[v.name];
	}else{
		last_value = globalValues[v.name];
	}
}

void LLCodeGen::visit(AST::VariableDeclaration& v) {
	ADT::Type& t = *v.table->findVariable(v.name)->type;
	expect_type = &t;
	t.translate(*this);
	if(inparams){
		arg_types.push_back(translated_type);
		arg_names.push_back(v.name);
	}else if(in_func != nullptr){
		llvm::AllocaInst* alloca = builder->CreateAlloca(translated_type, nullptr, v.name);
		allocaValues[v.name] = alloca;
		last_ptr = alloca;
		last_value = alloca;
		v.initial->accept(*this);
		if(last_value != nullptr){
			builder->CreateStore(last_value, allocaValues[v.name]);
		}
	}else if(v.initial != nullptr){
		llvm::GlobalVariable* global = new llvm::GlobalVariable(translated_type, v.constant, llvm::GlobalValue::InternalLinkage, llvm::Constant::getNullValue(translated_type), v.name);
		module->insertGlobalVariable(global);
		globalValues[v.name] = global;
		last_ptr = global;

		std::stringstream ss;
		ss << "$ctor$" << v.name;
		llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false);
		llvm::Function* ctor = llvm::Function::Create(ft, llvm::Function::InternalLinkage, ss.str(), module.get());
		llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "$entry", ctor);
		builder->SetInsertPoint(bb);

		in_func = ctor;
			v.initial->accept(*this);
			if(last_value != nullptr){
				builder->CreateStore(last_value, globalValues[v.name]);
			}
			builder->CreateRetVoid();
			builder->CreateUnreachable();

			// Validate the generated code, checking for consistency.
			llvm::verifyFunction(*ctor);
		in_func = nullptr;
		llvm::appendToGlobalCtors(*module, ctor, 0);
	}
	expect_type = nullptr;
}

void LLCodeGen::visit(AST::VariableLoad& v) {
	v.var->accept(*this);
	llvm::Type* t = last_value->getType();
	if(t->isPointerTy()){
		ADT::Type& type = v.getType();
		type.translate(*this);
		last_value = builder->CreateLoad(translated_type, last_value, v.var->name);
	}
}

void LLCodeGen::visit(AST::WithStatement& v) {
	unsigned int count = getCount("with");
	std::stringstream ss;
	ss << count;
	// Make the new basic block for the loop header, inserting after current block.
	llvm::Function *func = builder->GetInsertBlock()->getParent();
	llvm::BasicBlock *withBB = llvm::BasicBlock::Create(*context, std::string("with$")+ss.str(), func);
	llvm::BasicBlock *endBB = llvm::BasicBlock::Create(*context, std::string("with$end")+ss.str(), func);

	// Insert an explicit fall through from the current block to the with.
	builder->CreateBr(withBB);
	// Start insertion in with.
	builder->SetInsertPoint(withBB);

	v.init->accept(*this);
	v.body->accept(*this);

	// Insert an explicit fall through from the with.
	builder->CreateBr(endBB);
	// Any new code will be inserted in endBB.
	builder->SetInsertPoint(endBB);
}
