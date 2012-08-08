/*

    Andrew Crowell
    0545826
    acrowell@uoguelph.ca

    --

    The 'scanner' module.
    
    Takes input from stdin, scans it, and dumps tokens into stdout as they are scanned.
    Stops when EOF is encountered, or forcibly killed somehow.
    
*/
#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

/* Number of characters to read in */
#define SCANNER_BUFFER_SIZE 256

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
    SCAN_S = 0xD,
    SCAN_SE = 0xE,
} ScanState;

/*

    An enumeration of the types of tokens to be handled,
    according to specification.

*/
typedef enum
{
    TOKEN_EMPTY = 0x00,
    TOKEN_EOF = 0x01,
    TOKEN_INTERNAL_ERR = 0x02,
    
    TOKEN_INT = 0x03,
    TOKEN_HEX = 0x04,
    TOKEN_OCT = 0x05,
    TOKEN_REAL = 0x06,
    TOKEN_LPAREN = 0x07,
    TOKEN_RPAREN = 0x08,
    TOKEN_CAR = 0x09,
    TOKEN_CDR = 0x0A,
    TOKEN_SET = 0x0B,
    TOKEN_CHAR = 0x0C,
    TOKEN_STR = 0x0D,
    TOKEN_PLUS = 0x0E,
    TOKEN_SEMI = 0x0F,
} TokenType;

const char* TokenTypeGetDescription(TokenType tokenType);

typedef struct
{
    /* Token input source. */
    FILE* file;
    
    /* The kind of token that the scanner thinks it has read (so far) */
    TokenType tokenType;
    /* The current state within the DFA of the scanner. */
    ScanState state;
    
    /* Buffer containing a block of text from the input. */
    char buffer[SCANNER_BUFFER_SIZE];
    /* Position within the buffer. */
    int position;
    /* Size of buffer actually occupied. */
    int len;
    /* Current line number. */
    int lineNumber;
} Scanner;

Scanner* ScannerNew(FILE* file);
void ScannerFree(Scanner* self);
TokenType ScannerNext(Scanner* self);
int ScannerGetLineNumber(Scanner* self);

#endif

