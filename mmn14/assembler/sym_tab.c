#include <stdlib.h>

#include "dll.h"
#include "dvec.h"

#include "sym_tab.h"


typedef enum {FALSE, TRUE} bool;



struct sym_tab
{
    dll_t *symbol_table;
    size_t code_start_address; /* data_segment is supposed to be right after text (code segment),
    so absolute_start_address = inst_count * sizeof(word) + code_start_address.
    Calling this absolute start address, because relative start address is 0. Absolute counts from the beginning of RAM  */
};


struct symbol
{
    char label[32];
    bool is_code;
    bool is_data;

    bool is_entry;
    bool is_extern;

    size_t define_address; /*entry - where defined: address = code_start_address - to_address[from MemCpy]
    keep anyway! print out only for entry
    default = 0*/
    
    dll_t *referred_addresses; /*where referred to. will keep it anyway! will print out only for extern 
    default: null (pro = less space) or empty dll (pro - less if checks)?*/

    size_t data_n_members;
    enum data_type data_type;

    dvec_t *num_data_vector; /* single STRING[80] or array of words or array of half words or array of bytes */
    char str[80];
};



sym_tab_t *SymTabCreate()
{
    return NULL;
}

int HasSymbol(sym_tab_t *sym_tab, const char *label)
{
    return 1;
}

symbol_t *SymTabGetSymbol(sym_tab_t *sym_tab, const char *label)
{
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


void SymTabMemCpy(sym_tab_t *sym_tab, symbol_t *symbol, void *to)
{
    return
}

/*
return status 
0 - success;
*/
int SymTabSetEntryRelativeAddress(sym_tab_t *sym_tab, symbol_t *symbol, void *zeroth_address, void *to)
{
    return 0;
}


void SymTabDestroy(sym_tab_t *sym_tab)
{
    return;
}
