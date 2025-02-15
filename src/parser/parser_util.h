/**
 * @file parser_util.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

/**
 * @defgroup parser Parser
 * @details The parser is mostly implemented in Bison, see sprokit.y.
 * @{
 */

/**
 * Sprokit parser
 */
namespace parser {

/**
 * Parse an integer literal.
 * @param str Input string.
 * @param[out] out Parsed integer value.
 * @return true if successful, false if an error occurs.
 */
bool parse_int(const char* str, unsigned long long* out);

}

/** @} */
