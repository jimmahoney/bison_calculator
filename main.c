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

/* Output a graphviz.org representation of the parse tree to file tree.dot.
   (To generate an image of the tree after this runs, install the 
   graphviz "dot" software and at the command prompt run :
       dot -Tpng < tree.dot > tree.png 
   )
*/
void write_graphviz(SExpression *e);


/* recursive descent pieces of write_graphviz */
void write_tree(SExpression *e, FILE *dotfile, int node);
void write_node(SExpression *e, FILE *dotfile, int node, int parent);

/* return char* description of parse tree node, e.g. '+', '-', '3', etc */
char* node_name(SExpression *e){
  char* name = (char *)malloc(16);  /* up to 15 char descriptive node name */
  switch (e->type){
  case eVALUE:
    sprintf(name, "%i", e->value);
    return name;
  case eMULTIPLY:
    sprintf(name, "*");
    return name;
  case ePLUS:
    sprintf(name, "+");
    return name;
  default:  // shouldn't get here
    sprintf(name, "???");
    return name;
  }
}

void write_node(SExpression *e, FILE *dotfile, int node, int parent){
  fprintf(dotfile, " %i -> %i \n", parent, node);
  write_tree(e, dotfile, node);
}

void write_tree(SExpression *e, FILE *dotfile, int node){
  fprintf(dotfile, "%i [label=\"%s\"];\n", node, node_name(e));
  if (e->left != NULL) write_node(e->left, dotfile, 2*node, node);
  if (e->right != NULL) write_node(e->right, dotfile, 2*node+1, node);
}

void write_graphviz(SExpression *e){
  FILE *dotfile = fopen("tree.dot", "w");
  fprintf(dotfile, "digraph tree {\n");
  write_tree(e, dotfile, 1);
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
