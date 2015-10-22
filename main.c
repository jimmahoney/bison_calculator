/*
 * main.c file
 */

#include "expression.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>
 
int yyparse(SExpression **expression, yyscan_t scanner);
 
SExpression *getAST(const char *expr){
  SExpression *expression;
  yyscan_t scanner;
  YY_BUFFER_STATE state;
  if (yylex_init(&scanner)) return NULL;            // couldn't initialize
  state = yy_scan_string(expr, scanner);
  if (yyparse(&expression, scanner)) return NULL;   // error parsing
  yy_delete_buffer(state, scanner);
  yylex_destroy(scanner);
  return expression;
}
 
int evaluate(SExpression *e){
  switch (e->type) {
  case eVALUE:
    return e->value;
  case eMULTIPLY:
    return evaluate(e->left) * evaluate(e->right);
  case ePLUS:
    return evaluate(e->left) + evaluate(e->right);
  default:          // shouldn't get here
    return 0;
  }
}
 
int main(void){
  SExpression *e = NULL;
  char test[] = " 4 + 2*10 + 3*( 5 + 1 ) ";
  int result = 0;
  e = getAST(test);
  result = evaluate(e);
  printf("Result of '%s' is %d\n", test, result);
  deleteExpression(e);
  return 0;
}
