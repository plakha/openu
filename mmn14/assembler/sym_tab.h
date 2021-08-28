#ifndef SYM_TAB_H
#define SYM_TAB_H

#include "mmn14_types.h"

typedef struct sym_tab sym_tab_t;
typedef struct symbol symbol_t;


sym_tab_t *SymTabCreate();

int SymTabHasSymbol(const sym_tab_t *sym_tab, const char *label);
symbol_t *SymTabGetSymbol(sym_tab_t *sym_tab, const char *label);

int SymTabSymbolIsData(const sym_tab_t *sym_tab, const symbol_t *symbol);
int SymTabSymbolIsCode(const sym_tab_t *sym_tab, const symbol_t *symbol);
int SymTabSymbolIsExtern(const sym_tab_t *sym_tab, const symbol_t *symbol);
int SymTabSymbolIsEntry(const sym_tab_t *sym_tab, const symbol_t *symbol);



int SymTabSymbolSetData(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type);
int SymTabSymbolSetCode(sym_tab_t *sym_tab, symbol_t *symbol, size_t instruction_index);
int SymTabSymbolSetEntry(sym_tab_t *sym_tab, symbol_t *symbol);
int SymTabSymbolSetExtern(sym_tab_t *sym_tab, symbol_t *symbol);

size_t SymTabGetICDeclared(const sym_tab_t *sym_tab, const symbol_t *symbol);

int SymTabSymbolAddReferingInstr(sym_tab_t *sym_tab, symbol_t *symbol, size_t instr_addr);


int SymTabSymbolAddDataToDataVector(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type, const void *data);

size_t SymTabDataSymbolRelativeAddress(const sym_tab_t *sym_tab, const symbol_t *symbol);


symbol_t *SymTabAddSymbol(sym_tab_t *sym_tab, const char label[]);
/*
void SymTabMemCpy(sym_tab_t *sym_tab, symbol_t *symbol, void *to);
*/



void SymTabDestroy(sym_tab_t *sym_tab);

#endif /* SYM_TAB */
