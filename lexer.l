 /*   Group: Alexander Altman
  *          Schuyler Davis
  *          Timothy Gibson
  *   Class: CS4300
  * Project: Compiler (Final Project)
  */
%option noyywrap nodefault nounput noinput fast ecs batch align read 8bit never-interactive noreject noyymore bison-locations

%{
#include <iostream>
#include <cstdio>
#include "parser.h"
#include "error.h"
using namespace std;
%}
%%

%%
