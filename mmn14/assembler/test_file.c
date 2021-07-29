#include <stdio.h>
#include <string.h>

#include "file.h"

#ifndef MAX_LINE
#define MAX_LINE (81)
#endif

static const char *ignore_chars = " \t\n\r\f\v";

int main(int argc, char const *argv[])
{
    char buf[MAX_LINE] = {};
    FILE *file = fopen(argv[1], "r");
    enum get_line_status status = GOT_LINE;

    do{
        printf("status=%d\n", status = GetLine(file, buf, MAX_LINE, " \v\n", ' '));
        if (0 == strcmp(buf, ""))
        {
            printf("empty line");
        }
        puts(buf);
    }
    while (END_OF_FILE != status);

    fclose(file);

    printf("%ld", strlen(buf));
    

    return 0;
}
