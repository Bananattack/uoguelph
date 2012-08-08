/*

    Andrew Crowell
    0545826
    acrowell@uoguelph.ca
    2010-01-15

    --

    The 'scanner' program.
    
    Takes input from stdin, scans it, and dumps tokens into stdout as they are scanned.
    Stops when EOF is encountered, or forcibly killed somehow.
    
*/
#include <stdio.h>
#include <string.h>

void scan(FILE* input, FILE* output);

int main(int argc, char** argv)
{
    scan(stdin, stdout);
    return 0;
}

/*

    These are the various states used by the scanner. 

    Each state treats character inputs differently, resulting in
    switches between states and the determined token output.

*/
typedef enum
{
    SCAN_START = 0x0,
    SCAN_LEADING_ZERO = 0x1,
    SCAN_LEADING_SIGN = 0x2,
    SCAN_INT_DIGITS = 0x3,
    SCAN_REAL_DECIMAL_DIGITS = 0x4,
    SCAN_REAL_EXP = 0x5,
    SCAN_REAL_EXP_DIGITS = 0x6,
    SCAN_OCT_DIGITS = 0x7,
    SCAN_HEX_DIGITS = 0x8,
    SCAN_C = 0x9,
    SCAN_CA = 0xA,
    SCAN_CD = 0xB,
    SCAN_STR = 0xC,
} ScanState;

/*

    An enumeration of the types of tokens to be handled,
    according to specification.

*/
typedef enum
{
    TOKEN_EMPTY = 0x0,
    TOKEN_INT = 0x1,
    TOKEN_HEX = 0x2,
    TOKEN_OCT = 0x3,
    TOKEN_REAL = 0x4,
    TOKEN_LPAREN = 0x5,
    TOKEN_RPAREN = 0x6,
    TOKEN_CAR = 0x7,
    TOKEN_CDR = 0x8,
    TOKEN_CHAR = 0x9,
    TOKEN_STR = 0xA,
} TokenType;

/*

    The text associated with each of these token types.

    Used as lookup to figure out what thing to dump when a whitespace
    delimiter is encountered.

*/
const char* const tokenText[] = {
    "?",
    "INT",
    "HEX",
    "OCT",
    "REAL",
    "LPAREN",
    "RPAREN",
    "CAR",
    "CDR",
    "CHAR",
    "STR",
};


