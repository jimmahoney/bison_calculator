%{
 
/*
 * Parser.y file
 * To generate the parser run: "bison Parser.y"
 */
#include "expression.h"
#include "parser.h"
#include "lexer.h"
 
%}

%code requires {
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif
}

%output  "parser.c"
%defines "parser.h"
 
%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { SExpression **expression }
%parse-param { yyscan_t scanner }

%union {
  int value;
  SExpression *expression;
}
 
%left '+' TOKEN_PLUS
%left '*' TOKEN_MULTIPLY
 
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_PLUS
%token TOKEN_MULTIPLY
%token <value> TOKEN_NUMBER

%type <expression> expr
 
%%
 
input
  : expr                              { *expression = $1; }
  ;
 
expr
  : expr[L] TOKEN_PLUS expr[R]        { $$ = createOperation(ePLUS, $L,$R);}
  | expr[L] TOKEN_MULTIPLY expr[R]    { $$ = createOperation(eMULTIPLY, $L,$R);}
  | TOKEN_LPAREN expr[E] TOKEN_RPAREN { $$ = $E;}
  | TOKEN_NUMBER                      { $$ = createNumber($1);}
  ;

%%
