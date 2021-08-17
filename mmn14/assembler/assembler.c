#include <stdio.h> /* fprintf() */
#include <stdlib.h> 
#include <assert.h>
#include <string.h>

#include "mmn14_types.h" /* */
#include "file.h"
#include "ram.h"
#include "sym_tab.h"
#include "parser.h"

#define MAX_FILENAME_LEN (255)

enum status {OK = 0, ERROR_STATUS};

static const char *param_extension = ".as";

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

    /* for each file passed as parameter */
    for (i = 1; i < num_filepaths; ++i)
    {
        int file_status = CheckFileName(filepaths[i]);
        char filename[MAX_FILENAME_LEN] = {'\0'};
        enum get_line_status line_status = 0;

        FILE *pfile = NULL;
        ram_t *ram = NULL;
        sym_tab_t *sym_tab = NULL;
        parser_t *parser = NULL;


        if (file_status != OK)
        {
            continue;
        }


        GetFileName(filepaths[i], filename);

        pfile = fopen(filepaths[i], "r");
        if (NULL == pfile)
        {
            fprintf(stderr, "ERROR: The file %s doesn't exist\n", filepaths[i]);
            continue;
        }

        sym_tab = SymTabCreate();
        if(!sym_tab)
        {
            fprintf(stderr, "FATAL ERROR: Couldn't allocate memory, while running line %d in file %s\n", __LINE__, __FILE__);

            return ERROR;
        }

        ram = RAMCreate();
        if(!sym_tab || !ram)
        {
            RAMDestroy(ram);
            fprintf(stderr, "FATAL ERROR: Couldn't allocate memory, while running line %d in file %s\n", __LINE__, __FILE__);

            return ERROR;
        }

        parser = ParserCreate(filepaths[i], ram, sym_tab);
        if(!parser)
        {
            RAMDestroy(ram);
            SymTabCreate(sym_tab);
            fprintf(stderr, "Couldn't allocate memory, while running line %d in file %s\n", __LINE__, __FILE__);

            return ERROR;
        }


        do
        {
            char line_buf[MAX_LINE_LEN] = {'\0'};
            dvec_t *args_arr = NULL;

            if (LINE_TOO_LONG == line_status)
            {
                #ifndef NDEBUG
                puts("LINE_TOO_LONG");
                #endif
                line_status = FileGetLine(pfile, line_buf, MAX_LINE_LEN, SPACE_CHARS, ' ');
                continue;
            }    

            line_status = FileGetLine(pfile, line_buf, MAX_LINE_LEN, SPACE_CHARS, ' ');
            puts(line_buf);
            args_arr = FileLineToArgs(line_buf);
            if (NULL == args_arr)
            {
                fprintf(stderr, "FATAL ERROR: could not allocate memory while running line %d in %s/n", __LINE__, __FILE__);
        
                return ERROR_STATUS;
            }

            ParserFirstPass(parser, args_arr, -1);
            /* send to 1st pass */
       
        }
        while (END_OF_FILE != line_status);
    }

    return status;
}

static int CheckUsage(int num_filepaths, const char *filepaths[])
{
    int status = (num_filepaths > 1) ? OK : ERROR_STATUS;

    if (OK != status)
    {
        printf("USAGE: %s file1.as file2.as ...\n", filepaths[0]);
    }

    return status;
}

/* ends with .as; print error as needed*/
static int CheckFileName(const char *filename)
{
    const size_t param_ext_len = strlen(param_extension);

    assert(filename); 

    if (strcmp(filename + strlen(filename) - param_ext_len, param_extension))
    {
        printf("ERROR in filename \"%s\": expected extension \"%s\"\n", filename, param_extension);
        return ERROR;
    }
    return OK;
}



/* put filename with no extension in filename_buf*/
static void GetFileName(const char *filepath, char *filename_buf)
{
    size_t new_len = -1;
    const size_t param_ext_len = strlen(param_extension);

    assert(OK == CheckFileName(filepath));

    new_len = strlen(filepath) - param_ext_len;

    strncpy(filename_buf, filepath, new_len);
    filename_buf[new_len - 1] = '\0';
    
    return;
}
