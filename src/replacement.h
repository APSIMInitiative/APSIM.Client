#ifndef _REPLACEMENT_H
#define _REPLACEMENT_H

#include <stdint.h>
typedef struct Replacement
{
    char* path;
    int32_t paramType;
    char* value;
    int value_len;
} replacement_t;

replacement_t* createIntReplacement(char* path, int32_t value);
replacement_t* createDoubleReplacement(char* path, double value);

// Free a replacement instance.
void replacement_free(replacement_t* replacement);

// TODO: bool, date, string replacements.

#endif
