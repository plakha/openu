#include <stdio.h> /* fprintf() */
#include <stdlib.h> 
#include "mmn14_types.h" /* */
#include "file.h"

#define MAX_FILENAME_LEN (255)


enum status {OK = 0, ERROR};


static int CheckUsage(int num_filepaths, const char const *filepaths);
static int CheckFileName(char filename);

static void GetFileName(const char *filepaths, char *filename_buf);

int main(int argc, char const *argv[])
{
    const char  **filepaths = argv;
    const int num_filepaths = argc;

    int status = OK;
    int i = 0;

    if(OK != (status = CheckUsage(num_filepaths, filepaths)))
    {
        return status;
    }

    for (i = 0; i < num_filepaths; ++i)
    {
        int file_status = CheckFileName(filepaths[i]);
        char filename[MAX_FILENAME_LEN] = {};
        int line_status = 0;
        FILE *pfile = NULL;
        RAM *ram = NULL;
        SymTab *sym_tab = NULL;

        if (file_status != OK)
        {
            continue;
        }

        file_status = CheckFileExists(filepaths[i]);
        if (file_status != OK)
        {
            continue;
        }

        GetFileName(filepaths[i], filename);

        pfile = fopen(filepaths[i], "r");
        if (NULL == pfile)
        {
            fprintf(stderr, "The file %s doesn't exist", filepaths[i]);
        }

        do{
            char line_buf[MAX_LINE_LEN] = {};
            if (LINE_TOO_LONG == line_status)
            {
                line_status = GetLine(filepaths[i], buf, MAX_LINE_LEN, " \v\n", ' '));
                continue;
            }    

            line_status = GetLine(filepaths[i], buf, MAX_LINE_LEN, " \v\n", ' '));
            /* Get array of strings */
            /* send to 1st pass */
       
        }
        while (END_OF_FILE != status);


        


    }

    return 0;
}

static int CheckUsage(int num_filepaths, const char const *filepaths[])
{
    int status = (num_filepaths > 0) ? OK : ERROR;

    if (OK != status)
    {
        printf("USAGE: %s file1.as file2.as ...", filepaths[0]);
    }

    return status;
}

/* ends with .as; print error as needed*/
/* maybe inside "" or ''  */
static int CheckFileName(char filename)
{
    return OK;
}



/* put filename with no extension in filename_buf*/
static void GetFileName(const char *filepaths, char *filename_buf)
{
    return;
}
