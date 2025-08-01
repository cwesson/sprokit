%{
/* Declarations */
#include "sprokit.tab.hh"
#include "ASTNode.h"
#include <string.h>

using token = yy::parser::token;
extern yy::parser::value_type yylval;
#define YY_DECL int yylex(yy::parser::value_type* yylval_param, yy::parser::location_type* yylloc_param)
#define YYSTYPE yy::parser::value_type
#define YYLTYPE yy::parser::location_type
#define YY_USER_ACTION yylloc->begin.line = yylineno;
#define YY_BREAL yylloc->end.line = yylineno; break;
%}
/* Definitions */

%option noyywrap
%option nounput
%option yylineno
%option bison-locations

dec  [0-9_]
oct  [0-7_]
hex  [0-9a-fA-F_]
word [_a-zA-Z]

unit {word}+(\^{dec}+)?

/* Rules */
%%

"const"    { return token::CONST; }
"var"      { return token::VAR; }
"func"     { return token::FUNC; }
"type"     { return token::TYPE; }
"unit"     { return token::UNIT; }
"return"   { return token::RETURN; }
"operator" { return token::OPERATOR; }
"if"       { return token::IF; }
"else"     { return token::ELSE; }
"for"      { return token::FOR; }
"break"    { return token::BREAK; }
"continue" { return token::CONTINUE; }
"yield"    { return token::YIELD; }
"switch"   { return token::SWITCH; }
"case"     { return token::CASE; }
"default"  { return token::DEFAULT; }
"true"     { return token::TRUE; }
"false"    { return token::FALSE; }
"with"     { return token::WITH; }

"{"  { return token::LBRACE; }
"}"  { return token::RBRACE; }
";"  { return token::SEMICOLON; }
","  { return token::COMMA; }
":"  { return token::COLON; }
"."  { return token::MEMBER; }
"?"  { return token::QUESTION; }
"`"  { return token::PROPERTY; }
"==" { return token::EQUAL; }
"!=" { return token::NEQUAL; }
">=" { return token::GTEQUAL; }
"<=" { return token::LTEQUAL; }
">"  { return token::GREATER; }
"<"  { return token::LESSER; }
"="  { return token::ASSIGN; }
"+"  { return token::PLUS; }
"-"  { return token::MINUS; }
"*"  { return token::TIMES; }
"/"  { return token::DIVIDE; }
"%"  { return token::MOD; }
"^"  { return token::EXPONENT; }
"&"  { return token::BITAND; }
"|"  { return token::BITOR; }
"^|" { return token::BITXOR; }
"~"  { return token::BITNOT; }
"<<" { return token::LSHIFT; }
">>" { return token::RSHIFT; }
"&&" { return token::BOOLAND; }
"||" { return token::BOOLOR; }
"!"  { return token::BOOLNOT; }
"@"  { return token::POINTER; }
"("  { return token::LPAREN; }
")"  { return token::RPAREN; }
"["  { return token::LBRACKET; }
"]"  { return token::RBRACKET; }

({dec}+\.{dec}*)|({dec}*\.{dec}+)(e[\+\-]?{dec}+)? {
	yylval->literal = strdup(yytext); return token::FLOAT;
}

([0-9]{dec}*)|0(b[0-1]+|o{oct}+|x{hex}+)? {
	yylval->literal = strdup(yytext); return token::INTEGER;
}

\#([ \t\r\n]+)?({unit}(\*{unit})*|1)(\/{unit}(\*{unit})*)? {
	yylval->symbol = strdup(yytext); return token::UNIT_ID;
}

{word}[a-zA-Z_0-9]* {
	yylval->symbol = strdup(yytext); return token::ID;
}

\"([^\"]|(\\\"))*\" {
	yylval->literal = strdup(yytext); return token::STRING;
}

[ \t\n\r\f\v]+ ;

\/\/.* ;
\/\*([^\*]|(\*+[^\*/]))*\*+\/ ;

. { return token::SCANNERERROR; }

%%
// User subroutines

