#include <stdlib.h> /* malloc() */
#include <assert.h> /* assert() */
#include <string.h> /* strcpy() */
#include <stdio.h> /* fprintf() */

#include "dll.h"
#include "dvec.h"
#include "mmn14_types.h"

#include "sym_tab.h"

typedef enum {FALSE, TRUE} bool;


/* the assumption is that the declaration is in order, to kkep the data count.
So do not alter the data of n'th symbol 
after the (n + k)'th symbol has been declared */
struct sym_tab
{
    dll_t *symbol_table;
    size_t dc;
};


struct symbol
{
    const char label[32];
  


    bool is_entry; /* default FALSE,  */
    bool is_extern; /* default FALSE */
  
    /* either doce or data */
    bool is_data;
    bool is_code;

    /* for data. if code, default to -1 */
    size_t dc_declared;

    /* for code. If data, default to -1 */
    size_t ic_declared;

    /* fill in second pass. For extern only. */
    dvec_t *ic_referenced;
  
    /* for data only */
    enum data_type data_type;
    dvec_t *data_vector; /* array of words or array of half words or array of bytes */
};



sym_tab_t *SymTabCreate()
{
    sym_tab_t *new_sym_tab = malloc(sizeof(*new_sym_tab));

    return new_sym_tab;
}

/* typedef int (*cmp_f)(const void *data, const void *key, const void *params); */
/* If same label - return 0 */
static int CmpSymbolByLabel(const void *compared_symbol, const void *given_label, 
const void *void_param)
{
    assert(compared_symbol);
    assert(given_label);
    assert(!void_param);

    assert(((symbol_t *)compared_symbol)->label);

    (void)void_param;

    return strcmp(((symbol_t *)compared_symbol)->label, (const char *)given_label);
}

int SymTabHasSymbol(const sym_tab_t *sym_tab, const char *label)
{
    assert(sym_tab);
    assert(label);    
    
    return NULL != SymTabGetSymbol((sym_tab_t *)sym_tab, label);
}

symbol_t *SymTabGetSymbol(sym_tab_t *sym_tab, const char *label)
{
    it_t table_begun_iter = NULL;
    it_t table_end_iter = NULL;
    it_t found_symbol_iter = NULL;

    assert(sym_tab);
    assert(label);

    table_begun_iter = DLLBegin(sym_tab->symbol_table);
    table_end_iter = DLLEnd(sym_tab->symbol_table);
    found_symbol_iter = DLLFind(table_begun_iter, table_end_iter, 
    &CmpSymbolByLabel,label, NULL);
    
    /* Found Symbol */
    if (!DLLIsSameIter(table_end_iter, found_symbol_iter))
    {
        return DLLGetData(found_symbol_iter);
    }
    else 
    {
        return NULL;
    }
}

int SymTabIsData(const sym_tab_t *sym_tab, const symbol_t *symbol)
{
    int ret = FALSE;

    assert(sym_tab);
    assert(symbol);

    if ((ret = symbol->is_data))
    {
        assert(!symbol->is_entry);
        assert(!symbol->is_code);
        assert(NULL != symbol->data_vector);
    }

    return ret;
}

int SymTabIsCode(const sym_tab_t *sym_tab, const symbol_t *symbol)
{
    int ret = FALSE;

    assert(sym_tab);
    assert(symbol);
    
    if ((ret = symbol->is_code))
    {
        assert(!symbol->is_extern);
        assert(!symbol->is_data);
        assert(NULL == symbol->data_vector);
    }

    return ret;
}

int SymTabIsExtern(const sym_tab_t *sym_tab, const symbol_t *symbol)
{
    int ret = FALSE;

    assert(sym_tab);
    assert(symbol);
    
    if ((ret = symbol->is_extern))
    {
        assert(!symbol->is_entry);
        assert(!symbol->is_code);
        assert(!symbol->is_data);
        assert(NULL == symbol->data_vector);
    }

    return ret;
}

