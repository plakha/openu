#ifndef SYM_TAB_H
#define SYM_TAB_H

/*********************************
 * 
 * Symbol table implementation for assembler project (mmn14) Summer 2021
 * 
 * Please pass valid arguments.
 * Check the input according to the header file.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * *****************************************/


#include "mmn14_types.h"
#include "dll.h" /* act_f */

typedef struct sym_tab sym_tab_t;
typedef struct symbol symbol_t;


/* returns pointer to an instance of a symbol table (sym_tab_t) */
sym_tab_t *SymTabCreate();

/* receives symbol table syn_tab, label
return boolean value: whether such symbol exists or not.
Pass only legally acquired instance of sym_tab.
Pass valid string as label.
 */
int SymTabHasSymbol(const sym_tab_t *sym_tab, const char *label);

/*
receives symbol table sym_tab, label
return pointer to the symbol under such label. If none exists - returns NULL
Pass only legally acquired instance of sym_tab.
Pass valid string as label.
*/
symbol_t *SymTabGetSymbol(sym_tab_t *sym_tab, const char *label);


/* receives pointer to symbol table sym_tab, pointer to symbol,
returns boolean - whether the symbol refers to Data segment address */
int SymTabSymbolIsData(const sym_tab_t *sym_tab, const symbol_t *symbol);


/* receives pointer to symbol table sym_tab, pointer to symbol,
returns boolean - whether the symbol refers to Code segment address */
int SymTabSymbolIsCode(const sym_tab_t *sym_tab, const symbol_t *symbol);


/* receives pointer to symbol table sym_tab, pointer to symbol,
returns boolean - whether the symbol is extern 
this means the symbol cannot be entry.
It should not be either data or code*/
int SymTabSymbolIsExtern(const sym_tab_t *sym_tab, const symbol_t *symbol);


/* receives pointer to symbol table sym_tab, pointer to symbol,
returns boolean - whether the symbol is entry
this means that the symbol cannot be extern */
int SymTabSymbolIsEntry(const sym_tab_t *sym_tab, const symbol_t *symbol);


/* receives pointer to symbol table sym_tab, pointer to symbol, data type
sets the symbol data.
The symbol should not be code. should not be extern!
The function may not be idempotent. Do not use twice!

returns non-zero status if could not allocate memory to hold data.
returns 0 otherwise
*/
int SymTabSymbolSetData(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type);


/* receives pointer to symbol table sym_tab, pointer to symbol, the instruction index for the symbol.
sets the symbol code. keeps the instruction index 
(recommended to keep it in words, not in bytes, 
relative to the start of code segment , not the ram)

The symbol should not be data. should not be extern!
The function may not be idempotent. Do not use twice!
returns 0 status. non-zero maybe return in case of error
*/
int SymTabSymbolSetCode(sym_tab_t *sym_tab, symbol_t *symbol, size_t instruction_index);


/* receives pointer to symbol table sym_tab, pointer to symbol,
sets the symbol entry.

The symbol should not be extern!
The function may not be idempotent. Do not use twice!

returns 0 status. non-zero maybe return in case of error
*/
int SymTabSymbolSetEntry(sym_tab_t *sym_tab, symbol_t *symbol);

/* receives pointer to symbol table sym_tab, pointer to symbol,
sets the symbol extern.

The symbol should not be data or code, should not be entry!
The function may not be idempotent. Do not use twice!

returns 0 status. non-zero maybe return in case of error
*/
int SymTabSymbolSetExtern(sym_tab_t *sym_tab, symbol_t *symbol);

/* receives pointer to symbol table sym_tab, pointer to symbol,
returns the instruction index.

The symbol shoul be code!, shopuld not be extern!
*/
unsigned long SymTabGetICDeclared(const sym_tab_t *sym_tab, const symbol_t *symbol);

/* receives pointer to symbol table sym_tab
returns the data count in bytes

*/
unsigned long SymTabGetDataCount(const sym_tab_t *sym_tab);


/*
receives pointer to symbol table sym_tab, pointer to symbol,
returns the label. Altering the unreferenced return value is unsafe!
*/
const char *SymTabSymbolGetLabel(const sym_tab_t *sym_tab, const symbol_t *symbol);

/*
receives pointer to symbol table sym_tab, pointer to symbol,
returns the index, in bytes, of the data in this symbol

The symbol should be data! Should not be entry!
*/
unsigned long SymTabSymbolGetDC(const sym_tab_t *sym_tab, const symbol_t *symbol);

/*
receives pointer to symbol table sym_tab, pointer to symbol, receives instruction address
(recommended, in bytes). Keeps the instruction address value

The symbol should be extern only!

Returns non zero status if the value could not be pushed.
Returns 0 otherwise.

*/
int SymTabSymbolAddReferingInstr(sym_tab_t *sym_tab, symbol_t *symbol, size_t instr_addr);

/*
receives pointer to symbol table sym_tab, pointer to symbol, data type and pointer to data
. Copies the data and keeps it.

The symbol should be data!
The data type should be the same as passed in SymTabSymbolSetData()

Symbol cannot keep mixed data types.

Returns non zero status if the value could not be pushed.
Returns 0 otherwise.

*/
int SymTabSymbolAddDataToDataVector(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type, const void *data);

/*
    receives pointer to symbol table sym_tab, pointer to function of type act_f, that is
     (int (*)(void *data, const void *params),
    and the params for the function.

    applies the function act_f to each symbol, if the function returned non zero,
            stops and returns the non-zero value
*/
int SymTabForEachSymbol(sym_tab_t *sym_tab, act_f func, const void *params);

/*
    receives pointer to symbol table sym+tab, label.
    Creates symbol instance and returns iot to the xcaller.
    The caller is expected to set the symbol with the set functions in this library


    label is expected to be of MAX_LABEL_SIZE!
    the label is copied, so it is for the caller to further use the passed data
*/
symbol_t *SymTabAddSymbol(sym_tab_t *sym_tab, const char label[]);



/*
    receives the pointer to the symbol table and frees it, including all the symbols!
    The caller is expected to not use anyt symbols after this function has been called
*/
void SymTabDestroy(sym_tab_t *sym_tab);

#endif /* SYM_TAB */
