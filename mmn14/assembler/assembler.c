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

enum {FALSE = 0, TRUE};

static const char *param_extension = ".as";

static int ProcessFile(const char *filepath);
static int CheckUsage(int num_filepaths, const char *filepaths[]);
static int CheckFileName(const char *filename);

static char *GetExtensionlessFileName(const char *filepath, char *filename_buf);
static int CreateObjFile(const char *extensionless_filename, const parser_t *parser, const ram_t *ram, const sym_tab_t *sym_tab);

static int CreateEntFile(const char *extensionless_filename, 
    const parser_t *parser, const ram_t *ram, const sym_tab_t *sym_tab);
    
static int PrintSymTabData(FILE *p_obj_file, const parser_t *parser, const ram_t *ram, const sym_tab_t *sym_tab);


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
        int this_file_status = ProcessFile(filepaths[i]);
        status = OK == this_file_status ? status : this_file_status;
    }

    return status;
}

static int ProcessFile(const char *filepath)
{
    int file_status = CheckFileName(filepath);
    enum get_line_status line_status = 0;

    FILE *pfile = NULL;
    ram_t *ram = NULL;
    sym_tab_t *sym_tab = NULL;
    parser_t *parser = NULL;

    if (file_status != OK)
    {
        return file_status;
    }

    pfile = fopen(filepath, "r");
    if (NULL == pfile)
    {
        printf("ERROR: The file %s doesn't exist\n", filepath);
        return SYNT_ERR;
    }

    sym_tab = SymTabCreate();
    ram = RAMCreate();
    if(!sym_tab || !ram)
    {
        if (ram) 
        {
            RAMDestroy(ram); 
            ram = NULL;
        }
        if (sym_tab) 
        {
            SymTabDestroy(sym_tab); 
            sym_tab = NULL;
        }
        fclose(pfile); pfile = NULL;
        fprintf(stderr, 
        "MEMORY ERROR: Couldn't allocate memory, while running line %d in file %s\n", 
        __LINE__, __FILE__);

        return MEM_ERR;
    }

    parser = ParserCreate(filepath, ram, sym_tab);
    if(!parser)
    {
        RAMDestroy(ram); 
        ram = NULL;
        SymTabCreate(sym_tab); 
        sym_tab = NULL;
        fclose(pfile); 
        pfile = NULL;
        fprintf(stderr, 
        "MEMORY ERROR: Couldn't allocate memory, while running line %d in file %s\n", 
        __LINE__, __FILE__);

        return MEM_ERR;
    }

    do
    {
        char line_buf[MAX_LINE_LEN] = {'\0'};
        dvec_t *args_arr = NULL;

        if (LINE_TOO_LONG == line_status)
        {
            printf("WARNING: line %lu statement %s must be no longer than %d characters\n", 
            ParserGetCurLineNum(parser), line_buf, MAX_LINE_LEN - 1);
            line_status = FileGetLine(pfile, line_buf, MAX_LINE_LEN - 1, SPACE_CHARS, ' ');

            ParserFailParser(parser);

            continue;
        }    

        line_status = FileGetLine(pfile, line_buf, MAX_LINE_LEN - 1, SPACE_CHARS, ' ');
        #ifndef NDEBUG
        puts(line_buf);
        #endif
        args_arr = FileLineToArgs(line_buf);
        if (NULL == args_arr)
        {
            ParserDestroy(parser, TRUE); parser = NULL;
            ram = NULL;
            sym_tab = NULL;
            fclose(pfile); pfile = NULL;
            fprintf(stderr, 
            "MEMORY ERROR: Couldn't allocate memory, while running line %d in file %s\n", 
            __LINE__, __FILE__);

            return MEM_ERR;
        }

        if (MEM_ERR == ParserFirstPass(parser, args_arr))
        {
            ParserDestroy(parser, TRUE); parser = NULL;
            ram = NULL;
            sym_tab = NULL;
            fclose(pfile); pfile = NULL;
            fprintf(stderr, 
            "MEMORY ERROR: Couldn't allocate memory, while running line %d in file %s\n", 
            __LINE__, __FILE__);

            return MEM_ERR;
        }

        FileFreeArgs(args_arr); 
        args_arr = NULL;      
    }
    while (END_OF_FILE != line_status);

    #ifndef NDEBUG
    printf("Ended first run. IC=%lu , LineCount=%lu, is syntax corrupt? %d \n", ParserGetIC(parser), ParserGetCurLineNum(parser), ParserIsSyntaxCorrupt(parser));
    #endif

    if (!ParserIsSyntaxCorrupt(parser))
    {
        ParserResetLineCount(parser);
        rewind(pfile);
        #ifndef NDEBUG
        puts("Start second pass input file read.");
        #endif
        do
        {
            char line_buf[MAX_LINE_LEN] = {'\0'};
            dvec_t *args_arr = NULL;

            if (LINE_TOO_LONG == line_status)
            {
                printf("WARNING: line %lu statement %s must be no longer than %d characters\n", 
                ParserGetCurLineNum(parser), line_buf, MAX_LINE_LEN - 1);
                line_status = FileGetLine(pfile, line_buf, MAX_LINE_LEN - 1, SPACE_CHARS, ' ');

                ParserFailParser(parser);

                continue;
            }    

            line_status = FileGetLine(pfile, line_buf, MAX_LINE_LEN - 1, SPACE_CHARS, ' ');
            #ifndef NDEBUG
            puts(line_buf);
            #endif
            args_arr = FileLineToArgs(line_buf);
            if (NULL == args_arr)
            {
                ParserDestroy(parser, TRUE); parser = NULL;
                ram = NULL;
                sym_tab = NULL;
                fclose(pfile); pfile = NULL;
                fprintf(stderr, 
                "MEMORY ERROR: Couldn't allocate memory, while running line %d in file %s\n", 
                __LINE__, __FILE__);

                return MEM_ERR;
            }

            if (MEM_ERR == ParserSecondPass(parser, args_arr))
            {
                ParserDestroy(parser, TRUE); parser = NULL;
                ram = NULL;
                sym_tab = NULL;
                fclose(pfile); pfile = NULL;
                fprintf(stderr, 
                "MEMORY ERROR: Couldn't allocate memory, while running line %d in file %s\n", 
                __LINE__, __FILE__);

                return MEM_ERR;
            }

            FileFreeArgs(args_arr);  
            args_arr = NULL;     
        }
        while (END_OF_FILE != line_status);
    }
    fclose(pfile);
    pfile = NULL;

    if (!ParserIsSyntaxCorrupt(parser))
    {
        /* create output from ram, from sym_tab*/
        char filename_extensionless[MAX_FILENAME_LEN] = {'\0'}; /* will be used to store output filname via GetFileName() */
        GetExtensionlessFileName(filepath, filename_extensionless);
        CreateObjFile(filename_extensionless, parser, ram, sym_tab);
        CreateEntFile(filename_extensionless, parser, ram, sym_tab);
        /* CreateExtFile(filename_extensionless, parser, ram, sym_tab);*/

    }

    ParserDestroy(parser, TRUE);
    parser = NULL;
    ram = NULL;

    return OK;
}

