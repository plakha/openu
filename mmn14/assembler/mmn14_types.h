#ifndef MAX_LINE_LEN
#define MAX_LINE_LEN (81)
#endif

#ifndef MMN14_TYPES
#define MMN14_TYPES


#define SPACE_CHARS (" \v\n")

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

typedef char line_arg_t[MAX_LINE_LEN]; /* string of MAX_LINE_LEN length */

enum data_type {VOID, BYTE, HALF_WORD, WORD, ASCIZ};

size_t SizeOfDataType(enum data_type data_type)
{
    switch (data_type)
    {
    case BYTE:
        return sizeof(byte);
        break;
    case HALF_WORD:
        return sizeof(half_word_t);
        break;

    case WORD:
        return sizeof(word_t);
        break;
    case ASCIZ:
        return sizeof(char);
        break;
    case VOID:
        assert(FALSE);
        break;
    default:
        assert(FALSE);
        break;
    }

    return -1lu;
}



/* size_t ULONG_MAX = -1lu; */


#endif /* MMN14_TYPES */
