/*

    Andrew Crowell
    0545826
    acrowell@uoguelph.ca

    --

    The 'parser' module.
    
    Recognizes a language described by a LISP-like grammar
    
*/

#include <stdlib.h>
#include <setjmp.h>
#include "parser.h"

static jmp_buf env;

Parser* ParserNew(Scanner* scanner)
{
    Parser* self;
    
    self = malloc(sizeof(Parser));
    if(self)
    {
        self->scanner = scanner;
        self->tokenType = TOKEN_EMPTY;
    }
    return self;
}


void ParserFree(Parser* self)
{
    free(self);
}

void ParserNextToken(Parser* self)
{
    self->tokenType = ScannerNext(self->scanner);
}

static void UnexpectedTokenError(Parser* self, TokenType found, const char* production)
{
    fprintf(stderr, "error: unexpected %s (while parsing %s on line %d).\n", TokenTypeGetDescription(found), production, ScannerGetLineNumber(self->scanner));
    longjmp(env, 1);
}

static void ConsumeError(Parser* self, TokenType found, TokenType expected, const char* production)
{
    fprintf(stderr, "error: expected %s but found %s instead (while parsing %s on line %d).\n", TokenTypeGetDescription(expected), TokenTypeGetDescription(found), production, ScannerGetLineNumber(self->scanner));
    longjmp(env, 1);
}

void ParserConsume(Parser* self, TokenType expected, const char* production)
{
    if(self->tokenType == expected)
    {
        ParserNextToken(self);
    }
    else
    {
        ConsumeError(self, self->tokenType, expected, production);
    }
}

/* Production-handling prototypes used by code below. */
#define PROD_STR_STMTS "statements"
#define PROD_STR_STMT "statement"
#define PROD_STR_LIST_EXPR "list expression"
#define PROD_STR_LIST_CONCAT "concat expression"
#define PROD_STR_LIST_TERM "list term"
#define PROD_STR_LIST_CDR "cdr expression"
#define PROD_STR_ITEMS "items"
#define PROD_STR_ITEM "item"
#define PROD_STR_VAR "variable"

static void ParserHandleStatements(Parser*);
static void ParserHandleStatement(Parser*);
static void ParserHandleListExpression(Parser*);
static void ParserHandleListConcat(Parser*);
static void ParserHandleListTerm(Parser*);
static void ParserHandleListCDR(Parser*);
static void ParserHandleItems(Parser*);
static void ParserHandleItem(Parser*);
static void ParserHandleVariable(Parser*);

static void ParserHandleStatements(Parser* self)
{
    /*
        Tail-recursion optimization.
        Use a loop instead of function call when rule has itself on its right side.
    */
    while(1)
    {
        switch(self->tokenType)
        {
            /* STMTS_1 */
            case TOKEN_CHAR:
            case TOKEN_CDR:
            case TOKEN_LPAREN:
            case TOKEN_SET:
                ParserHandleStatement(self); /* STMT */
                ParserConsume(self, TOKEN_SEMI, PROD_STR_STMTS); /* ; */
                break; /* STMTS */
            /* STMTS_2 */
            case TOKEN_EOF:
                return; /* $ */
            default:
                UnexpectedTokenError(self, self->tokenType, PROD_STR_STMTS);
                return;
        }
    }
}

static void ParserHandleStatement(Parser* self)
{
    switch(self->tokenType)
    {
        /* STMT_1 */
        case TOKEN_CHAR:
        case TOKEN_CDR:
        case TOKEN_LPAREN:
            ParserHandleListExpression(self); /* LIST_EXPR */
            return;
        /* STMT_2 */
        case TOKEN_SET:
            ParserNextToken(self); /* set */
            ParserHandleVariable(self); /* VAR */
            ParserHandleListExpression(self); /* LIST_EXPR */
            return;
        default:
            UnexpectedTokenError(self, self->tokenType, PROD_STR_STMT);
            return;
    }
}

static void ParserHandleListExpression(Parser* self)
{
    switch(self->tokenType)
    {
        /* L_EXPR_1 */
        case TOKEN_CHAR:
        case TOKEN_CDR:
        case TOKEN_LPAREN:
            ParserHandleListTerm(self); /* LIST_TERM */
            ParserHandleListConcat(self); /* LIST_CONCAT */
            return;
        default:
            UnexpectedTokenError(self, self->tokenType, PROD_STR_LIST_EXPR);
            return;
    }
}

