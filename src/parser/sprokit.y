%language "c++"

%{
#include "parser_util.h"
#include "AST.h"
#include <cstdlib>
using namespace AST;
using namespace ADT;
#include "sprokit.tab.hh"

extern unsigned int yylineno;
int yylex(yy::parser::value_type* yylval, yy::parser::location_type* yylloc);
void yyset_in(FILE*);

AST::List* program_ast = nullptr;
%}

%locations

%token <literal> INTEGER FLOAT STRING
%token <symbol> UNIT_ID
%token <symbol> ID

%token CONST VAR FUNC TYPE UNIT
%token RETURN
%token OPERATOR
%token IF ELSE FOR BREAK CONTINUE YIELD SWITCH CASE DEFAULT WITH
%token TRUE FALSE

%token LBRACE RBRACE
%token SEMICOLON
%token COMMA
%token COLON
%token PROPERTY
%token ASSIGN

%token LPAREN RPAREN
%left QUESTION COLON
%left EQUAL NEQUAL GREATER LESSER GTEQUAL LTEQUAL
%left BOOLOR
%left BOOLAND
%right BOOLNOT
%left LSHIFT RSHIFT
%left BITOR BITXOR
%left BITAND
%right BITNOT
%left PLUS MINUS
%left TIMES DIVIDE MOD
%left EXPONENT
%right POINTER
%left LBRACKET RBRACKET
%left MEMBER

%token SCANNERERROR

%type <list> program
%type <list> statement_list
%type <node> statement
%type <node> block_statement
%type <node> if_statement
%type <node> for_statement
%type <node> switch_statement
%type <node> case_statement
%type <list> case_list
%type <list> paramdecl_list
%type <list> paramdecl_cont
%type <symbol> unit_id
%type <var_decl> variable_decl
%type <var> variable_access
%type <arr> array_access
%type <arr> array_decl
%type <list> member_list
%type <node> member
%type <list> unitmember_list
%type <node> unitmember
%type <list> declaration_list
%type <node> declaration
%type <node> funcdecl
%type <exp> expression
%type <list> expression_list
%type <list> expression_cont
%type <type> type_spec

%union{
	const char* literal;
	const char* symbol;
	class ASTNode* node;
	class Expression* exp;
	class List* list;
	class VariableDeclaration* var_decl;
	class Variable* var;
	class Array* arr;
	class Type* type;
}

%%

program:
	declaration_list {
		$$ = $1;
		program_ast = $$;
	}
;

declaration_list:
	declaration declaration_list {
		$$ = new List($1);
		$$->next = $2;
	}
	| /* empty */ {
		$$ = new List(nullptr);
	}
;

statement_list:
	statement statement_list {
		$$ = new List($1);
		$$->next = $2;
	}
	| /* empty */ {
		$$ = new List(nullptr);
	}
;

statement:
	declaration {
		$$ = $1;
	}
	| variable_access[var] ASSIGN expression[value] SEMICOLON {
		$$ = new Assignment(@2.begin.line, $var, $value);
	}
	| variable_access[var] ASSIGN block_statement[value] SEMICOLON {
		
	}
	| block_statement {
		$$ = $1;
	}
	| RETURN expression SEMICOLON {
		$$ = new Return(@1.begin.line, $2);
	}
	| BREAK SEMICOLON {
		
	}
	| BREAK ID[id] SEMICOLON {
		
	}
	| CONTINUE SEMICOLON {
		
	}
	| CONTINUE ID[id] SEMICOLON {
		
	}
	| YIELD expression[exp] SEMICOLON {
		
	}
	| WITH declaration[dec] LBRACE statement_list[body] RBRACE {
		
	}
;

block_statement:
	if_statement {
		$$ = $1;
	}
	| for_statement {
		$$ = $1;
	}
	| switch_statement {
		$$ = $1;
	}

if_statement:
	IF expression[cond] LBRACE statement_list[body] RBRACE {
		$$ = new IfStatement(@1.begin.line, $cond, $body, nullptr);
	}
	| IF expression[cond] LBRACE statement_list[body] RBRACE ELSE LBRACE statement_list[alt] RBRACE {
		$$ = new IfStatement(@1.begin.line, $cond, $body, $alt);
	}
	| IF expression[cond] LBRACE statement_list[body] RBRACE ELSE if_statement[alt] {
		$$ = new IfStatement(@1.begin.line, $cond, $body, new List($alt));
	}
;

for_statement:
	FOR variable_decl[init] SEMICOLON expression[cond] SEMICOLON ID[id] ASSIGN expression[inc] LBRACE statement_list[body] RBRACE {

	}
	| FOR expression[cond] LBRACE statement_list[body] RBRACE {

	}
;

