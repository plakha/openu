#ifndef MMN14_TYPES
#define MMN14_TYPES

typedef unsigned char byte;


typedef
struct half_word
{
    byte bytes[4];
}
half_word_t;


typedef
struct word
{
    byte bytes[4];
}
word_t;

#endif /* MMN14_TYPES */