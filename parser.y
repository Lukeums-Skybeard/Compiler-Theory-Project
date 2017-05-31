%{
%skeleton "lalr1.cc"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "error.h"
%}

%locations
%defines "parser.h"
%union{
    /* variables */
    string <d> STRING;
}

 /* tokens here */
%token EOL
%token ID
%token INT_LITERAL
%token INT
%token FLOAT
%token RETURN
%token IF
%token ELSE
%token WHILE
%token CIN
%token STREAMIN
%token COUT
%token STREAMOUT
%token STR_LITERAL
%token ENDL
%token ASSIGNOP
%token INCOP
%token MULOP
%token FLT_LITERAL
%token OR
%token AND
%token NOT
%token RELOP
%token WHILE


%%
/* TODO: Add more grammar rules */
cout: /* no output */
| out EOL {
  printf("> "); /* whitespace and comments */
}
%%