/*

*/
static int CreateObjFile(const char *extensionless_filename, 
    const parser_t *parser, const ram_t *ram, const sym_tab_t *sym_tab)
{
    size_t i = 0;
    size_t ram_size = -1;
    char filename[MAX_FILENAME_LEN] = {'\0'};
    static const char *ob_ext = ".ob";
    FILE *p_ob_file = NULL;
    unsigned long byte_count = RAM_START_ADDRESS;

    assert(extensionless_filename); 
    assert(parser); 
    assert(ram); 
    assert(sym_tab);
    assert(!ParserIsSyntaxCorrupt(parser));
    assert(ParserGetIC(parser) == RAMSize(ram));

    ram_size = RAMSize(ram);
    strcpy(filename, extensionless_filename);
    strcat(filename, ob_ext);

    p_ob_file = fopen(filename, "w");
    if (!p_ob_file)
    {
        fprintf(stderr, "SYSTEM ERROR: could not create the file ");
        puts(filename);
        
        return MEM_ERR;
    }
   
    fprintf(p_ob_file, "     %lu %lu\n", RAMSize(ram) * sizeof(word_t), SymTabGetDataCount(sym_tab));
    for (i = 0; i < ram_size; ++i)
    {
        word_t *w = RAMGetIthWord(ram, i);
        byte *b = (byte *)w;
        int j = 0;
        fprintf(p_ob_file ,"%04lu", byte_count);
        for (; j < sizeof(*w); ++j)
        {
            fprintf(p_ob_file ," %02X", *(j + b));
        }
        byte_count += sizeof(word_t);
        fprintf(p_ob_file, "\n");
        (void)&PrintSymTabData; /* PrintSymTabData() not finished */
    }

    fclose(p_ob_file);
    p_ob_file = NULL;
    return OK;
}

static int CheckUsage(int num_filepaths, const char *filepaths[])
{
    int status = (num_filepaths > 1) ? OK : SYNT_ERR;

    if (OK != status)
    {
        printf("USAGE: %s file1.as file2.as ...\n", filepaths[0]);
    }

    return status;
}

