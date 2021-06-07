#include <stdio.h> /* putchar() */
#include <assert.h> /* assert() */
#include <string.h> /* strncpy() */

#define BUF_LEN (100)

#ifdef __unix__ /* including linux */
#define EXT ("")
#else /* windows */
#define EXT (".exe")
#endif /* will not work on non-windows, non-unix systems */

/*
 takes run_file_name that ends with .exe on windows or with no extension on unix/linux and pus te source file name into the buffer src_file_name.
    the buffer should be initiated to all '\0'. The buffer should have enough size ro hold the file name
 */
static void SourceFileName(const char *run_file_name, char *src_file_name);


static void PrintFile(char *file_directory);

int main(int argc, char *argv[])
{
    char buf[BUF_LEN] = {0};
    const char *run_file_name = argv[0];

    SourceFileName(run_file_name, buf);
    PrintFile(buf);

    return 0;
}

/* the src_file_name has enough size and is filled with '\0' */
static void SourceFileName(const char *run_file_name, char *src_file_name)
{
    const char *c_ext = ".c";

    strncpy(src_file_name, run_file_name, strlen(run_file_name) - strlen(EXT));
    strcat(src_file_name, c_ext);
}

static void PrintFile(char *file_directory)
{
    int c = 0;
    FILE *fd = NULL;
    
    fd = fopen(file_directory, "r");
    if (!fd)
    {
        fprintf(stderr, "The file %s could not be opened", file_directory);
    }

    while ( EOF != (c = fgetc(fd)) )
    {
        assert (!feof(fd));

        /* Windows editors have new line like this: "\n\r", while Unix/Linux - like this: "\n"
            when porting windows created text files to linux, the '\r' will not be printed
         */
        if (c == '\r')
        {
            #ifdef __unix__
                continue;
            #else
                /* the char will be printed */
            #endif
        }

        putchar(c);
    }
    
    fclose(fd);
}

#undef BUF_LEN
#undef EXT