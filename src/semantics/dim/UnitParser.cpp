/**
 * @file UnitParser.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "UnitParser.h"

UnitScanner::UnitScanner(const std::string& u) :
	unit(u),
	i(0)
{}

std::string UnitScanner::token() {
	std::string tok;
	if(i > unit.length()){
		return tok;
	}

	for( ; i < unit.length(); ++i){
		char sym = unit[i];
		switch(sym){
			case '#':
			case '*':
			case '^':
			case '/':
				if(tok.length() == 0){
					tok += sym;
					++i;
				}
				return tok;
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				break;
			default:
				tok += sym;
				break;
		}
	}

	return tok;
}

Dimensions UnitParser::parse(const std::string& unit){
	UnitScanner scanner(unit);
	Dimensions dim;

	std::string last;
	bool add = true;
	for(std::string token = scanner.token(); token != ""; token = scanner.token()){
		if(token == "#" || token == "*" || token == "1"){
			continue;
		}else if(token == "/"){
			add = false;
		}else if(token == "^"){
			token = scanner.token();
			unsigned int exp = atoi(token.c_str());
			if(add){
				dim[last] += exp-1;  // already added once when the unit token was scanned
			}else{
				dim[last] -= exp-1;  // already added once when the unit token was scanned
			}
			
		}else{
			if(add){
				dim *= token;
			}else{
				dim /= token;
			}
			last = token;
		}
	}

	return dim;
}
