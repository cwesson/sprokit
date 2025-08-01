/**
 * @file Type.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "Type.h"
#include "PrimitiveType.h"
#include "UnknownType.h"
#include "UserType.h"
#include "TypeDecorator.h"

namespace ADT {

std::map<std::string, Type*> Type::table;

Type::Type(const std::string& n) :
	name(n)
{}

void Type::add(Type* type){
	table[type->name] = type;
}

void Type::init(){
	if(table.empty()){
		add(new UnknownType());
		add(new BoolType());
		add(new IntType( 8, true));
		add(new IntType(16, true));
		add(new IntType(32, true));
		add(new IntType(64, true));
		add(new IntType( 8, false));
		add(new IntType(16, false));
		add(new IntType(32, false));
		add(new IntType(64, false));
		add(new FloatType( 5, 11));
		add(new FloatType( 8, 24));
		add(new FloatType(11, 53));
	}
}

Type& Type::findType(const std::string& type) {
	init();

	if(!table.contains(type)){
		add(new UserType(type));
	}
	return *table[type];
}

Type& Type::findPointerType(const std::string& type) {
	init();

	Type& base = findType(type);
	std::string ptr(base.name + std::string("@"));
	if(!table.contains(ptr)){
		add(new PointerType(base));
	}

	return *table[ptr];
}

UserType* Type::createType(const std::string& type){
	init();

	if(!table.contains(type)){
		UserType* u = new UserType(type);
		add(u);
		return u;
	}else{
		return nullptr;
	}
}

Type::operator std::string() const {
	return name;
}

}