#include "config.h"
#include <string.h>
#include "mutt/mutt.h"
#include "dlg_one.h"
#include "keymap.h"
#include "mutt_curses.h"
#include "mutt_window.h"
#include "opcodes.h"

static int dlg_one_menu_repaint(struct Dialog *dlg, struct MuttWindow *win, WindowChangeFlags flags)
{
  if (!dlg || !win)
    return -1;

  struct DlgOne *d1 = (struct DlgOne *) dlg;

  if (d1->number >= win->state.rows)
    d1->number = win->state.rows - 1;

  for (int i = 0; i < win->state.rows; i++)
  {
    if (i == d1->number)
      mutt_curses_set_color(MT_COLOR_INDICATOR);
    else
      mutt_curses_set_color(MT_COLOR_NORMAL);

    if (i == 0)
      mutt_window_mvprintw(win, i, 0, "%d FIRST ROW", i);
    else if (i == (win->state.rows - 1))
      mutt_window_mvprintw(win, i, 0, "%d LAST ROW", i);
    else
      mutt_window_mvprintw(win, i, 0, "%d %s", i, d1->string);
    mutt_window_clrtoeol(win);
  }

  return 0;
}

static bool dlg_one_menu_handler(struct Dialog *dlg, struct MuttWindow *win,
                                 struct KeyEvent *ke)
{
  struct DlgOne *d1 = (struct DlgOne *) dlg;

  // switch (ke->op)
  switch (ke->ch)
  {
    case 'j': // OP_NEXT_ENTRY:
    {
      if (d1->number < (win->state.rows - 1))
      {
        d1->number++;
        win->need_repaint = true;
        return true;
      }
      break;
    }

    case 'k': // OP_PREV_ENTRY:
    {
      if (d1->number > 0)
      {
        d1->number--;
        win->need_repaint = true;
        return true;
      }
      break;
    }
  }

  return false;
}

static int dlg_one_bar_repaint(struct Dialog *dlg, struct MuttWindow *win, WindowChangeFlags flags)
{
  if (!dlg || !win)
    return -1;

  struct DlgOne *d1 = (struct DlgOne *) dlg;

  char buf[1024];
  memset(buf, '-', sizeof(buf));

  int start = snprintf(buf, sizeof(buf), "<-- %s ", d1->string);
  buf[start] = '-';

  int width = win->state.cols - 1;
  snprintf(buf + width, sizeof(buf) - width, ">");

  mutt_curses_set_color(MT_COLOR_STATUS);
  mutt_window_mvprintw(win, 0, 0, buf);
  mutt_window_clrtoeol(MuttMessageWindow);
  mutt_curses_set_color(MT_COLOR_NORMAL);

  return 0;
}

static void create_windows(struct Dialog *dlg)
{
  struct MuttWindow *root =
      mutt_window_new(MUTT_WIN_ORIENT_VERTICAL, MUTT_WIN_SIZE_MAXIMISE,
                      MUTT_WIN_SIZE_UNLIMITED, MUTT_WIN_SIZE_UNLIMITED);
  struct MuttWindow *menu =
      mutt_window_new(MUTT_WIN_ORIENT_VERTICAL, MUTT_WIN_SIZE_MAXIMISE,
                      MUTT_WIN_SIZE_UNLIMITED, MUTT_WIN_SIZE_UNLIMITED);
  struct MuttWindow *bar = mutt_window_new(MUTT_WIN_ORIENT_VERTICAL, MUTT_WIN_SIZE_FIXED,
                                           1, MUTT_WIN_SIZE_UNLIMITED);

  menu->repaint = dlg_one_menu_repaint;
  menu->handler = dlg_one_menu_handler;
  bar->repaint = dlg_one_bar_repaint; // bar->handler is NULL, window is passive

  root->name = "Dialog One";
  menu->name = "D1 Menu";
  bar->name = "D1 Bar";

  mutt_window_add_child(root, menu);
  mutt_window_add_child(root, bar);

  dlg->root = root;
  dlg->focus = menu;
}

void dlg_one_free(struct DlgOne **ptr)
{
  if (!ptr || !*ptr)
    return;

  struct DlgOne *dlg = *ptr;

  mutt_window_free(&dlg->dialog.root);
  FREE(&dlg->string);
  FREE(ptr);
}

struct DlgOne *dlg_one_new(void)
{
  struct DlgOne *dlg = mutt_mem_calloc(1, sizeof(struct DlgOne));

  create_windows(&dlg->dialog);

  // Payload
  dlg->number = 5;
  dlg->string = mutt_str_strdup("hello world");

  return dlg;
}
