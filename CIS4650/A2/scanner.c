/*

    Andrew Crowell
    0545826
    acrowell@uoguelph.ca

    --

    The 'scanner' module.
    
    Takes input from stdin, scans it, and dumps tokens into stdout as they are scanned.
    Stops when EOF is encountered, or forcibly killed somehow.
    
*/

#include <string.h>
#include <stdlib.h>
#include "scanner.h"

/*

    The descriptive name associated with each of these token types.
    
    Used mainly for error messages.
    
*/
static const char* const tokenDesc[] = {
    "?",
    "end-of-file marker",
    "internal error (scanner or file handle invalid)",
    
    "int value",
    "hexadecimal int value",
    "octal int value",
    "real number value",
    "left parenthesis (",
    "right parenthesis )",
    "car",
    "cdr",
    "set",
    "character",
    "string",
    "plus +",
    "semi-colon ;",
};

const char* TokenTypeGetDescription(TokenType tokenType)
{
    return tokenDesc[tokenType];
}

Scanner* ScannerNew(FILE* file)
{
    Scanner* self;
    
    self = malloc(sizeof(Scanner));
    if(self)
    {
        self->file = file;
        self->tokenType = TOKEN_EMPTY;
        self->state = SCAN_START;
        self->position = 0;
        self->len = 0;
        self->lineNumber = 1;
    }
    return self;
}

void ScannerFree(Scanner* self)
{
    free(self);
}

static void __ScannerReset(Scanner* self)
{
    /* Reset the scanning state. */
    self->tokenType = TOKEN_EMPTY;
    self->state = SCAN_START;    
}

