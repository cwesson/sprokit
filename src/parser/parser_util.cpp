/**
 * @file parser_util.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "parser_util.h"
#include <string>

bool parser::parse_int(const char* str, unsigned long long* out) {
	if(out == nullptr){
		return false;
	}

	*out = 0;
	int base = 10;
	if(str[0] == '0'){
		switch(str[1]){
			case 'b':
				base = 2;
				str += 2;
				break;
			case 'o':
				base = 8;
				str += 2;
				break;
			case 'x':
				base = 16;
				str += 2;
				break;
			default:
				break;
		}
	}

	for(auto c : std::string(str)){
		unsigned long long prev = *out;
		*out *= base;
		if(*out < prev){
			return false; // overflow
		}
		unsigned int v = 0;
		switch(base) {
			case 2:
				if(c < '0' || c > '1'){
					return false;
				}else{
					v = c - '0';
				}
				break;
			case 8:
				if(c < '0' || c > '7'){
					return false;
				}else{
					v = c - '0';
				}
				break;
			case 10:
				if(c < '0' || c > '9'){
					return false;
				}else{
					v = c - '0';
				}
				break;
			case 16:
				if(c >= '0' && c <= '9'){
					v = c - '0';
				}else if(c >= 'a' && c <= 'f'){
					v = c - 'a';
				}else if(c >= 'A' && c <= 'F'){
					v = c - 'A';
				}else{
					return false;
				}
				break;
			default:
				return false;
		}
		*out += v;
		if(*out < prev){
			return false; // overflow
		}
	}
	return true;
}