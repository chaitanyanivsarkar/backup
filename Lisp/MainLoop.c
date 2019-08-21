#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

#include "mpc.h"

long eval_operation(long x, char *operation, long y) {
    if(strcmp(operation, "+") == 0) {return x + y;}
    else if(strcmp(operation, "-") == 0) {return x - y;}
    else if(strcmp(operation, "*") == 0) {return x * y;}
    else if(strcmp(operation, "/") == 0) {return x / y;}
    return 0;
}

long evaluate(mpc_ast_t *tree) {
    // base case if number
    if(strstr(tree->tag, "number")) {
        return atoi(tree->contents);
    }
    
    // recursive case
    char *operation = tree->children[1]->contents;
    long x = evaluate(tree->children[2]);

    int i = 3;
    while(strstr(tree->children[i]->tag, "expr")) {
        x = eval_operation(x, operation, evaluate(tree->children[i]));
        ++i;
    }

    return x;
}

int main(int argc, char** argv) {
  
  /* Create Some Parsers */
  mpc_parser_t *Number   = mpc_new("number");
  mpc_parser_t *Operator = mpc_new("operator");
  mpc_parser_t *Expr     = mpc_new("expr");
  mpc_parser_t *Lispy    = mpc_new("lispy");
  
  /* Define them with the following Language */
  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                     \
      number   : /-?[0-9]+/ ;                             \
      operator : '+' | '-' | '*' | '/' ;                  \
      expr     : <number> | '(' <operator> <expr>+ ')' ;  \
      lispy    : /^/ <operator> <expr>+ /$/ ;             \
    ",
    Number, Operator, Expr, Lispy);
  
  puts("Lispy Version 0.0.0.0.2");
  puts("Press Ctrl+c to Exit\n");
  
  while (1) {
  
    char* input = readline("lispy> ");
    add_history(input);
    
    /* Attempt to parse the user input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      /* On success print and delete the AST */
      long result = evaluate(r.output);
      printf("%li\n", result);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise print and delete the Error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    
    free(input);
  }
  
  /* Undefine and delete our parsers */
  mpc_cleanup(4, Number, Operator, Expr, Lispy);
  
  return 0;
}

#undef forever
