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
#include "LLCodeGen.h"
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
#include "sprokit.h"

/**
 * Run a command via exec.
 * @param _cmd Command to run.
 * @param _args Arguments to the command.
 * @note This is a macro in order to pass variadic arguments without expanding.
 * `execvp()` doesn't work because the argv parameter is not const, as is
 * necessary to allow `c_str()`.
 */
#define runToolchain(_cmd, _args...) do {   \
	pid_t _p = fork();                      \
	if(_p == 0){                            \
		execlp(_cmd, _cmd, _args, nullptr); \
	}else if(_p > 0){                       \
		int _s = 0;                         \
		waitpid(_p, &_s, 0);                \
		if(WEXITSTATUS(_s) != 0){           \
			std::cerr << "Error running " << _cmd << ": " << WEXITSTATUS(_s) << "." << std::endl; \
			return 1;                       \
		}                                   \
	}else{                                  \
		std::cerr << "Error running " << _cmd << "." << std::endl; \
		printHelp(false);                   \
		return 1;                           \
	}                                       \
} while(0)

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
	const char* outfile = "a.out";
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
	const char* infile = argv[optind];
	unsigned int ecount = parse(infile, &ast);

	if(ast != nullptr){
		if(print_ast){
			PrintAST p;
			p.visit(*ast);
			std::cout << std::endl;
		}

		CollectSymbols c;
		c.visit(*ast);
		ecount += c.error_count;
		if(print_sym){
			std::cout << *c.global;
		}

		DimensionalAnalysis d;
		d.visit(*ast);
		ecount += d.error_count;

		TypeChecker t;
		t.visit(*ast);
		ecount += t.error_count;

		if(ecount == 0){
			std::filesystem::path path(outfile);
			std::string ext = path.extension();

			if(ext == ".cpp"){
				std::ofstream cpp(outfile, std::ios::out);
				CppTranspiler g(cpp);
				g.visit(*ast);
			}else{
				enum {
					STAGE_LL,
					STAGE_ASM,
					STAGE_COMP,
				} stages;

				std::filesystem::path llfile;
				std::filesystem::path sfile;
				std::filesystem::path compfile;
				if(ext == ".ll"){
					stages = STAGE_LL;
					llfile = outfile;
				}else if(ext == ".s" || ext == ".S"){
					stages = STAGE_ASM;
					llfile = std::string(outfile) + ".ll";
					sfile = outfile;
				}else{
					stages = STAGE_COMP;
					llfile = std::string(outfile) + ".ll";
					sfile = std::string(outfile) + ".s";
					compfile = outfile;
				}

				if(stages >= STAGE_LL){
					std::ofstream ll(llfile, std::ios::out);
					LLCodeGen g(ll, infile);
					g.visit(*ast);
				}
				if(stages >= STAGE_ASM){
					runToolchain("llc", "-o", sfile.c_str(), llfile.c_str());
				}
				if(stages >= STAGE_COMP){
					runToolchain("clang", "-o", compfile.c_str(), sfile.c_str());
				}
			}
		}

		delete ast;
	}

	return (ecount != 0 ? -1 : 0);
}
