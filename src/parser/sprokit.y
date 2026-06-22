%language "c++"

%{
#include "parser_util.h"
#include "AST.h"
#include "Type.h"
#include <cstdlib>
using namespace AST;
using namespace ADT;
#include "sprokit.tab.hh"

std::string yyfilename;
extern unsigned int yylineno;
int yylex(yy::parser::value_type* yylval, yy::parser::location_type* yylloc);
void yyset_in(FILE*);

AST::List* program_ast = nullptr;
%}

%locations

%token <literal> INTEGER FLOAT STRING
%token <symbol> UNIT_ID
%token <symbol> ID

%token CONST VAR FUNC STRUCT ENUM UNIT
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
%left QUESTION
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
%left COLON UNIT_ID
%left UMINUS UPLUS
%right POINTER
%left LBRACKET RBRACKET
%left MEMBER SCOPE

%token SCANNERERROR

%type <list> program
%type <list> statement_list
%type <node> statement
%type <node> assignment
%type <node> block_statement
%type <node> if_statement
%type <list> if_else_statement
%type <node> for_statement
%type <node> case_statement
%type <list> case_list
%type <list> paramdecl_list
%type <list> paramdecl_cont
%type <symbol> unit_id
%type <var_decl> variable_init
%type <var_decl> variable_decl
%type <var> variable_access
%type <arr> array_access
%type <arr> array_decl
%type <list> member_list
%type <node> member
%type <list> enum_list
%type <node> enum_val
%type <list> unitmember_list
%type <node> unitmember
%type <list> declaration_list
%type <node> declaration
%type <list> attr_list
%type <node> attribute
%type <node> attr_param
%type <list> attr_param_list
%type <node> funcdecl
%type <exp> expression
%type <list> expression_list
%type <list> expression_cont
%type <type> base_type
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
	| variable_init SEMICOLON declaration_list {
		$$ = new List($1);
		$$->next = $3;
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
	| variable_init SEMICOLON {
		$$ = $1;
	}
	| assignment SEMICOLON {
		$$ = $1;
	}
	| block_statement {
		$$ = $1;
	}
	| RETURN expression SEMICOLON {
		$$ = new Return(@1.begin, $2);
	}
	| BREAK SEMICOLON {
		/// @todo break/continue statements
	}
	| BREAK ID[id] SEMICOLON {
		/// @todo break/continue statements
	}
	| CONTINUE SEMICOLON {
		/// @todo break/continue statements
	}
	| CONTINUE ID[id] SEMICOLON {
		/// @todo break/continue statements
	}
	| YIELD expression[exp] SEMICOLON {
		/// @todo Statement initializers
	}
;

block_statement:
	if_statement {
		$$ = $1;
	}
	| for_statement {
		$$ = $1;
	}
	| SWITCH expression[exp] LBRACE case_list[list] RBRACE {
		/// @todo switch statements
	}
	| WITH variable_init[init] LBRACE statement_list[body] RBRACE {
		$$ = new WithStatement(@1.begin, $init, $body);
	}
;

assignment:
	variable_access[var] ASSIGN expression[value] {
		$$ = new Assignment(@2.begin, $var, $value);
	}
	| variable_access[var] ASSIGN block_statement[value] {
		/// @todo Statement initializers
	}
;

if_statement:
	IF expression[cond] LBRACE statement_list[body] RBRACE if_else_statement[alt] {
		$$ = new IfStatement(@1.begin, nullptr, $cond, $body, $alt);
	}
	| IF variable_init[init] SEMICOLON  expression[cond] LBRACE statement_list[body] RBRACE if_else_statement[alt] {
		$$ = new IfStatement(@1.begin, $init, $cond, $body, $alt);
	}
;

if_else_statement:
	ELSE LBRACE statement_list[alt] RBRACE {
		$$ = $alt;
	}
	| ELSE if_statement[alt] {
		$$ = new List($alt);
	}
	| /* empty */ {
		$$ = nullptr;
	}
;


for_statement:
	FOR variable_init[init] SEMICOLON expression[cond] SEMICOLON assignment[inc] LBRACE statement_list[body] RBRACE {
		$$ = new ForStatement(@1.begin, $init, $cond, dynamic_cast<Assignment*>($inc), $body);
	}
	| FOR expression[cond] LBRACE statement_list[body] RBRACE {
		$$ = new ForStatement(@1.begin, nullptr, $cond, nullptr, $body);
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
		/// @todo switch statements
	}
	| DEFAULT LBRACE statement_list[list] RBRACE {
		/// @todo switch statements
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

base_type:
	ID[type] {
		$$ = &ADT::Type::findType($type);
	}
;

type_spec:
	type_spec[ext] POINTER {
		$$ = &ADT::Type::findPointerType((std::string)*$ext);
	}
	| base_type {
		$$ = $1;
	}
;

variable_decl:
	CONST attr_list ID[name] unit_id[unit] COLON type_spec[type] array_decl[arr] {
		auto d = new VariableDeclaration(@1.begin, $name, *$type, $unit, $arr);
		d->constant = true;
		$$ = d;
	}
	| VAR attr_list ID[name] unit_id[unit] COLON type_spec[type] array_decl[arr] {
		auto d = new VariableDeclaration(@1.begin, $name, *$type, $unit, $arr);
		d->constant = false;
		$$ = d;
	}
;

array_decl:
	LBRACKET expression[index] RBRACKET array_decl[next] {
		$$ = new Array(@1.begin, $index, $next);
	}
	|
	LBRACKET CONST ID[name] COLON base_type[type] RBRACKET array_decl[next] {
		$$ = new Array(@1.begin, new VariableDeclaration(@name.begin, $name, *$type, "#1", nullptr), $next);
	}
	|
	LBRACKET RBRACKET array_decl[next] {
		$$ = new Array(@1.begin, $next);
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

enum_list:
	enum_val enum_list {
		$$ = new List($1);
		$$->next = $2;
	}
	| /* empty */ {
		$$ = new List(nullptr);
	}
;

enum_val:
	ID SEMICOLON {
		/// @todo Enums
	}
	| ID ASSIGN INTEGER SEMICOLON {
		/// @todo Enums
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
		$$ = new Conversion(@2.begin, $name, $unit, $exp);
	}
;

variable_init:
	variable_decl[var] ASSIGN expression[value] {
		$$ = $var;
		$var->initial = $value;
	}
	| variable_decl[var] ASSIGN statement[value] {
		/// @todo Statement initializers
	}
	| variable_decl[var] ASSIGN LBRACE expression_list[value] RBRACE {
		/// @todo Statement initializers
	}
;

declaration:
	funcdecl {
		$$ = $1;
	}
	| STRUCT attr_list ID[type] LBRACE member_list[list] RBRACE {
		$$ = new TypeDeclaration(@1.begin, $type, $list);
	}
	| ENUM attr_list ID[type] LBRACE enum_list[list] RBRACE {
		$$ = new TypeDeclaration(@1.begin, $type, $list);
	}
	| UNIT attr_list UNIT_ID[unit] LBRACE unitmember_list[list] RBRACE {
		$$ = new UnitDeclaration(@1.begin, $unit, $list, "");
	}
	| UNIT attr_list UNIT_ID[unit] COLON UNIT_ID[alias] LBRACE unitmember_list[list] RBRACE {
		$$ = new UnitDeclaration(@1.begin, $unit, $list, $alias);
	}
;

attr_list:
	PROPERTY attribute attr_list {
		/// @todo Attributes
	}
	| /* empty */{
	}
;

attribute:
	ID[name] {
		/// @todo Attributes
	}
	| ID[name] LPAREN attr_param_list RPAREN {
		/// @todo Attributes
	}
	| ID[scope] SCOPE attribute[att] {
		/// @todo Attributes
	}
;

attr_param_list:
	attr_param_list COMMA attr_param {
		/// @todo Attributes
	}
	| attr_param {
		/// @todo Attributes
	}
	| /* empty */ {
		/// @todo Attributes
	}
;

attr_param:
	ID[value] {
		/// @todo Attribute parameters
	}
	| INTEGER[value] {
		/// @todo Attribute parameters
	}
	| INTEGER[value] UNIT_ID[unit] {
		/// @todo Attribute parameters
	}
;

funcdecl:
	FUNC attr_list ID[name] LPAREN paramdecl_list[param] RPAREN unit_id[unit] COLON ID[type] LBRACE statement_list[body] RBRACE {
		auto d = new FunctionDeclaration(@1.begin, $name, $type, $unit, $body);
		d->params = $param;
		$$ = d;
	}
	| FUNC attr_list ID[name] LPAREN paramdecl_list[param] RPAREN unit_id[unit] COLON ID[type] POINTER LBRACE statement_list[body] RBRACE {
		auto d = new FunctionDeclaration(@1.begin, $name, $type, $unit, $body);
		d->params = $param;
		d->pointer = true;
		$$ = d;
	}
	| FUNC attr_list ID[name] LPAREN paramdecl_list[param] RPAREN unit_id[unit] COLON CONST ID[type] POINTER LBRACE statement_list[body] RBRACE {
		auto d = new FunctionDeclaration(@1.begin, $name, $type, $unit, $body);
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
		$$ = new Addition(@2.begin, $l, $r);
	}
	| expression[l] MINUS expression[r] {
		$$ = new Subtraction(@2.begin, $l, $r);
	}
	| expression[l] DIVIDE expression[r] {
		$$ = new Division(@2.begin, $l, $r);
	}
	| expression[l] TIMES expression[r] {
		$$ = new Multiplication(@2.begin, $l, $r);
	}
	| expression[l] MOD expression[r] {
		$$ = new Modulo(@2.begin, $l, $r);
	}
	| expression[l] EXPONENT expression[r] {
		$$ = new Exponent(@2.begin, $l, $r);
	}
	| MINUS expression[r] %prec UMINUS {
		$$ = new Negation(@1.begin, $r);
	}
	| PLUS expression[r] %prec UPLUS {
		$$ = $r;
	}
	| expression[l] BOOLOR expression[r] {
		$$ = new BoolOr(@2.begin, $l, $r);
	}
	| expression[l] BOOLAND expression[r] {
		$$ = new BoolAnd(@2.begin, $l, $r);
	}
	| BOOLNOT expression[r] {
		$$ = new BoolNot(@2.begin, $r);
	}
	| expression[l] LSHIFT expression[r] {
		/// @todo $$ = new ShiftLeft(@2.begin, $l, $r);
	}
	| expression[l] RSHIFT expression[r] {
		/// @todo $$ = new ShiftRight(@2.begin, $l, $r);
	}
	| expression[l] BITOR expression[r] {
		/// @todo $$ = new BitOr(@2.begin, $l, $r);
	}
	| expression[l] BITXOR expression[r] {
		/// @todo $$ = new BitXor(@2.begin, $l, $r);
	}
	| expression[l] BITAND expression[r] {
		/// @todo $$ = new BitAnd(@2.begin, $l, $r);
	}
	| BITNOT expression[r] {
		/// @todo $$ = new BitNot(@2.begin, $r);
	}
	| INTEGER[value] UNIT_ID[unit] {
		unsigned long long i = 0;
		if(::parser::parse_int($value, &i)){
			$$ = new IntegerLiteral(@1.begin, i, $unit);
		}
	}
	| INTEGER[value] {
		unsigned long long i = 0;
		if(::parser::parse_int($value, &i)){
			$$ = new IntegerLiteral(@1.begin, i, "#1");
		}
	}
	| FLOAT[value] UNIT_ID[unit] {
		double i = 0;
		if(::parser::parse_float($value, &i)){
			$$ = new FloatLiteral(@1.begin, i, $unit);
		}
	}
	| FLOAT[value] {
		double i = 0;
		if(::parser::parse_float($value, &i)){
			$$ = new FloatLiteral(@1.begin, i, "#1");
		}
	}
	| TRUE {
		$$ = new BoolLiteral(@1.begin, true);
	}
	| FALSE {
		$$ = new BoolLiteral(@1.begin, false);
	}
	| expression[l] EQUAL expression[r] {
		$$ = new Equal(@2.begin, $l, $r);
	}
	| expression[l] NEQUAL expression[r] {
		$$ = new NotEqual(@2.begin, $l, $r);
	}
	| expression[l] GREATER expression[r] {
		$$ = new GreaterThan(@2.begin, $l, $r);
	}
	| expression[l] LESSER expression[r] {
		$$ = new LessThan(@2.begin, $l, $r);
	}
	| expression[l] GTEQUAL expression[r] {
		$$ = new GreaterEqual(@2.begin, $l, $r);
	}
	| expression[l] LTEQUAL expression[r] {
		$$ = new LessEqual(@2.begin, $l, $r);
	}
	| expression[cond] QUESTION expression[t] QUESTION QUESTION expression[f]{
		/// @todo ternary operator
	}
	| variable_access {
		$$ = $1;
	}
	| POINTER variable_access[var] {
		$$ = new Pointer(@1.begin, $var);
	}
	| variable_access[var] PROPERTY ID[name] {
		$$ = new Property(@2.begin, $var, $name);
	}
	| COLON type_spec[type] PROPERTY ID[name] {
		/// @todo Type properties
	}
	| expression[e] COLON type_spec[type] {
		/// @todo Type casting
	}
;

variable_access:
	ID[name] array_access[arr] {
		$$ = new Variable(@1.begin, $name, $arr);
	}
	| ID[name] LPAREN expression_list[param] RPAREN {
		$$ = new FunctionCall(@2.begin, $name, $param);
	}
	| variable_access[l] MEMBER variable_access[r] {
		$$ = new Member(@2.begin, $l, $r);
	}
;

array_access:
	LBRACKET expression[index] RBRACKET array_access[next] {
		$$ = new Array(@1.begin, $index, $next);
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
		std::cerr << "Error at " << loc.begin << ": " << msg << std::endl;
	}
}

int parse(const char* file, AST::List** ast){
	yyfilename = file;
	yyset_in(fopen(file, "r"));
	yy::parser parser;
	int ret = parser();

	*ast = program_ast;
	return ret;
}
