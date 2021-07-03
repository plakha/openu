#include <stdio.h>
#include <stdlib.h>

typedef unsigned char word_t[4];

struct r_instruction
{
    size_t opcode : 6;
    size_t rs : 5;
    //char rt : 5;
    //char rd : 5;
    //char funct : 5;
    //char unused_filler : 5;
};

int main(int argc, char const *argv[])
{
    printf("%ld\n", sizeof(int));
    size_t a = 1;
    struct  r_instruction r_in = {};
    printf("%#x\n", *(1 + (unsigned char *)&r_in));

    r_in.opcode = a;

    printf("%x\n", *(1 + (unsigned char *)&r_in));
    
    return 0;
}
