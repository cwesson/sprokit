%language "c++"

%{
#include "parser_util.h"
#include "AST.h"
#include "PrintAST.h"
#include "CollectSymbols.h"
#include "DimensionalAnalysis.h"
using namespace AST;
#include "sprokit.tab.hh"

extern unsigned int yylineno;
int yylex(yy::parser::value_type* yylval, yy::parser::location_type* yylloc);

AST::List* program_ast = nullptr;
%}

%locations

%token <literal> INTEGER FLOAT
%token <symbol> UNIT_ID
%token <symbol> ID

%token CONST VAR FUNC TYPE UNIT
%token RETURN
%token OPERATOR
%token IF ELSE FOR
%token TRUE FALSE

%token LBRACE RBRACE
%token SEMICOLON
%token COMMA
%token COLON
%token ASSIGN

%token LPAREN RPAREN
%token LBRACKET RBRACKET
%left EQUAL NEQUAL GREATER LESSER
%left OR
%left AND
%right NOT
%left PLUS MINUS
%left TIMES DIVIDE MOD
%left EXPONENT

%token SCANNERERROR

%type <list> program
%type <list> statement_list
%type <node> statement
%type <node> if_statement
%type <list> paramdecl_list
%type <list> paramdecl_cont
%type <node> paramdecl
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

%union{
	const char* literal;
	const char* symbol;
	class ASTNode* node;
	class Expression* exp;
	class List* list;
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
	declaration
	| ID ASSIGN expression SEMICOLON {
		$$ = new Assignment(@2.begin.line, $1, $3);
	}
	| RETURN SEMICOLON {
		$$ = new Return(@1.begin.line, nullptr);
	}
	| RETURN expression SEMICOLON {
		$$ = new Return(@1.begin.line, $2);
	}
	| if_statement {
		$$ = $1;
	}
;

if_statement:
	IF expression[cond] LBRACE statement_list[body] RBRACE {
		$$ = new IfStatement(@1.begin.line, $cond, $body, nullptr);
	}
	| IF expression[cond] LBRACE statement_list[body] RBRACE ELSE LBRACE statement_list[alt] RBRACE {
		$$ = new IfStatement(@1.begin.line, $cond, $body, $alt);
	}
	| IF expression[cond] LBRACE statement_list[body] RBRACE ELSE if_statement[alt] {
		$$ = new IfStatement(@1.begin.line, $cond, $body, $alt);
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
	paramdecl {
		$$ = new List($1);
	}
	| paramdecl_cont COMMA paramdecl {
		$$ = $1;
		auto l = new List($3);
		$1->next = l;
	}
;

paramdecl:
	CONST ID UNIT_ID COLON ID {
		auto d = new ParameterDeclaration(@1.begin.line, $2, $5, $3);
		d->constant = true;
		$$ = d;
	}
	| VAR ID UNIT_ID COLON ID {
		auto d = new ParameterDeclaration(@1.begin.line, $2, $5, $3);
		d->constant = false;
		$$ = d;
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
	CONST ID UNIT_ID COLON ID SEMICOLON {
		auto d = new VariableDeclaration(@1.begin.line, $2, $5, $3, nullptr);
		$$ = d;
		d->constant = true;
	}
	| VAR ID UNIT_ID COLON ID SEMICOLON {
		auto d = new VariableDeclaration(@1.begin.line, $2, $5, $3, nullptr);
		$$ = d;
		d->constant = false;
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
	OPERATOR UNIT_ID LPAREN CONST ID RPAREN ASSIGN expression {
		$$ = new Conversion(yylineno, $5, $2, $8);
	}
;

declaration:
	CONST ID[name] UNIT_ID[unit] COLON ID[type] ASSIGN expression[init] SEMICOLON {
		auto d = new VariableDeclaration(@1.begin.line, $name, $type, $unit, $init);
		d->constant = true;
		$$ = d;
	}
	| VAR ID[name] UNIT_ID[unit] COLON ID[type] ASSIGN expression[init] SEMICOLON {
		auto d = new VariableDeclaration(@1.begin.line, $name, $type, $unit, $init);
		d->constant = false;
		$$ = d;
	}
	| funcdecl {
		$$ = $1;
	}
	| TYPE ID[type] LBRACE member_list[list] RBRACE {
		$$ = new TypeDeclaration(@1.begin.line, $type, $list);
	}
	| UNIT UNIT_ID[unit] LBRACE unitmember_list[list] RBRACE {
		$$ = new UnitDeclaration(@1.begin.line, $unit, $list);
	}
	| UNIT UNIT_ID[unit] COLON UNIT_ID[alias] LBRACE unitmember_list[list] RBRACE {
		$$ = new UnitDeclaration(@1.begin.line, $unit, $list);
	}
;

funcdecl:
	FUNC ID[name] LPAREN paramdecl_list[param] RPAREN UNIT_ID[unit] COLON ID[type] LBRACE statement_list[body] RBRACE {
		auto d = new FunctionDeclaration(@1.begin.line, $name, $type, $unit, $body);
		d->params = $param;
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
	| ID[name] LPAREN expression_list[param] RPAREN {
		$$ = new FunctionCall(@2.begin.line, $name, $param);
	}
	| ID[name] {
		$$ = new Variable(@1.begin.line, $name);
	}
	| expression[l] EQUAL expression[r] {
		$$ = new Equal(@2.begin.line, $l, $r);
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
		$1->next = l;
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

int main() {
	yy::parser parse;
	int ret = parse();

	if(program_ast != nullptr){
		PrintAST p;
		p.visit(*program_ast);
		std::cout << std::endl;

		CollectSymbols c;
		c.visit(*program_ast);
		ret += c.error_count;
		std::cout << *c.table;

		DimensionalAnalysis d;
		d.visit(*program_ast);
		ret += d.error_count;

		delete program_ast;
	}

	return ret;
}
