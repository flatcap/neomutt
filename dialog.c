#include "config.h"
#include <stdbool.h>
#include <unistd.h>
#include "dialog.h"
#include "curs_lib.h"
#include "mutt_curses.h"
#include "mutt_window.h"

static void dialog_push(struct Dialog *dlg)
{
  if (!dlg || !MuttDialogWindow)
    return;

  struct MuttWindow *last = TAILQ_LAST(&MuttDialogWindow->children, MuttWindowList);
  if (last)
    last->state.visible = false; //XXX needs to send notification. WHEN? at reflow!

  TAILQ_INSERT_TAIL(&MuttDialogWindow->children, dlg->root, entries);
  dlg->root->state.visible = true;
  dlg->root->need_repaint = true;
}

static void dialog_pop(void)
{
  if (!MuttDialogWindow)
    return;

  struct MuttWindow *last = TAILQ_LAST(&MuttDialogWindow->children, MuttWindowList);
  if (!last) // log?
    return;

  last->state.visible = false;
  TAILQ_REMOVE(&MuttDialogWindow->children, last, entries);

  last = TAILQ_LAST(&MuttDialogWindow->children, MuttWindowList);
  if (last)
    last->state.visible = true; //XXX needs to send notification
}

void window_repaint(struct Dialog *dlg, struct MuttWindow *win, bool force)
{
  if (!dlg || !win)
    return;

  if (win->repaint)
  {
    mutt_debug(LL_DEBUG1, "dump: repaint %s [%d,%d] (%d,%d)\n", win->name,
               win->state.row_offset, win->state.col_offset, win->state.rows,
               win->state.cols);

    WindowChangeFlags flags = force ? WIN_CHANGE_FORCE : WIN_CHANGE_NO_FLAGS;
    flags |= mutt_window_calc_changes(win);
    win->repaint(dlg, win, flags);
  }

  struct MuttWindow *np = NULL;
  TAILQ_FOREACH(np, &win->children, entries)
  {
    if (!np->state.visible)
      continue;
    if (!np->need_repaint && !force)
      continue;

    if (np->repaint)
    {
      mutt_debug(LL_DEBUG1, "dump: repaint %s [%d,%d] (%d,%d)\n", np->name,
                 np->state.row_offset, np->state.col_offset, np->state.rows,
                 np->state.cols);

      WindowChangeFlags flags = force ? WIN_CHANGE_FORCE : WIN_CHANGE_NO_FLAGS;
      flags |= mutt_window_calc_changes(np);
      np->repaint(dlg, np, flags);
    }
  }
}

void dialog_repaint(struct Dialog *dlg, bool force)
{
  window_repaint(dlg, dlg->root, force);
  refresh();
}

bool find_handler(struct Dialog *dlg, struct MuttWindow *win,
                  struct MuttWindow *except, struct KeyEvent *ch)
{
  if (!win)
    return false;

  struct MuttWindow *np = NULL;
  TAILQ_FOREACH(np, &win->children, entries)
  {
    if (np == except)
      continue;

    // Depth-first search
    if (find_handler(dlg, np, NULL, ch))
      return true;
  }

  if (win->handler && (win->handler(dlg, win, ch)))
    return true;

  if (except && find_handler(dlg, win->parent, win, ch))
    return true;

  return false;
}

int dialog_run(struct Dialog *dlg)
{
  if (!dlg)
    return -1;

  dialog_push(dlg);

  mutt_window_reflow(MuttDialogWindow);
  dialog_repaint(dlg, true);

  struct KeyEvent ch;
  while (true) // main loop
  {
    mutt_window_reflow(NULL);
    dialog_repaint(dlg, true);

    ch = mutt_getch();
    if (ch.ch == -2)
    {
      // WINCH?
      mutt_resize_screen();
      MuttHelpWindow->repaint(NULL, MuttHelpWindow, true);
      MuttMessageWindow->repaint(NULL, MuttMessageWindow, true);
      dialog_repaint(dlg, true);
    }

    if (find_handler(dlg, dlg->focus, NULL, &ch))
      continue;

    // OP_REDRAW

    if (ch.ch == 'q')
      break;
  }

  dialog_pop();

  return 0;
}
