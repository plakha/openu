#ifndef MMN14_TYPES
#define MMN14_TYPES

#ifndef MAX_LINE_LEN
#define MAX_LINE_LEN (81)
#endif

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

typedef char line_arg_t[MAX_LINE_LEN];
typedef line_args_arr_t[(MAX_LINE_LEN / 2 + 1)];

#endif /* MMN14_TYPES */
