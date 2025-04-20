/**
 * @file sprokit.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include "AST.h"

/**
 * Parse the given file.
 * @param file Path to file to parse.
 * @param[out] ast AST generated during parsing.
 * @return Number of errors that occurred.
 */
int parse(const char* file, AST::List** ast);