/* typedef int (*act_f)(void *data, const void *params); */
static int UtilPrintEntrySymbols(void *data, const void *params)
{
    symbol_t *symbol = (symbol_t *)data;
    /* params is array of 3 pointers to void */
    void **param_arr = (void **)params;
    FILE *p_ent_file = (FILE *)(param_arr[0]);
    unsigned long *address = (unsigned long *)(param_arr[1]);
    sym_tab_t *sym_tab = (sym_tab_t *)(param_arr[2]);

    assert(p_ent_file);

    /*if Symbol is entry Print label,
        if Data print dc + given addressd
        if Code print saved_ic  */

    if (SymTabSymbolIsEntry(sym_tab, symbol))
    {
        fprintf(p_ent_file,"%s ", SymTabSymbolGetLabel(sym_tab, symbol));
        if (SymTabSymbolIsData(sym_tab, symbol))
        {
            unsigned long dc = *address + SymTabSymbolGetDC(sym_tab, symbol);
            fprintf(p_ent_file, "%lu", dc);

        }
        else if (SymTabSymbolIsCode(sym_tab, symbol))
        {
            unsigned long instr_addr = RAM_START_ADDRESS 
                + sizeof(word_t) * SymTabGetICDeclared(sym_tab, symbol);
            fprintf(p_ent_file, "%lu", instr_addr);

        }
        fprintf(p_ent_file,"\n");
    }

    return OK;
}

static int CreateEntFile(const char *extensionless_filename, 
    const parser_t *parser, const ram_t *ram, const sym_tab_t *sym_tab)
{
    int ret = OK;
    char filename[MAX_FILENAME_LEN] = {'\0'};
    static const char *ent_extension = ".ent";
    void *params[3] = {NULL};
    unsigned long start_address = RAM_START_ADDRESS + RAMSize(ram) * sizeof(word_t); 
    FILE *p_ent_file = NULL;

    strcpy(filename, extensionless_filename);
    strcat(filename, ent_extension);

    p_ent_file = fopen(filename, "w");
    if (!p_ent_file)
    {
        fprintf(stderr, "SYSTEM ERROR: could not create the file ");
        puts(filename);

        return MEM_ERR;
    }

    params[0] = (void *)p_ent_file;
    params[1] = (void *)&start_address;
    params[2] = (void *)sym_tab;

    ret = SymTabForEachSymbol((sym_tab_t *)sym_tab, &UtilPrintEntrySymbols, params);

    fclose(p_ent_file);
    p_ent_file = NULL;

    return ret;
}

/* ends with .as; print error as needed*/
static int CheckFileName(const char *filename)
{
    const size_t param_ext_len = strlen(param_extension);

    assert(filename); 

    if (strcmp(filename + strlen(filename) - param_ext_len, param_extension))
    {
        printf("ERROR in filename \"%s\": expected extension \"%s\"\n", filename, param_extension);
        return SYNT_ERR;
    }
    return OK;
}

/* typedef int (*act_f)(void *data, const void *params); */
/* unfinished */
static int UtilPrintSymData(void *data, const void *params)
{
    symbol_t *symbol = (symbol_t *)data;
    /* params is array of 3 pointers to void */
    FILE *p_ent_file = (FILE *)*(void **)params;
    unsigned long *address = (unsigned long *)(((void **)params)[1]);
    sym_tab_t *sym_tab = (sym_tab_t *)*((void **)params + 2);

    if (SymTabSymbolIsData(sym_tab, symbol))
    {
        /* GetDataVector, DataSize(Type), print byte-by-byte to the file */
    }
    

    (void)sym_tab;
    (void)address;
    (void)symbol;
    (void)p_ent_file;

    return OK;
}

/* unfinished */
static int PrintSymTabData(FILE *p_obj_file, const parser_t *parser, const ram_t *ram, const sym_tab_t *sym_tab)
{
    void *params[3] = {NULL};
    unsigned long start_address = RAM_START_ADDRESS + RAMSize(ram) * sizeof(word_t);

    params[0] = (void *)p_obj_file;
    params[1] = (void *)&start_address;
    params[2] = (void *)sym_tab;
    
    return SymTabForEachSymbol((sym_tab_t *)sym_tab, &UtilPrintSymData, params);
}

/* put filename with no extension in filename_buf*/
static char *GetExtensionlessFileName(const char *filepath, char *filename_buf)
{
    size_t new_len = -1;
    const size_t param_ext_len = strlen(param_extension);

    assert(OK == CheckFileName(filepath));

    new_len = strlen(filepath) - param_ext_len;

    strncpy(filename_buf, filepath, new_len);
    
    return filename_buf;
}
