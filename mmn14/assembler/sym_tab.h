#ifndef SYM_TAB_H
#define SYM_TAB_H

#include "mmn14_types.h"

typedef struct sym_tab sym_tab_t;
typedef struct symbol symbol_t;


sym_tab_t *SymTabCreate();

int SymTabHasSymbol(const sym_tab_t *sym_tab, const char *label);
symbol_t *SymTabGetSymbol(sym_tab_t *sym_tab, const char *label);

int SymTabIsData(const sym_tab_t *sym_tab, const symbol_t *symbol);
int SymTabIsCode(const sym_tab_t *sym_tab, const symbol_t *symbol);
int SymTabIsExtern(const sym_tab_t *sym_tab, const symbol_t *symbol);


int SymTabSetDataVector(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type);

int SymTabAddDataToDataVector(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type, const void *data);


symbol_t *SymTabAddSymbol(sym_tab_t *sym_tab, const char label[]);
/*
void SymTabMemCpy(sym_tab_t *sym_tab, symbol_t *symbol, void *to);
*/

/*
return status 
0 - success;
int SymTabSetEntryRelativeAddress(sym_tab_t *sym_tab, symbol_t *symbol, void *zeroth_address, void *to);
*/


void SymTabDestroy(sym_tab_t *sym_tab);

#endif /* SYM_TAB */
