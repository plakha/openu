#include <stdio.h> /* fprintf() */
#include <stdlib.h> 

#include "mmn14_types.h" /* */
#include "file.h"
#include "ram.h"
#include "sym_tab.h"
#include "parser.h"

#define MAX_FILENAME_LEN (255)



enum status {OK = 0, ERROR_STATUS};


static int CheckUsage(int num_filepaths, const char *filepaths[]);
static int CheckFileName(const char *filename);

static void GetFileName(const char *filepaths, char *filename_buf);

int main(int argc, char const *argv[])
{
    const char  **filepaths = argv;
    const int num_filepaths = argc;

    int status = OK;
    int i = 0;

    if (OK != (status = CheckUsage(num_filepaths, filepaths)))
    {
        return status;
    }

    for (i = 0; i < num_filepaths; ++i)
    {
        int file_status = CheckFileName(filepaths[i]);
        char filename[MAX_FILENAME_LEN] = {'\0'};
        int line_status = 0;
        FILE *pfile = NULL;
        ram_t *ram = NULL;
        sym_tab_t *sym_tab = NULL;
        parser_t *parser = NULL;
        size_t line_number = 0;


        if (file_status != OK)
        {
            continue;
        }


        GetFileName(filepaths[i], filename);

        pfile = fopen(filepaths[i], "r");
        if (NULL == pfile)
        {
            fprintf(stderr, "The file %s doesn't exist", filepaths[i]);
            continue;
        }

        do
        {
            char line_buf[MAX_LINE_LEN] = {'\0'};
            dvec_t *args_arr = NULL;

            if (LINE_TOO_LONG == line_status)
            {
                line_status = FileGetLine(pfile, line_buf, MAX_LINE_LEN, " \v\n", ' ');
                continue;
            }    

            line_status = FileGetLine(pfile, line_buf, MAX_LINE_LEN, " \v\n", ' ');
            args_arr = FileLineToArgs(line_buf);
            if (NULL == args_arr)
            {
                fprintf(stderr, "Cannot allocate memory while running line %d in %s/n", __LINE__, __FILE__);
        
                return ERROR_STATUS;
            }

            ParserFirstPass(parser, args_arr, line_number);
            /* send to 1st pass */
       
        }
        while (END_OF_FILE != status);


        


    }

    return 0;
}

static int CheckUsage(int num_filepaths, const char *filepaths[])
{
    int status = (num_filepaths > 0) ? OK : ERROR_STATUS;

    if (OK != status)
    {
        printf("USAGE: %s file1.as file2.as ...", filepaths[0]);
    }

    return status;
}

/* ends with .as; print error as needed*/
/* maybe inside "" or ''  */
static int CheckFileName(const char *filename)
{
    return OK;
}



/* put filename with no extension in filename_buf*/
static void GetFileName(const char *filepaths, char *filename_buf)
{
    return;
}
