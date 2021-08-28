#include <stdlib.h> /* malloc() */
#include <assert.h> /* assert() */
#include <string.h> /* strcpy() */
#include <stdio.h> /* fprintf() */

#include "dll.h"
#include "dvec.h"

#include "sym_tab.h"

typedef enum {FALSE, TRUE} bool;
enum {OK = 0, ERROR};

/* the assumption is that the declaration is in order, to kkep the data count.
So do not alter the data of n'th symbol 
after the (n + k)'th symbol has been declared */
struct sym_tab
{
    dll_t *symbol_table;
    unsigned long dc;
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
    unsigned long dc_declared;

    /* for code. If data, default to -1 */
    unsigned long ic_declared;

    /* fill in second pass. For extern only. */
    dvec_t *arr_instr_referencing;
  
    /* for data only */
    enum data_type data_type;
    dvec_t *data_vector; /* array of words or array of half words or array of bytes */
};

/************ static function declaration *********/
static size_t SizeOfDataType(enum data_type data_type);
static int UtilDestroySymbol(void *data,const void *params);



static int UtilIsFoundSymbolByLabel(const void *compared_symbol, const void *given_label, const void *void_param);
static int UtilDestroySymbol(void *data,const void *params);


/*********************/

sym_tab_t *SymTabCreate()
{
    sym_tab_t *new_sym_tab = malloc(sizeof(*new_sym_tab));
    if (!new_sym_tab)
    {
        fprintf(stderr, "MEMORY ERROR: could not allocate memory while running line %d in %s/n", 
        __LINE__, __FILE__);

        return NULL;
    }

    new_sym_tab->symbol_table = DLLCreate();
    if (!new_sym_tab->symbol_table)
    {   
        free(new_sym_tab);
        new_sym_tab = NULL;
        fprintf(stderr, "MEMORY ERROR: could not allocate memory while running line %d in %s/n", 
        __LINE__, __FILE__);

        return NULL;
    }

    new_sym_tab->dc = 0;

    return new_sym_tab;
}

/* typedef int (*cmp_f)(const void *data, const void *key, const void *params); */
/* If same label - return 0 */
static int UtilIsFoundSymbolByLabel(const void *compared_symbol, const void *given_label, 
const void *void_param)
{
    assert(compared_symbol);
    assert(given_label);
    assert(!void_param);

    assert(((symbol_t *)compared_symbol)->label);

    (void)void_param;
   
    return !strcmp(((symbol_t *)compared_symbol)->label, (const char *)given_label);
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
        assert(0);
        break;
    default:
        assert(0);
        break;
    }

    return -1lu;
}


int SymTabHasSymbol(const sym_tab_t *sym_tab, const char *label)
{
    assert(sym_tab);
    assert(label);    
    
    return NULL != SymTabGetSymbol((sym_tab_t *)sym_tab, label);
}

unsigned long SymTabGetICDeclared(const sym_tab_t *sym_tab, const symbol_t *symbol)
{
    assert(sym_tab);
    assert(symbol);

    assert(symbol->is_code);
    assert(-1 != symbol->ic_declared);

    return symbol->ic_declared;
}

int SymTabSymbolAddReferingInstr(sym_tab_t *sym_tab, symbol_t *symbol, size_t instr_addr)
{
    assert(sym_tab);
    assert(symbol);
    assert(symbol->is_extern);
    assert(symbol->arr_instr_referencing);

    return DVECPushBack(symbol->arr_instr_referencing, &instr_addr);
}

unsigned long SymTabDataSymbolRelativeAddress(const sym_tab_t *sym_tab, const symbol_t *symbol)
{
    assert(sym_tab);
    assert(symbol);
    assert(symbol->is_data);

    return symbol->dc_declared;
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
    &UtilIsFoundSymbolByLabel,label, NULL);
    
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

int SymTabSymbolIsData(const sym_tab_t *sym_tab, const symbol_t *symbol)
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

int SymTabSymbolIsCode(const sym_tab_t *sym_tab, const symbol_t *symbol)
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

int SymTabSymbolIsExtern(const sym_tab_t *sym_tab, const symbol_t *symbol)
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

