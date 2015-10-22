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

#define MAX_NODE_NAME_CHARS 8

void write_node(SExpression *e, FILE *dotfile, int node_number){
  char node_name[MAX_NODE_NAME_CHARS + 1];
  int this_number = node_number;
  switch (e->type){
  case eVALUE:
    sprintf(node_name, "%i", e->value);
    break;
  case eMULTIPLY:
    sprintf(node_name, "*");
    break;
  case ePLUS:
    sprintf(node_name, "+");
    break;
  }
  fprintf(dotfile, "%i [label=\"%s\"];\n", this_number, node_name);
  if (e->left != NULL){
    node_number++;
    fprintf(dotfile, " %i -> %i \n", this_number, node_number);
    write_node(e->left, dotfile, node_number);
  }
  if (e->right != NULL){
    node_number++;
    fprintf(dotfile, " %i -> %i \n", this_number, node_number);
    write_node(e->right, dotfile, node_number);
  }
}

void write_graphviz(SExpression *e){
  /* Output a graphviz.org representation of parse tree to file tree.dot.
     To generate an image of the tree, install the graphviz "dot" software
     and run at the command prompt " dot -Tpng < tree.dot > tree.png ".
   */
  FILE *dotfile = fopen("tree.dot", "w");
  fprintf(dotfile, "digraph tree {\n");
  write_node(e, dotfile, 1);
  fprintf(dotfile, "}\n");
  fclose(dotfile);
}
 
int main(void){
  SExpression *e = NULL;
  char test[] = " 4 + 2*10 + 3*( 5 + 1 ) ";
  int result = 0;
  e = getAST(test);
  write_graphviz(e);
  result = evaluate(e);
  printf("Result of '%s' is %d\n", test, result);
  deleteExpression(e);
  return 0;
}
