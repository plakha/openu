#include <stdio.h> 
#include <assert.h>

void PrintFile(char *file_directory);

int main()
{
    PrintFile(__FILE__);

    return 0;
}

void PrintFile(char *file_directory)
{
    int c = 0;
    FILE *fd = NULL;
    
    fd = fopen(file_directory, "r");
    if (!fd)
    {
        fprintf(stderr, "The file %s could not be opened", file_directory);
    }

    while (EOF != (c = fgetc(fd)))
    {
        assert (!feof(fd));

        /* Windows editors have new line like this: "\n\r", while Unix/Linux - like this: "\n" */
        if (c == '\r')
        {
            #ifdef __unix__
                continue;
            #else

            #endif
        }

        putchar(c);
    }
    
    fclose(fd);
}