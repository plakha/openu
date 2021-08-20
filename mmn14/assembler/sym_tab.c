#include <stdlib.h>
#include <assert.h>

#include "dll.h"
#include "dvec.h"

#include "sym_tab.h"


typedef enum {FALSE, TRUE} bool;



struct sym_tab
{
    dll_t *symbol_table;
    size_t dc;
    size_t code_start_address; /* data_segment is supposed to be right after text (code segment),
    so absolute_start_address = inst_count * sizeof(word) + code_start_address.
    Calling this absolute start address, because relative start address is 0. Absolute counts from the beginning of RAM  */
};


struct symbol
{
    char label[32];
  

    bool is_entry; /* default FALSE,  */
    bool is_extern; /* default FALSE */

    /* for code. if not code, default to -1 */
    size_t ic;

  
/* for data only */
    dvec_t *num_data_vector; /* array of words or array of half words or array of bytes */
};



sym_tab_t *SymTabCreate()
{
    sym_tab_t *new_sym_tab = malloc(sizeof(*new_sym_tab));

    return new_sym_tab;
}

int HasSymbol(sym_tab_t *sym_tab, const char *label)
{
    /* DLLFind, strcmp */
    return 1;
}

symbol_t *SymTabGetSymbol(sym_tab_t *sym_tab, const char *label)
{
        /* DLLFind, strcmp */

    return NULL;
}

void SymTabPushNumData(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type, long *num_arr)
{
    return;
}

void SymTabPushStringData(sym_tab_t *sym_tab, symbol_t *symbol, const char *string)
{
    return;
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


void SymTabDestroy(sym_tab_t *sym_tab)
{
    assert(sym_tab);

    free(sym_tab);
    sym_tab = NULL;    
}
