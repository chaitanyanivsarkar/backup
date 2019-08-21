#include <stdio.h>
#include <stdlib.h>

#include "mpc.h"
#include "parser.h"

struct lisp_parser_t {
    mpc_parser_t *Number;
    mpc_parser_t *Operator;
    mpc_parser_t *Expression;
    mpc_parser_t *Lisp;
}

lisp_parser_t * lisp_parser_init() {
    lisp_parser_t *parser = (lisp_parser_t *)malloc(sizeof(lisp_parser_t));
    parser->Number = mpc_new("number");
    parser->Operator = mpc_new("operator");
    parser->Expression = mpc_new("expr");
    parser->Lisp = mpc_new("lisp");


    mpca_lang(MPCA_LANG_DEFAULT,
           "                                                     \
             number   : /-?[0-9]+/ ;                             \
             operator : '+' | '-' | '*' | '/' ;                  \
             expr     : <number> | '(' <operator> <expr>+ ')' ;  \
             lisp    : /^/ <operator> <expr>+ /$/ ;              \
           "       
             , parser->Number, parser->Operator, parser->Expression,
             parser->Lisp);
    
    return parser; 
}
