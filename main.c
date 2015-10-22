/*
 * main.c file
 */

#include "expression.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>
 
int yyparse(SExpression **expression, yyscan_t scanner);

SExpression *getAST(const char *source_code){
  /* get Abstract Syntax Tree , i.e. 
     source_code (text) => parse_tree (SExpression is root node) */
  SExpression *expression;
  yyscan_t scanner;
  YY_BUFFER_STATE state;
  int parse_status;
  int lex_status;

  lex_status = yylex_init(&scanner);              // initialize lexer
  if (lex_status) return NULL;                    // ... failed.

  state = yy_scan_string(source_code, scanner);   // do lexical analysis

  parse_status = yyparse(&expression, scanner);   // create parse tree
  if (parse_status) return NULL;                  // error while parsing

  yy_delete_buffer(state, scanner);               // clean up 
  yylex_destroy(scanner);

  return expression;
}

/* -- "execute" the parse tree -- 
 *
 *  (This is where the "now run it" part would
 *   happen for a larger programming language.)
 */
 
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

/* -- start graphviz parse tree code -- 
 *
 * write_graphviz(expression) creates the file tree.dot
 * which can be used to generate an image of the parse tree
 * using the graphiz.org "dot" command, i.e.

       dot -Tpng < tree.dot > tree.png 
 */

char* node_name(SExpression *e){
  /* return char* description of parse tree node, e.g. '+', '-', '3', etc */
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

/* We need to declare this before write_node so
   that the recursive mutual calls compile properly. */
void write_tree(SExpression *e, FILE *dotfile, int node);

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

/* -- main -- */

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
