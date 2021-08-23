#ifndef MAX_LINE_LEN
#define MAX_LINE_LEN (81)
#endif

#ifndef MMN14_TYPES
#define MMN14_TYPES


#define SPACE_CHARS (" \v\n")

#define SIZE_OF_WORD (4)

typedef unsigned char byte;


typedef
struct half_word
{
    byte bytes[SIZE_OF_WORD];
}
half_word_t;


typedef
struct word
{
    byte bytes[SIZE_OF_WORD];
}
word_t;

typedef char line_arg_t[MAX_LINE_LEN]; /* string of MAX_LINE_LEN length */

enum data_type {VOID = 0, BYTE, HALF_WORD, WORD, ASCIZ};




/* size_t ULONG_MAX = -1lu; */


#endif /* MMN14_TYPES */