static void DestroySymbol(sym_tab_t *sym_tab, symbol_t *symbol)
{
    it_t table_begun_iter = NULL;
    it_t table_end_iter = NULL;
    it_t found_symbol_iter = NULL;

    assert(sym_tab);
    assert(symbol);

    table_begun_iter = DLLBegin(sym_tab->symbol_table);
    table_end_iter = DLLEnd(sym_tab->symbol_table);
    found_symbol_iter = DLLFind(table_begun_iter, table_end_iter, &CmpSymbolByLabel,symbol->label, NULL);
    
    /* if symbol has been pushed to table, pop it */
    if (!DLLIsSameIter(table_end_iter, found_symbol_iter))
    {
        DLLErase(found_symbol_iter);
    }

    if (symbol->data_vector)
    {
        DVECDestroy(symbol->data_vector);
        symbol->data_vector = NULL;
    }

    if (symbol->ic_referenced)
    {
        DVECDestroy(symbol->ic_referenced);
        symbol->ic_referenced = NULL;
    }

    free(symbol);
    symbol = NULL;
}

symbol_t *SymTabAddSymbol(sym_tab_t *sym_tab, const char label[])
{
    symbol_t *new_symbol = NULL;
    it_t dll_iter = NULL;
    it_t bad_iter = NULL;

    assert(sym_tab);
    assert(label);
    assert(!SymTabHasSymbol(sym_tab, label));

    new_symbol = malloc(sizeof(*new_symbol));
    {
        if (!new_symbol)
        {
            fprintf(stderr, "MEMORY ERROR: could not allocate memory while running line %d in %s/n", 
            __LINE__, __FILE__);

            return NULL;
        }
    }

    bad_iter = DLLEnd(sym_tab->symbol_table);
    strcpy ((char *)(new_symbol->dc_declared), label);
    new_symbol->is_entry = FALSE;
    new_symbol->is_extern = FALSE;

    new_symbol->is_code = FALSE;
    new_symbol->is_data = FALSE;

    new_symbol->dc_declared = -1;
    new_symbol->ic_declared = -1;

    new_symbol->ic_referenced = NULL; /* will be set if symbol declared extern */

    new_symbol->data_type = VOID;
    new_symbol->data_vector = NULL;

    dll_iter = DLLPushBack(sym_tab->symbol_table, new_symbol);
    if (DLLIsSameIter(dll_iter, bad_iter))
    {
        
        fprintf(stderr, "MEMORY ERROR: could not allocate memory while running line %d in %s/n", 
        __LINE__, __FILE__);

        return NULL;
    }

    return new_symbol;
}

static size_t SizeOfDataType(enum data_type data_type)
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

    assert(FALSE);
    return -1;
}

int SymTabSetDataVector(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type)
{
    /* memory error if cannot allocate memory
    */
    assert(sym_tab);
    assert(symbol); 
    assert(!symbol->is_code);
    assert(!symbol->is_entry);
    assert(VOID != data_type);

    /* Should not be set twice */
    assert(NULL == symbol->data_vector);
    assert(!symbol->is_data);

    symbol->is_data = TRUE;
    symbol->data_type = data_type;

    symbol->data_vector = DVECCreate(SizeOfDataType(data_type), MAX_LINE_LEN / 2);

    if (NULL == symbol->data_vector)
    {
         fprintf(stderr, "MEMORY ERROR: could not allocate memory while running line %d in %s/n", 
        __LINE__, __FILE__);

        return 1;
    }

    return 0;
}




/*
void SymTabMemCpy(sym_tab_t *sym_tab, symbol_t *symbol, void *to)
{
    return;
}
*/

/*
return status 
0 - success;
*/

/* typedef int (*act_f)(void *data, const void *params); */
static int UtilDestroySymbol(void *data,const void *params)
{
    symbol_t *symbol = (symbol_t *)data;

    assert(data);
    assert(NULL == params);

    (void)params;

    if (symbol->data_vector)
    {
        DVECDestroy(symbol->data_vector);
        symbol->data_vector = NULL;
    }

    if (symbol->ic_referenced)
    {
        DVECDestroy(symbol->ic_referenced);
        symbol->ic_referenced = NULL;
    }

    free(symbol);
    symbol = NULL;

    return 0;
}


void SymTabDestroy(sym_tab_t *sym_tab)
{
    assert(sym_tab);
    assert(sym_tab->symbol_table);

    DLLForEach(DLLBegin(sym_tab->symbol_table),DLLEnd(sym_tab->symbol_table), 
    &UtilDestroySymbol, NULL);
    free(sym_tab);
    sym_tab = NULL;    
}