/*

    This scans a file and emits tokenized output based on
    a full view of the document.

    Takes a first FILE* that is opened for reading, for input purposes.
    Reads this file, figures out what the types of tokens are contained in the file,
    printing determined tokens to a second FILE* that is opened for writing.

    Keeps reading from the input until EOF or IO error.

*/
void scan(FILE* input, FILE* output)
{
    /* Data buffer related locals. */
    int i; 
    char c;
    const int BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];
    
    /* Current scanning state things. */
    int delimiter;
    TokenType tokenType;
    ScanState state; 

    /* Reset scanning state. */
    delimiter = 1;
    tokenType = TOKEN_EMPTY;
    state = SCAN_START;
    
    /* Keep reading lines into a buffer, until we reach EOF or something dies. */
    while(fgets(buffer, BUFFER_SIZE, input))
    {
        /* Read from the buffer. */
        for(i = 0; i < strlen(buffer); i++)
        {
            c = buffer[i];
        
            /* Whitespace delimiter. */
            if(c == ' ' || c == '\t' || c == '\r' || c == '\n')
            {
                /*
                    If we've cleared the delimiter flag,
                    this means we've read at least one non-delimiter character,
                    and can emit a known token.
                */
                if(!delimiter)
                {
                    fputs(tokenText[tokenType], output);
                    fputs(" ", output);
                }
                
                /* Reset the scanning state. */
                delimiter = 1;
                tokenType = TOKEN_EMPTY;
                state = SCAN_START;
                
                /* Skip forward. */
                continue;
            }
        
            delimiter = 0;

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
            switch(state)
            {
                case SCAN_START:
                {
                    switch(c)
                    {
                        case '0':
                            state = SCAN_LEADING_ZERO;
                            tokenType = TOKEN_INT;
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
                            state = SCAN_INT_DIGITS;
                            tokenType = TOKEN_INT;
                            break;
                        case '+':
                        case '-':
                            state = SCAN_LEADING_SIGN;
                            tokenType = TOKEN_CHAR;
                            break;
                        case '.':
                            state = SCAN_REAL_DECIMAL_DIGITS;
                            tokenType = TOKEN_CHAR;
                            break;
                        case 'c':
                        case 'C':
                            state = SCAN_C;
                            tokenType = TOKEN_CHAR;
                            break;
                        case '(':
                            state = SCAN_STR;
                            tokenType = TOKEN_LPAREN;
                            break;
                        case ')':
                            state = SCAN_STR;
                            tokenType = TOKEN_RPAREN;
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_CHAR;
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
                            state = SCAN_OCT_DIGITS;
                            tokenType = TOKEN_OCT;
                            break;
                        case '8':
                        case '9':
                            state = SCAN_INT_DIGITS;
                            tokenType = TOKEN_INT;
                            break;
                        case '.':
                            state = SCAN_REAL_DECIMAL_DIGITS;
                            tokenType = TOKEN_REAL;
                            break;
                        case 'x':
                        case 'X':
                            state = SCAN_HEX_DIGITS;
                            /* String, unless a 0x is followed a digit. */
                            tokenType = TOKEN_STR;
                            break;
                        case 'e':
                        case 'E':
                            state = SCAN_REAL_EXP;
                            /* String, unless there's a number (or sign-then-number) after this. */
                            tokenType = TOKEN_STR;
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_LEADING_SIGN:
                {
                    switch(c)
                    {
                        case '0':
                            state = SCAN_LEADING_ZERO;
                            tokenType = TOKEN_INT;
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
                            state = SCAN_INT_DIGITS;
                            tokenType = TOKEN_INT;
                            break;
                        case '.':
                            state = SCAN_REAL_DECIMAL_DIGITS;
                            tokenType = TOKEN_STR;
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
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
                            state = SCAN_INT_DIGITS;
                            tokenType = TOKEN_INT;
                            break;
                        case '.':
                            state = SCAN_REAL_DECIMAL_DIGITS;
                            tokenType = TOKEN_REAL;
                            break;
                        case 'e':
                        case 'E':
                            state = SCAN_REAL_EXP;
                            /* String, unless there's a number (or sign-then-number) after this. */
                            tokenType = TOKEN_STR;
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
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
                            state = SCAN_REAL_DECIMAL_DIGITS;
                            tokenType = TOKEN_REAL;
                            break;
                        case 'e':
                        case 'E':
                            /* Dirty hack to prevent +.e0 like expressions. */
                            if(tokenType == TOKEN_CHAR || tokenType == TOKEN_STR)
                            {
                                state = SCAN_STR;
                            }
                            else
                            {
                                state = SCAN_REAL_EXP;
                            }
                            /* String, unless there's a number (or sign-then-number) after this. */
                            tokenType = TOKEN_STR;
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
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
                            state = SCAN_REAL_EXP_DIGITS;
                            tokenType = TOKEN_REAL;
                            break;
                        case '+':
                        case '-':
                            state = SCAN_REAL_EXP_DIGITS;
                            /* String, unless there's a digit after this. */
                            tokenType = TOKEN_STR; 
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
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
                            state = SCAN_REAL_EXP_DIGITS;
                            tokenType = TOKEN_REAL;
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
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
                            state = SCAN_OCT_DIGITS;
                            tokenType = TOKEN_OCT;
                            break;
                        case '8':
                        case '9':
                            state = SCAN_INT_DIGITS;
                            tokenType = TOKEN_INT;
                            break;
                        case '.':
                            state = SCAN_REAL_DECIMAL_DIGITS;
                            tokenType = TOKEN_REAL;
                            break;
                        case 'e':
                        case 'E':
                            state = SCAN_REAL_EXP;
                            /* String, unless there's a number (or sign-then-number) after this. */
                            tokenType = TOKEN_STR;
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
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
                            state = SCAN_HEX_DIGITS;
                            tokenType = TOKEN_HEX;
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
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
                            state = SCAN_CA;
                            tokenType = TOKEN_STR;                            
                            break;
                        case 'D':
                        case 'd':
                            state = SCAN_CD;
                            tokenType = TOKEN_STR;                            
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
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
                            state = SCAN_STR;
                            tokenType = TOKEN_CAR;                            
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
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
                            state = SCAN_STR;
                            tokenType = TOKEN_CDR;
                            break;
                        default:
                            state = SCAN_STR;
                            tokenType = TOKEN_STR;
                            break;
                    }
                    break;
                }
                case SCAN_STR:
                {
                    tokenType = TOKEN_STR;
                    break;
                }
            }
        }
    }
}
