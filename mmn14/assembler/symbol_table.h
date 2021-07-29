typedef struct sym_tab sym_tab_t;
typedef struct symbol symbol_t;

enum data_type {VOID, BYTE, HALF_WORD, WORD, STRING};

sym_tab_t *SymTabCreate();

int HasSymbol(sym_tab_t *sym_tab, const char *label);
symbol_t *SymTabGetSymbol(sym_tab_t *sym_tab, const char *label);

void SymTabPushNumData(sym_tab_t *sym_tab, symbol_t *symbol, enum data_type data_type, long *num_arr);
void SymTabPushStringData(sym_tab_t *sym_tab, symbol_t *symbol, const char *string);


void SymTabMemCpy(sym_tab_t *sym_tab, symbol_t *symbol, void *to);

/*
return status 
0 - success;
*/
int SymTabSetEntryRelativeAddress(sym_tab_t *sym_tab, symbol_t *symbol, void *zeroth_address, void *to);


void SymTabDestroy(sym_tab_t *sym_tab);