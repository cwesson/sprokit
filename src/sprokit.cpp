/**
 * @file sprokit.cpp
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 * Sprokit compiler.
 */

#include "AST.h"
#include "PrintAST.h"
#include "CollectSymbols.h"
#include "DimensionalAnalysis.h"
#include "TypeChecker.h"
#include "CppTranspiler.h"
#include <fstream>
#include <unistd.h>
#include "sprokit.h"

/**
 * Print help message to stdout.
 * @param all false to print only the usage messages, true to print options as well.
 */
static void printHelp(bool all){
	std::cout << "Usage: sprokit -h | -v" << std::endl;
	std::cout << "Usage: sprokit [-ps] [-o output] input" << std::endl;
	if(all){
		std::cout << std::endl;
		std::cout << "Options:"<< std::endl;
		std::cout << "  -h           Display help message and exit." << std::endl;
		std::cout << "  -o output    Write generated code to the specified output file." << std::endl;
		std::cout << "  -p           Print the abstract syntax tree." << std::endl;
		std::cout << "  -s           Print the symbol tables." << std::endl;
		std::cout << "  -v           Display version information and exit." << std::endl;
	}
}

/**
 * Main function for the Sprokit compiler.
 * @param argc Number of command line arguments.
 * @param argv Command line arguments.
 * @return Number of errors encountered.
 */
int main(int argc, char* argv[]) {
	const char* outfile = "out.cpp";
	bool print_ast = false;
	bool print_sym = false;
	int opt;
	while((opt = getopt(argc, argv, "ho:psv")) != -1){
		switch(opt){
			case 'h':
				printHelp(true);
				return 0;
			case 'o':
				outfile = optarg;
				break;
			case 'p':
				print_ast = true;
				break;
			case 's':
				print_sym = true;
				break;
			case 'v':
				std::cout << "Sprokit Compiler v0.0.1" << std::endl;
				std::cout << "Copyright (c) 2025 Conlan Wesson" << std::endl;
				std::cout << "GNU General Public License v3" << std::endl;
				return 0;
			case '?':
				printHelp(false);
				return 1;
			default:
				std::cerr << "Unknown option " << (char)opt << "." << std::endl;
				printHelp(false);
				return 1;
		}
	}
	if(optind >= argc){
		std::cerr << "No input file specified." << std::endl;
		printHelp(false);
		return 1;
	}

	AST::List* ast;
	int ret = parse(argv[optind], &ast);

	if(ast != nullptr){
		if(print_ast){
			PrintAST p;
			p.visit(*ast);
			std::cout << std::endl;
		}

		CollectSymbols c;
		c.visit(*ast);
		ret += c.error_count;
		if(print_sym){
			std::cout << *c.table;
		}

		DimensionalAnalysis d;
		d.visit(*ast);
		ret += d.error_count;

		TypeChecker t;
		t.visit(*ast);
		ret += t.error_count;

		std::ofstream cpp(outfile, std::ios::out);
		CppTranspiler g(cpp);
		g.visit(*ast);

		delete ast;
	}

	return ret;
}
