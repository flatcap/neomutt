#include "config.h"
#include <string.h>
#include "mutt/mutt.h"
#include "dlg_two.h"
#include "keymap.h"
#include "mutt_curses.h"
#include "mutt_window.h"
#include "opcodes.h"

static int dlg_two_page_repaint(struct Dialog *dlg, struct MuttWindow *win, WindowChangeFlags flags)
{
  if (!dlg || !win)
    return -1;

  // struct DlgTwo *d2 = (struct DlgTwo *) dlg;

  char buf[1024];
  memset(buf, ' ', sizeof(buf));
  buf[win->state.cols] = 0;

  for (int i = 0; i < win->state.rows; i++)
  {
    mutt_window_mvprintw(win, i, 0, buf);
  }

  mutt_curses_set_color(MT_COLOR_INDICATOR);
  mutt_window_mvprintw(win, 0, 0, "NW");
  mutt_window_mvprintw(win, win->state.rows - 1, 0, "SW");
  mutt_window_mvprintw(win, 0, win->state.cols - 2, "NE");
  mutt_window_mvprintw(win, win->state.rows - 1, win->state.cols - 2, "SE");
  mutt_curses_set_color(MT_COLOR_NORMAL);

  return 0;
}

static bool dlg_two_page_handler(struct Dialog *dlg, struct MuttWindow *win,
                                 struct KeyEvent *ke)
{
  struct DlgTwo *d2 = (struct DlgTwo *) dlg;

  struct MuttWindow *side = TAILQ_FIRST(&d2->dialog.root->children);

  switch (ke->ch)
  {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    {
      d2->filler = ke->ch;
      win->need_repaint = true;
      return true;
    }

    case 'l':
    {
      d2->width += 5;
      side->req_cols = d2->width;
      side->need_repaint = true;
      mutt_window_reflow(d2->dialog.root);
      return true;
    }

    case 'h':
    {
      if (d2->width > 5)
      {
        d2->width -= 5;
        side->req_cols = d2->width;
        side->need_repaint = true;
        mutt_window_reflow(d2->dialog.root);
      }
      return true;
    }
  }

  return false;
}

static int dlg_two_side_repaint(struct Dialog *dlg, struct MuttWindow *win, WindowChangeFlags flags)
{
  if (!dlg || !win)
    return -1;

  struct DlgTwo *d2 = (struct DlgTwo *) dlg;

  char buf[1024] = { 0 };
  memset(buf, d2->filler, d2->width);
  buf[0] = '<';
  buf[d2->width - 1] = '>';

  for (int i = 0; i < win->state.rows; i++)
  {
    mutt_window_mvprintw(win, i, 0, buf);
  }

  mutt_window_mvprintw(win, 0, 2, "[TOP]");
  mutt_window_mvprintw(win, win->state.rows - 1, 2, "[BOT]");

  return 0;
}

static void create_windows(struct DlgTwo *d2)
{
  struct MuttWindow *root =
      mutt_window_new(MUTT_WIN_ORIENT_HORIZONTAL, MUTT_WIN_SIZE_MAXIMISE,
                      MUTT_WIN_SIZE_UNLIMITED, MUTT_WIN_SIZE_UNLIMITED);
  struct MuttWindow *side = mutt_window_new(MUTT_WIN_ORIENT_VERTICAL, MUTT_WIN_SIZE_FIXED,
                                            MUTT_WIN_SIZE_UNLIMITED, d2->width);
  struct MuttWindow *page =
      mutt_window_new(MUTT_WIN_ORIENT_VERTICAL, MUTT_WIN_SIZE_MAXIMISE,
                      MUTT_WIN_SIZE_UNLIMITED, MUTT_WIN_SIZE_UNLIMITED);

  side->repaint = dlg_two_side_repaint; // size->handler is NULL, window is passive
  page->repaint = dlg_two_page_repaint;
  page->handler = dlg_two_page_handler;

  root->name = "Dialog Two";
  side->name = "D2 Side";
  page->name = "D2 Page";

  mutt_window_add_child(root, side);
  mutt_window_add_child(root, page);

  d2->dialog.root = root;
  d2->dialog.focus = page;
}

void dlg_two_free(struct DlgTwo **ptr)
{
  if (!ptr || !*ptr)
    return;

  struct DlgTwo *d2 = *ptr;

  mutt_window_free(&d2->dialog.root);
  FREE(ptr);
}

struct DlgTwo *dlg_two_new(void)
{
  struct DlgTwo *dlg = mutt_mem_calloc(1, sizeof(struct DlgTwo));

  // Payload
  dlg->width = 30;
  dlg->filler = '.';

  create_windows(dlg);

  return dlg;
}