TokenType ScannerNext(Scanner* self)
{
    char c;
    TokenType result;
    
    /* Undefined scanner. */
    if(!self || !self->file)
    {
        return TOKEN_INTERNAL_ERR;
    }
    
    do
    {
        /* Continue across buffer until we exhaust it. */
        for(; self->position < self->len; self->position++)
        {
            /* Read from the buffer. */
            c = self->buffer[self->position];
            
            /* Whitespace delimiter. */
            if(c == ' ' || c == '\t' || c == '\r' || c == '\n')
            {
                /*
                    If we've cleared the delimiter flag,
                    this means we've read at least one non-delimiter character,
                    and can emit a known token.
                */
                if(self->tokenType != TOKEN_EMPTY)
                {
                    result = self->tokenType;
                    
                    __ScannerReset(self);
             
                    if(c != '\n')
                    {
                        /* Move forward, unless newline, in which case we keep it in buffer for next token. */
                        self->position++;
                    }       
                    
                    return result;
                }
                /* Advance line. */
                if(c == '\n')
                {
                    self->lineNumber++;
                }
                
                __ScannerReset(self);
                
                /* Skip forward. */
                continue;
            }
            
            /*
                The big, frightening scanner state lookup.
            
                Implemented through two switch statements.
                
                First, we look at our current state.
                Then, based on the state, we decide how to react
                to each character.
            
                The "reactions" will modify the state of parsing
                for the next character (if any), and the current
                classification of what type the token is.
            
                The token type is used when a delimiter is
                finally reached.
                
                The whole thing is just a state machine that
                always begins in state SCAN_START
                when a token begins.
                
                So don't be too frightened, and read carefully.
                Don't expect _many_ comments beyond this point.
                
                Grab a coke before you try and dive into this.
            */
            switch(self->state)
            {
                case SCAN_START:
                {
                    switch(c)
                    {
                        case '0':
                            self->state = SCAN_LEADING_ZERO;
                            self->tokenType = TOKEN_INT;
                            break;
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            self->state = SCAN_INT_DIGITS;
                            self->tokenType = TOKEN_INT;
                            break;
                        case ';':
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_SEMI;
                            break;
                        case '+':
                            self->state = SCAN_LEADING_SIGN;
                            self->tokenType = TOKEN_PLUS;
                            break;
                        case '-':
                            self->state = SCAN_LEADING_SIGN;
                            self->tokenType = TOKEN_CHAR;
                            break;
                        case '.':
                            self->state = SCAN_REAL_DECIMAL_DIGITS;
                            self->tokenType = TOKEN_CHAR;
                            break;
                        case 'c':
                        case 'C':
                            self->state = SCAN_C;
                            self->tokenType = TOKEN_CHAR;
                            break;
                        case 's':
                        case 'S':
                            self->state = SCAN_S;
                            self->tokenType = TOKEN_CHAR;
                            break;
                        case '(':
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_LPAREN;
                            break;
                        case ')':
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_RPAREN;
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_CHAR;
                            break;
                    }
                    break;
                }
                case SCAN_LEADING_ZERO:
                {
                    switch(c)
                    {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                            self->state = SCAN_OCT_DIGITS;
                            self->tokenType = TOKEN_OCT;
                            break;
                        case '8':
                        case '9':
                            self->state = SCAN_INT_DIGITS;
                            self->tokenType = TOKEN_INT;
                            break;
                        case '.':
                            self->state = SCAN_REAL_DECIMAL_DIGITS;
                            /* String, unless a . is followed a digit. */
                            self->tokenType = TOKEN_STR;
                            break;
                        case 'x':
                        case 'X':
                            self->state = SCAN_HEX_DIGITS;
                            /* String, unless a 0x is followed a digit. */
                            self->tokenType = TOKEN_STR;
                            break;
                        case 'e':
                        case 'E':
                            self->state = SCAN_REAL_EXP;
                            /* String, unless there's a number (or sign-then-number) after this. */
                            self->tokenType = TOKEN_STR;
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_LEADING_SIGN:
                {
                    switch(c)
                    {
                        case '0':
                            self->state = SCAN_LEADING_ZERO;
                            self->tokenType = TOKEN_INT;
                            break;
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            self->state = SCAN_INT_DIGITS;
                            self->tokenType = TOKEN_INT;
                            break;
                        case '.':
                            self->state = SCAN_REAL_DECIMAL_DIGITS;
                            self->tokenType = TOKEN_REAL;
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_INT_DIGITS:
                {
                    switch(c)
                    {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            self->state = SCAN_INT_DIGITS;
                            self->tokenType = TOKEN_INT;
                            break;
                        case '.':
                            self->state = SCAN_REAL_DECIMAL_DIGITS;
                            self->tokenType = TOKEN_REAL;
                            break;
                        case 'e':
                        case 'E':
                            self->state = SCAN_REAL_EXP;
                            /* String, unless there's a number (or sign-then-number) after this. */
                            self->tokenType = TOKEN_STR;
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_REAL_DECIMAL_DIGITS:
                {
                    switch(c)
                    {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            self->state = SCAN_REAL_DECIMAL_DIGITS;
                            self->tokenType = TOKEN_REAL;
                            break;
                        case 'e':
                        case 'E':
                            self->state = SCAN_REAL_EXP;
                            /* String, unless there's a number (or sign-then-number) after this. */
                            self->tokenType = TOKEN_STR;
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_REAL_EXP:
                {
                    switch(c)
                    {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            self->state = SCAN_REAL_EXP_DIGITS;
                            self->tokenType = TOKEN_REAL;
                            break;
                        case '+':
                        case '-':
                            self->state = SCAN_REAL_EXP_DIGITS;
                            /* String, unless there's a digit after this. */
                            self->tokenType = TOKEN_STR; 
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_REAL_EXP_DIGITS:
                {
                    switch(c)
                    {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            self->state = SCAN_REAL_EXP_DIGITS;
                            self->tokenType = TOKEN_REAL;
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_OCT_DIGITS:
                {
                    switch(c)
                    {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                            self->state = SCAN_OCT_DIGITS;
                            self->tokenType = TOKEN_OCT;
                            break;
                        case '8':
                        case '9':
                            self->state = SCAN_INT_DIGITS;
                            self->tokenType = TOKEN_INT;
                            break;
                        case '.':
                            self->state = SCAN_REAL_DECIMAL_DIGITS;
                            self->tokenType = TOKEN_REAL;
                            break;
                        case 'e':
                        case 'E':
                            self->state = SCAN_REAL_EXP;
                            /* String, unless there's a number (or sign-then-number) after this. */
                            self->tokenType = TOKEN_STR;
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_HEX_DIGITS:
                {
                    switch(c)
                    {
                        /* the first ten digits */
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                        /* Lower-case letter digits */
                        case 'a':
                        case 'b':
                        case 'c':
                        case 'd':
                        case 'e':
                        case 'f':
                        /* Upper-case letter digits */
                        case 'A':
                        case 'B':
                        case 'C':
                        case 'D':
                        case 'E':
                        case 'F':
                            self->state = SCAN_HEX_DIGITS;
                            self->tokenType = TOKEN_HEX;
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_C:
                {
                    switch(c)
                    {
                        case 'A':
                        case 'a':
                            self->state = SCAN_CA;
                            self->tokenType = TOKEN_STR;                            
                            break;
                        case 'D':
                        case 'd':
                            self->state = SCAN_CD;
                            self->tokenType = TOKEN_STR;                            
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }  
                case SCAN_CA:
                {
                    switch(c)
                    {
                        case 'R':
                        case 'r':
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_CAR;                            
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_CD:
                {
                    switch(c)
                    {
                        case 'R':
                        case 'r':
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_CDR;
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_S:
                {
                    switch(c)
                    {
                        case 'E':
                        case 'e':
                            self->state = SCAN_SE;
                            self->tokenType = TOKEN_STR;
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_SE:
                {
                    switch(c)
                    {
                        case 'T':
                        case 't':
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_SET;                            
                            break;
                        default:
                            self->state = SCAN_STR;
                            self->tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_STR:
                {
                    self->tokenType = TOKEN_STR;
                    break;
                }
            }
        }
        
        /* Keep reading lines into a buffer, until we reach EOF. */
        if(fgets(self->buffer, SCANNER_BUFFER_SIZE, self->file))
        {
            self->position = 0;
            self->len = strlen(self->buffer);
        }
        else
        {
            self->tokenType = TOKEN_EOF;
        }
    }
    /* Quit when we hit an EOF */
    while(self->tokenType != TOKEN_EOF);
    
    return TOKEN_EOF;
}

int ScannerGetLineNumber(Scanner* self)
{
    return self->lineNumber;
}
