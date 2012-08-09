#ifndef BOOLEAN_H
#define BOOLEAN_H

/* Boolean type */
typedef enum { false, true } Boolean;

/* Some of my code expects a bool, due to stdbool.h, but for compatibility
   I use this enum instead. */
typedef Boolean bool;

#endif
