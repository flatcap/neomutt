struct ConfigDef
{
  const char *name; // "askbcc",      old name
  uint32_t type;    // DT_SYNONYM,    synonym
  intptr_t initial; // IP "ask_bcc",  new name
  intptr_t data;    // "2021-07-01",  since release (1625137200)
  int (*validator)();
  const char *docs;
  intptr_t var;
};

{ "askbcc", DT_SYNONYM, IP "ask_bcc", },

