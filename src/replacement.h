#include <stdint.h>
struct Replacement
{
    char* path;
    int32_t paramType;
    char* value;
    int value_len;
};

struct Replacement* createIntReplacement(char* path, int32_t value);
struct Replacement* createDoubleReplacement(char* path, double value);

// Free a replacement instance.
void replacement_free(struct Replacement* replacement);

// TODO: bool, date, string replacements.
