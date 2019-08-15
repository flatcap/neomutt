#ifndef MUTT_DLG_TWO_H
#define MUTT_DLG_TWO_H

#include "dialog.h"

struct DlgTwo
{
  struct Dialog dialog; // must be first

  // Payload
  int width;
  char filler;
};

void dlg_two_free(struct DlgTwo **ptr);
struct DlgTwo *dlg_two_new(void);

#endif /* MUTT_DLG_TWO_H */