int SymTabSymbolIsEntry(const sym_tab_t *sym_tab, const symbol_t *symbol)
{
    assert(FALSE);
    return FALSE;
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
    if (!new_symbol)
    {
        fprintf(stderr, "MEMORY ERROR: could not allocate memory while running line %d in %s/n", 
        __LINE__, __FILE__);

        return NULL;
    }
    

    bad_iter = DLLEnd(sym_tab->symbol_table);
    strcpy ((char *)(new_symbol->label), label);
    new_symbol->is_entry = FALSE;
    new_symbol->is_extern = FALSE;

    new_symbol->is_code = FALSE;
    new_symbol->is_data = FALSE;

    new_symbol->dc_declared = -1;
    new_symbol->ic_declared = -1;

    new_symbol->arr_instr_referencing = NULL; /* will be set if symbol declared extern */

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

int SymTabSymbolSetData(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type)
{
    /* memory error if cannot allocate memory
    */
    assert(sym_tab);
    assert(symbol); 
    assert(!symbol->is_code);
    assert(!symbol->is_entry);
    assert(VOID != data_type);
    assert(!symbol->is_extern);
    assert(NULL == symbol->arr_instr_referencing);



    /* Should not be set twice */
    assert(NULL == symbol->data_vector);
    assert(!symbol->is_data);

    symbol->is_data = TRUE;
    symbol->data_type = data_type;
    symbol->dc_declared = sym_tab->dc;

    symbol->data_vector = DVECCreate(SizeOfDataType(data_type), MAX_LINE_LEN / 2);

    if (NULL == symbol->data_vector)
    {
        fprintf(stderr, "MEMORY ERROR: could not allocate memory while running line %d in %s/n", 
        __LINE__, __FILE__);

        return ERROR;
    }

    return OK;
}

int SymTabSymbolSetCode(sym_tab_t *sym_tab, symbol_t *symbol, size_t instruction_index)
{
    assert(sym_tab);
    assert(symbol);
    assert(!symbol->is_entry);
    assert(VOID == symbol->data_type);
    assert(NULL == symbol->data_vector);
    assert(NULL == symbol->arr_instr_referencing);
    assert(!symbol->is_data);
    assert(!symbol->is_extern);
    /* Should not be set twice */
    assert(!symbol->is_code);

    symbol->is_code = TRUE;
    symbol->ic_declared = instruction_index;

    return OK;
}

int SymTabSymbolSetEntry(sym_tab_t *sym_tab, symbol_t *symbol)
{
    assert(sym_tab);
    assert(symbol);

    assert(!symbol->is_extern);
    assert(NULL == symbol->arr_instr_referencing);

    return OK;
}

int SymTabSymbolSetExtern(sym_tab_t *sym_tab, symbol_t *symbol)
{
    assert(sym_tab);
    assert(symbol);
    assert(!symbol->is_entry);
    assert(VOID == symbol->data_type);
    assert(NULL == symbol->data_vector);
    assert(!symbol->is_data);
    assert(!symbol->is_code);

    /* Should not be set twice */
    assert(!symbol->is_extern);
    assert(NULL == symbol->arr_instr_referencing);

    symbol->is_extern = TRUE;
    symbol->arr_instr_referencing = DVECCreate(sizeof(size_t), 0);

    return NULL == symbol->arr_instr_referencing ? !OK : OK;
}

int SymTabSymbolAddDataToDataVector(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type, const void *data)
{
    size_t size_of_element = -1;
    int status = OK;

    /* push to symbol and promote sym_ytab dc */
    assert(sym_tab);
    assert(symbol);
    assert(data);
    assert(symbol->data_vector);
    assert(data_type == symbol->data_type);

    size_of_element = SizeOfDataType(data_type);

    status = DVECPushBack(symbol->data_vector, data);
    /* enough memory has been allocated in SymTabSetDataVector()  */
    assert(OK == status);

    sym_tab->dc += size_of_element;

    return status;
}

enum data_type SymTabSymbolGetDataType(sym_tab_t *sym_tab, symbol_t *symbol)
{
    (void)sym_tab;

    assert(symbol);
    assert(symbol->is_data);

    return symbol->data_type;
}



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

    if (symbol->arr_instr_referencing)
    {
        DVECDestroy(symbol->arr_instr_referencing);
        symbol->arr_instr_referencing = NULL;
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

/*

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
    
    "if symbol has been pushed to table, pop it";
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

*/
