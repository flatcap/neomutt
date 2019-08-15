#ifndef MUTT_DIALOG_H
#define MUTT_DIALOG_H

struct MuttWindow;

struct Dialog
{
  struct MuttWindow *root;
  struct MuttWindow *focus;
};

int dialog_run(struct Dialog *dlg);

#endif /* MUTT_DIALOG_H */
