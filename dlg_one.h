#ifndef MUTT_DLG_ONE_H
#define MUTT_DLG_ONE_H

#include "dialog.h"

struct DlgOne
{
  struct Dialog dialog; // must be first

  // Payload
  int number;
  char *string;
};

void dlg_one_free(struct DlgOne **ptr);
struct DlgOne *dlg_one_new(void);

#endif /* MUTT_DLG_ONE_H */