static void ParserHandleListConcat(Parser* self)
{
    /*
        Tail-recursion optimization.
        Use a loop instead of function call when rule has itself on its right side.
    */
    while(1)
    {
        switch(self->tokenType)
        {
            /* L_CONCAT_1 */
            case TOKEN_PLUS:
                ParserNextToken(self); /* + */
                ParserHandleListExpression(self); /* LIST_EXPR */
                break; /* LIST_CONCAT */
            /* L_CONCAT_2 */
            case TOKEN_CHAR:
            case TOKEN_INT:
            case TOKEN_OCT:
            case TOKEN_HEX:
            case TOKEN_REAL:
            case TOKEN_STR:
            case TOKEN_CAR:
            case TOKEN_CDR:
            case TOKEN_LPAREN:
            case TOKEN_RPAREN:
            case TOKEN_SEMI:
                return; /* epsilon */
            default:
                UnexpectedTokenError(self, self->tokenType, PROD_STR_LIST_TERM);
                return; 
        }
    }
}

static void ParserHandleListTerm(Parser* self)
{
    switch(self->tokenType)
    {
        /* L_TERM_1 */
        case TOKEN_CHAR:
            ParserHandleVariable(self); /* VAR */
            return;
        /* L_TERM_2 */
        case TOKEN_LPAREN:
            ParserNextToken(self); /* ( */
            ParserHandleItems(self); /* ITEMS */
            ParserConsume(self, TOKEN_RPAREN, PROD_STR_LIST_TERM); /* ) */
            return;
        /* L_TERM_3 */
        case TOKEN_CDR:
            ParserHandleListCDR(self); /* LIST_CDR */
            return;
        default:
            UnexpectedTokenError(self, self->tokenType, PROD_STR_LIST_TERM);
            return;
    }
}

static void ParserHandleListCDR(Parser* self)
{
    switch(self->tokenType)
    {
        /* L_CDR_1 */
        case TOKEN_CDR:
            ParserNextToken(self); /* cdr */
            ParserHandleListExpression(self); /* LIST_EXPR */
            return;
        default:
            UnexpectedTokenError(self, self->tokenType, PROD_STR_LIST_CDR);
            return;
    }
}

static void ParserHandleItems(Parser* self)
{
    /*
        Tail-recursion optimization.
        Use a loop instead of function call when rule has itself on its right side.
    */
    while(1)
    {
        switch(self->tokenType)
        {
            /* ITEMS_1 */
            case TOKEN_CHAR:
            case TOKEN_INT:
            case TOKEN_OCT:
            case TOKEN_HEX:
            case TOKEN_REAL:
            case TOKEN_STR:
            case TOKEN_CAR:    
            case TOKEN_CDR:    
            case TOKEN_LPAREN:
                ParserHandleItem(self); /* ITEM */
                break; /* ITEMS */
            /* ITEMS_2 */
            case TOKEN_RPAREN:
                return; /* epsilon */            
            default:
                UnexpectedTokenError(self, self->tokenType, PROD_STR_ITEMS);
                return;
        }
    }
}

static void ParserHandleItem(Parser* self)
{
    switch(self->tokenType)
    {
        /* ITEM_1 */
        case TOKEN_CHAR:
        case TOKEN_CDR:    
        case TOKEN_LPAREN:
            ParserHandleListExpression(self); /* LIST_EXPR */
            return;
        /* ITEM_2 */
        case TOKEN_STR:
            ParserNextToken(self); /* str */
            return;
        /* ITEM_3 */
        case TOKEN_INT:
        case TOKEN_OCT:
        case TOKEN_HEX:
            ParserNextToken(self); /* int */
            return;
        /* ITEM_4 */
        case TOKEN_REAL:
            ParserNextToken(self); /* real */
            return;
        /* ITEM_5 */
        case TOKEN_CAR:
            ParserNextToken(self); /* car */
            ParserHandleListExpression(self); /* LIST_EXPR */
            return;
        default:
            UnexpectedTokenError(self, self->tokenType, PROD_STR_ITEM);
            return;
    }
}

static void ParserHandleVariable(Parser* self)
{
    switch(self->tokenType)
    {
        /* VAR_1 */
        case TOKEN_CHAR:
            ParserNextToken(self); /* char */
            return;
        default:
            UnexpectedTokenError(self, self->tokenType, PROD_STR_VAR);
            return;
    }
}

void ParserRun(Parser* self)
{
    if(setjmp(env))
    {
        fprintf(stderr, "terminating compilation.\n\n");
        exit(-1);
    }
    ParserNextToken(self);
    while(self->tokenType != TOKEN_EOF)
    {
        ParserHandleStatements(self);
    }
}


int main(int argc, char** argv)
{
    Scanner* scanner;
    Parser* parser;

    scanner = ScannerNew(stdin);
    parser = ParserNew(scanner);
    ParserRun(parser);
    ParserFree(parser);
    ScannerFree(scanner);
    
    return 0;
}
