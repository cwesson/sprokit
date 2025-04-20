/**
 * @file Dimensions.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#include "Dimensions.h"

Dimensions& Dimensions::operator*=(const std::string& id) {
	if(dim.contains(id)){
		dim[id]++;
	}else{
		dim[id] = 1;
	}
	return *this;
}

Dimensions& Dimensions::operator/=(const std::string& id) {
	if(dim.contains(id)){
		dim[id]--;
	}else{
		dim[id] = -1;
	}
	return *this;
}

Dimensions& Dimensions::operator*=(const Dimensions& other) {
	for(auto d : other.dim){
		if(dim.contains(d.first)){
			dim[d.first] += d.second;
		}else{
			dim[d.first] = d.second;
		}
	}
	return *this;
}

Dimensions& Dimensions::operator/=(const Dimensions& other) {
	for(auto d : other.dim){
		if(dim.contains(d.first)){
			dim[d.first] -= d.second;
		}else{
			dim[d.first] = -d.second;
		}
	}
	return *this;
}

bool Dimensions::operator==(const Dimensions& other) const {
	for(auto d : other.dim){
		if(d.second != 0){
			if(dim.contains(d.first)){
				if(d.second != dim.at(d.first)){
					return false;
				}
			}else{
				return false;
			}
		}
	}
	for(const auto& d : dim){
		if(d.second != 0){
			if(other.dim.contains(d.first)){
				if(d.second != other.dim.at(d.first)){
					return false;
				}
			}else{
				return false;
			}
		}
	}
	return true;
}

bool Dimensions::operator!=(const Dimensions& other) const {
	return !(*this == other);
}

int& Dimensions::operator[](const std::string& i) {
	if(!dim.contains(i)){
		dim[i] = 0;
	}
	return dim[i];
}

Dimensions::operator std::string() const {
	std::string unit("#");
	bool first = true;
	for(auto d : dim){
		if(d.second > 0){
			if(!first){
				unit += "*";
			}
			first = false;
			unit += d.first;
			if(d.second > 1){
				unit += "^" + std::to_string(d.second);
			}
		}
	}
	if(first){
		unit += "1";
	}
	first = true;
	for(auto d : dim){
		if(d.second < 0){
			if(first){
				unit += "/";
				first = false;
			}else{
				unit += "*";
			}
			unit += d.first;
			if(d.second < -1){
				unit += "^" + std::to_string(-d.second);
			}
		}
	}
	return unit;
}
