/*

    Andrew Crowell
    0545826
    acrowell@uoguelph.ca

    --

    The 'parser' module.
    
    Recognizes a language described by a LISP-like grammar
    
*/

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"

typedef struct
{
    Scanner* scanner;
    TokenType tokenType;
} Parser;

Parser* ParserNew(Scanner* scanner);
void ParserFree(Parser* self);
void ParserNextToken(Parser* self);
void ParserConsume(Parser* self, TokenType expected, const char* prod);
void ParserRun(Parser* self);

#endif