switch_statement:
	SWITCH expression[exp] LBRACE case_list[list] RBRACE {

	}
;

case_list:
	case_statement case_list {
		$$ = new List($1);
		$$->next = $2;
	}
	| /* empty */ {
		$$ = new List(nullptr);
	}
;

case_statement:
	CASE expression[exp] LBRACE statement_list[list] RBRACE {

	}
	| DEFAULT LBRACE statement_list[list] RBRACE {

	}
;


unit_id:
	UNIT_ID {
		$$ = $1;
	}
	| /* empty */ {
		$$ = "";
	}
;

type_spec:
	type_spec array_decl {

	}
	| type_spec[ext] POINTER {
		$$ = ADT::Type::findPointerType($ext->name);
	}
	| ID[type] {
		$$ = ADT::Type::findType($type);
	}
;

variable_decl:
	CONST ID[name] unit_id[unit] COLON ID[type] array_decl[arr] {
		auto d = new VariableDeclaration(@1.begin.line, $name, $type, $unit, $arr);
		d->constant = true;
		$$ = d;
	}
	| VAR ID[name] unit_id[unit] COLON ID[type] array_decl[arr] {
		auto d = new VariableDeclaration(@1.begin.line, $name, $type, $unit, $arr);
		d->constant = false;
		$$ = d;
	}
	| CONST ID[name] unit_id[unit] COLON ID[type] POINTER array_decl[arr] {
		auto d = new VariableDeclaration(@1.begin.line, $name, $type, $unit, $arr);
		d->constant = true;
		d->pointer = true;
		$$ = d;
	}
	| VAR ID[name] unit_id[unit] COLON ID[type] POINTER array_decl[arr] {
		auto d = new VariableDeclaration(@1.begin.line, $name, $type, $unit, $arr);
		d->constant = false;
		d->pointer = true;
		$$ = d;
	}
;

array_decl:
	LBRACKET expression[index] RBRACKET array_decl[next] {
		$$ = new Array(@1.begin.line, $index, $next);
	}
	|
	LBRACKET CONST ID[name] COLON ID[type] RBRACKET array_decl[next] {
		$$ = new Array(@1.begin.line, new VariableDeclaration(@name.begin.line, $name, $type, "#1", nullptr), $next);
	}
	|
	LBRACKET RBRACKET array_decl[next] {
		$$ = new Array(@1.begin.line, $next);
	}
	| /* empty */ {
		$$ = nullptr;
	}
;

paramdecl_list:
	paramdecl_cont {
		$$ = $1;
	}
	| /* empty */ {
		$$ = new List(nullptr);
	}
;

paramdecl_cont:
	variable_decl {
		$$ = new List($1);
	}
	| paramdecl_cont COMMA variable_decl {
		$$ = $1;
		auto l = new List($3);
		auto i = $$;
		while (i->next != nullptr){
			i = i->next;
		}
		i->next = l;
	}
;

member_list:
	member member_list {
		$$ = new List($1);
		$$->next = $2;
	}
	| /* empty */ {
		$$ = new List(nullptr);
	}
;

member:
	variable_decl SEMICOLON {
		$$ = $1;
	}
	| funcdecl {
		$$ = $1;
	}
;

unitmember_list:
	unitmember SEMICOLON unitmember_list {
		$$ = new List($1);
		$$->next = $3;
	}
	| /* empty */ {
		$$ = new List(nullptr);
	}
;

unitmember:
	OPERATOR UNIT_ID[unit] LPAREN CONST ID[name] RPAREN ASSIGN expression[exp] {
		$$ = new Conversion(yylineno, $name, $unit, $exp);
	}
;

declaration:
	variable_decl[var] ASSIGN expression[value] SEMICOLON {
		$$ = $var;
		$var->initial = $value;
	}
	| variable_decl[var] ASSIGN statement[value] SEMICOLON {
		
	}
	| variable_decl[var] ASSIGN LBRACE expression_list[value] RBRACE SEMICOLON {
		
	}
	| funcdecl {
		$$ = $1;
	}
	| TYPE ID[type] LBRACE member_list[list] RBRACE {
		$$ = new TypeDeclaration(@1.begin.line, $type, $list);
	}
	| UNIT UNIT_ID[unit] LBRACE unitmember_list[list] RBRACE {
		$$ = new UnitDeclaration(@1.begin.line, $unit, $list, "");
	}
	| UNIT UNIT_ID[unit] COLON UNIT_ID[alias] LBRACE unitmember_list[list] RBRACE {
		$$ = new UnitDeclaration(@1.begin.line, $unit, $list, $alias);
	}
;

