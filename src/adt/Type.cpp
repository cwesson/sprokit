/**
 * @file Type.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "Type.h"
#include "PrimitiveType.h"
#include "UnknownType.h"
#include "StructType.h"
#include "TypeDecorator.h"

namespace ADT {

Type::TypeTable Type::table;

Type::TypeTable::TypeTable() :
	table()
{}

Type::TypeTable::~TypeTable(){
	for(std::pair<std::string, Type*> t : table){
		delete t.second;
	}
}

template<typename TYPE, typename... ARGS>
void Type::TypeTable::insert(ARGS... args){
	Type* type = new TYPE(args...);
	table[type->name] = type;
}

bool Type::TypeTable::empty(){
	return table.empty();
}

bool Type::TypeTable::contains(const std::string& name){
	return table.contains(name);
}

Type* Type::TypeTable::at(const std::string& name){
	if(table.contains(name)){
		return table.at(name);
	}else{
		return nullptr;
	}
}

Type::Type(const std::string& n) :
	name(n)
{}

void Type::init(){
	if(table.empty()){
		table.insert<UnknownType>();
		table.insert<BoolType>();
		table.insert<IntType>(  8, true);
		table.insert<IntType>( 16, true);
		table.insert<IntType>( 32, true);
		table.insert<IntType>( 64, true);
		table.insert<IntType>(128, true);
		table.insert<IntType>(  8, false);
		table.insert<IntType>( 16, false);
		table.insert<IntType>( 32, false);
		table.insert<IntType>( 64, false);
		table.insert<IntType>(128, false);
		table.insert<FloatType>( 5,  11); // float16
		table.insert<FloatType>( 8,  24); // float32
		table.insert<FloatType>(11,  53); // float64
		table.insert<FloatType>(15, 113); // float128
	}
}

Type& Type::findType(const std::string& type) {
	init();

	if(!table.contains(type)){
		table.insert<StructType>(type);
	}
	return *table.at(type);
}

Type& Type::findPointerType(const std::string& type) {
	init();

	Type& base = findType(type);
	std::string ptr(base.name + std::string("@"));
	if(!table.contains(ptr)){
		table.insert<PointerType, Type&>(base);
	}

	return *table.at(ptr);
}

StructType* Type::createType(const std::string& type){
	init();

	if(!table.contains(type)){
		table.insert<StructType>(type);
		return static_cast<StructType*>(table.at(type));
	}else{
		return nullptr;
	}
}

Type::operator std::string() const {
	return name;
}

}