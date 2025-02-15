/**
 * @file UnitParser.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "Dimensions.h"
#include <string>

/**
 * Scanner for unit strings.
 * @ingroup dim
 */
class UnitScanner {
	public:
		/**
		 * Constructor.
		 * @param u Unit string.
		 */
		UnitScanner(const std::string& u);
		
		/**
		 * Get the next token in the unit string.
		 * @return Next token or empty string at the end of the unit string.
		 */
		std::string token();
	
	private:
		const std::string& unit; ///< Unit string being scanned.
		unsigned int i;          ///< Current index into the unit string.
};

/**
 * Parser for unit strings.
 * @ingroup dim
 */
class UnitParser {
	public:
		/**
		 * Constructor.
		 */
		UnitParser() = default;

		/**
		 * Parse a unit string.
		 * @param unit Unit string to parse.
		 * @return Dimensions of the unit.
		 */
		Dimensions parse(const std::string& unit);
};