funcdecl:
	FUNC ID[name] LPAREN paramdecl_list[param] RPAREN unit_id[unit] COLON ID[type] LBRACE statement_list[body] RBRACE {
		auto d = new FunctionDeclaration(@1.begin.line, $name, $type, $unit, $body);
		d->params = $param;
		$$ = d;
	}
	| FUNC ID[name] LPAREN paramdecl_list[param] RPAREN unit_id[unit] COLON ID[type] POINTER LBRACE statement_list[body] RBRACE {
		auto d = new FunctionDeclaration(@1.begin.line, $name, $type, $unit, $body);
		d->params = $param;
		d->pointer = true;
		$$ = d;
	}
	| FUNC ID[name] LPAREN paramdecl_list[param] RPAREN unit_id[unit] COLON CONST ID[type] POINTER LBRACE statement_list[body] RBRACE {
		auto d = new FunctionDeclaration(@1.begin.line, $name, $type, $unit, $body);
		d->params = $param;
		d->pointer = true;
		$$ = d;
	}
;

expression:
	LPAREN expression[exp] RPAREN {
		$$ = $exp;
	}
	| expression[l] PLUS expression[r] {
		$$ = new Addition(@2.begin.line, $l, $r);
	}
	| expression[l] MINUS expression[r] {
		$$ = new Subtraction(@2.begin.line, $l, $r);
	}
	| expression[l] DIVIDE expression[r] {
		$$ = new Division(@2.begin.line, $l, $r);
	}
	| expression[l] TIMES expression[r] {
		$$ = new Multiplication(@2.begin.line, $l, $r);
	}
	| expression[l] MOD expression[r] {
		$$ = new Modulo(@2.begin.line, $l, $r);
	}
	| expression[l] EXPONENT expression[r] {
		$$ = new Exponent(@2.begin.line, $l, $r);
	}
	| INTEGER[value] UNIT_ID[unit] {
		unsigned long long i = 0;
		if(::parser::parse_int($value, &i)){
			$$ = new IntegerLiteral(@1.begin.line, i, $unit);
		}
	}
	| INTEGER[value] {
		unsigned long long i = 0;
		if(::parser::parse_int($value, &i)){
			$$ = new IntegerLiteral(@1.begin.line, i, "#1");
		}
	}
	| FLOAT[value] UNIT_ID[unit] {
		double i = 0;
		if(::parser::parse_float($value, &i)){
			$$ = new IntegerLiteral(@1.begin.line, i, $unit);
		}
	}
	| FLOAT[value] {
		double i = 0;
		if(::parser::parse_float($value, &i)){
			$$ = new IntegerLiteral(@1.begin.line, i, "#1");
		}
	}
	| TRUE {
		$$ = new BoolLiteral(@1.begin.line, true);
	}
	| FALSE {
		$$ = new BoolLiteral(@1.begin.line, false);
	}
	| expression[l] EQUAL expression[r] {
		$$ = new Equal(@2.begin.line, $l, $r);
	}
	| expression[l] NEQUAL expression[r] {
		$$ = new NotEqual(@2.begin.line, $l, $r);
	}
	| expression[cond] QUESTION expression[t] COLON expression[f]{

	}
	| variable_access {
		$$ = $1;
	}
	| POINTER variable_access[var] {
		$$ = new Pointer(@1.begin.line, $var);
	}
	| variable_access[var] PROPERTY ID[name] {
		$$ = new Property(@2.begin.line, $var, $name);
	}
;

variable_access:
	ID[name] array_access[arr] {
		$$ = new Variable(@1.begin.line, $name, $arr);
	}
	| ID[name] LPAREN expression_list[param] RPAREN {
		$$ = new FunctionCall(@2.begin.line, $name, $param);
	}
	| variable_access[l] MEMBER variable_access[r] {
		$$ = new Member(@2.begin.line, $l, $r);
	}
;

array_access:
	LBRACKET expression[index] RBRACKET array_access[next] {
		$$ = new Array(@1.begin.line, $index, $next);
	}
	| /* empty */ {
		$$ = nullptr;
	}
;

expression_list:
	expression_cont {
		$$ = $1;
	}
	| /* empty */ {
		$$ = new List(nullptr);
	}
;

expression_cont:
	expression {
		$$ = new List($1);
	}
	| expression_cont COMMA expression {
		$$ = $1;
		auto l = new List($3);
		auto i = $$;
		while (i->next != nullptr){
			i = i->next;
		}
		i->next = l;
	}
;

%%

namespace yy
{
	// Report an error to the user.
	void parser::error(const location_type& loc, const std::string& msg)
	{
		std::cerr << "Error on line " << loc.begin.line << ":" << std::endl;
		std::cerr << "  " << msg << std::endl;
	}
}

int parse(const char* file, AST::List** ast){
	yyset_in(fopen(file, "r"));
	yy::parser parser;
	int ret = parser();

	*ast = program_ast;
	return ret;
}
