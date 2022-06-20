/**
 * @file
 * GUI ask the user to enter a string
 *
 * @authors
 * Copyright (C) 1996-2000,2007,2011,2013 Michael R. Elkins <me@mutt.org>
 * Copyright (C) 2000-2001 Edmund Grimley Evans <edmundo@rano.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @page enter_window GUI ask the user to enter a string
 *
 * GUI ask the user to enter a string
 */

#include "config.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include "mutt/lib.h"
#include "core/lib.h"
#include "gui/lib.h"
#include "mutt.h"
#include "complete/lib.h"
#include "enter/lib.h"
#include "history/lib.h"
#include "menu/lib.h"
#include "color/color.h"
#include "debug.h"
#include "enter.h"
#include "functions.h"
#include "globals.h"
#include "keymap.h"
#include "muttlib.h"
#include "opcodes.h"
#include "wdata.h"

const char *km_keyname(int c);

/// Help Bar for the Command Line Editor
static const struct Mapping EditorHelp[] = {
  // clang-format off
  { N_("Complete"),    OP_EDITOR_COMPLETE },
  { N_("Hist Up"),     OP_EDITOR_HISTORY_UP },
  { N_("Hist Down"),   OP_EDITOR_HISTORY_DOWN },
  { N_("Hist Search"), OP_EDITOR_HISTORY_SEARCH },
  { N_("Begin Line"),  OP_EDITOR_BOL },
  { N_("End Line"),    OP_EDITOR_EOL },
  { N_("Kill Line"),   OP_EDITOR_KILL_LINE },
  { N_("Kill Word"),   OP_EDITOR_KILL_WORD },
  { NULL, 0 },
  // clang-format on
};

/**
 * my_addwch - Display one wide character on screen
 * @param win Window
 * @param wc  Character to display
 * @retval OK  Success
 * @retval ERR Failure
 */
static int my_addwch(struct MuttWindow *win, wchar_t wc)
{
  int n = wcwidth(wc);
  if (IsWPrint(wc) && (n > 0))
    return mutt_addwch(win, wc);
  if (!(wc & ~0x7f))
    return mutt_window_printf(win, "^%c", ((int) wc + 0x40) & 0x7f);
  if (!(wc & ~0xffff))
    return mutt_window_printf(win, "\\u%04x", (int) wc);
  return mutt_window_printf(win, "\\u%08x", (int) wc);
}

/**
 * enter_window_recalc - Recalculate the Enter Window - Implements MuttWindow::recalc() - @ingroup window_recalc
 */
static int enter_window_recalc(struct MuttWindow *win)
{
  if (!win || !win->wdata)
    return 0;

  // struct EnterWindowData *wdata = win->wdata;

  // if (SigWinch)
  // {
  //   SigWinch = false;
  //   mutt_resize_screen();
  //   clearok(stdscr, true);
  //   window_redraw(NULL);
  // }

  // if (wdata->state->wbuf)
  // {
  //   /* Coming back after return 1 */
  //   wdata->redraw = ENTER_REDRAW_LINE;
  //   wdata->flags &= ~MUTT_COMP_CLEAR;
  // }
  // else
  // {
  //   /* Initialise wbuf from buf */
  //   wdata->state->wbuflen = 0;
  //   wdata->state->lastchar = mutt_mb_mbstowcs(&state->wbuf, &state->wbuflen, 0, buf_string(wdata.buf));
  //   wdata->redraw = ENTER_REDRAW_INIT;
  //   wdata->state->curpos = state->lastchar;
  // }
  win->actions |= WA_REPAINT;
  mutt_debug(LL_DEBUG1, "recalc done, request WA_REPAINT\n");
  return 0;
}

/**
 * enter_window_repaint - Repaint the Enter Window - Implements MuttWindow::repaint() - @ingroup window_repaint
 */
static int enter_window_repaint(struct MuttWindow *win)
{
  if (!win || !win->wdata)
    return 0;

  struct EnterWindowData *wdata = win->wdata;
  struct EnterState *es = wdata->state;

  mutt_window_clearline(win, 0);
  mutt_curses_set_normal_backed_color_by_id(MT_COLOR_PROMPT);
  mutt_window_addstr(win, wdata->field);
  mutt_curses_set_color_by_id(MT_COLOR_NORMAL);
  mutt_refresh();
  mutt_window_get_coords(win, &wdata->col, NULL);

  if (!wdata->pass)
  {
    int width = win->state.cols - wdata->col - 1;

    if (wdata->redraw == ENTER_REDRAW_INIT)
    {
      /* Go to end of line */
      es->curpos = es->lastchar;
      wdata->begin = mutt_mb_width_ceiling(es->wbuf, es->lastchar,
                                           mutt_mb_wcswidth(es->wbuf, es->lastchar) -
                                               width + 1);
    }
    if ((es->curpos < wdata->begin) ||
        (mutt_mb_wcswidth(es->wbuf + wdata->begin, es->curpos - wdata->begin) >= width))
    {
      wdata->begin = mutt_mb_width_ceiling(es->wbuf, es->lastchar,
                                           mutt_mb_wcswidth(es->wbuf, es->curpos) -
                                               (width / 2));
    }
    mutt_window_move(win, wdata->col, 0);
    int w = 0;
    for (size_t i = wdata->begin; i < es->lastchar; i++)
    {
      w += mutt_mb_wcwidth(es->wbuf[i]);
      if (w > width)
        break;
      my_addwch(win, es->wbuf[i]);
    }
    mutt_window_clrtoeol(win);
    mutt_window_move(win,
                     wdata->col + mutt_mb_wcswidth(es->wbuf + wdata->begin,
                                                   es->curpos - wdata->begin),
                     0);
  }

  // Restore the cursor position after drawing the screen
  // int r = 0, c = 0;
  // mutt_window_get_coords(win, &c, &r);
  // window_redraw(NULL);
  // mutt_window_move(win, c, r);
  mutt_debug(LL_DEBUG1, "repaint done\n");
  return 0;
}

/**
 * enter_state_observer - Notification that the Enter state has changed - Implements ::observer_t - @ingroup observer_api
 */
int enter_state_observer(struct NotifyCallback *nc)
{
  if ((nc->event_type != NT_ENTER) || !nc->global_data)
    return -1;

  struct MuttWindow *win = nc->global_data;

  if (nc->event_subtype & NT_ENTER)
    win->actions |= WA_RECALC;
  else
    win->actions |= WA_REPAINT;

  return 0;
}

/**
 * self_insert - Insert a normal character
 * @param wdata Enter window data
 * @param ch    Raw keypress
 * @retval true If done (enter pressed)
 */
enum InsertResult self_insert(struct EnterWindowData *wdata, int ch)
{
  if (!wdata)
    return IR_ERROR;

  wdata->tabs = 0;
  if ((ch == '\r') || (ch == '\n'))
    return IR_ENTER;

  if (ch & ~0xff)
  {
    // Expand an unhandled function key to its name
    const char *str = km_keyname(ch);
    if (!str)
      return IR_ERROR;
    for (size_t i = 0; str[i]; i++)
    {
      enum InsertResult ir = inner_self_insert(wdata->state, str[i]);
      if (ir != IR_GOOD)
        return ir;
    }
    return IR_GOOD;
  }

  return inner_self_insert(wdata->state, ch);
}

/**
 * buf_get_field - Ask the user for a string
 * @param[in]  field    Prompt
 * @param[in]  buf      Buffer for the result
 * @param[in]  complete Flags, see #CompletionFlags
 * @param[in]  multiple Allow multiple selections
 * @param[in]  m        Mailbox
 * @param[out] files    List of files selected
 * @param[out] numfiles Number of files selected
 * @retval 0  Selection made
 * @retval -1 Aborted
 */
int buf_get_field(const char *field, struct Buffer *buf, CompletionFlags complete,
                  bool multiple, struct Mailbox *m, char ***files, int *numfiles)
{
  struct MuttWindow *win = mutt_window_new(WT_CUSTOM, MUTT_WIN_ORIENT_VERTICAL, MUTT_WIN_SIZE_FIXED,
                                           MUTT_WIN_SIZE_UNLIMITED, 1);
  win->recalc = enter_window_recalc;
  win->repaint = enter_window_repaint;
  win->actions |= WA_RECALC;

  msgcont_push_window(win);

  const bool old_oime = OptIgnoreMacroEvents;
  if (complete & MUTT_COMP_UNBUFFERED)
    OptIgnoreMacroEvents = true;

  int rc = 0;
  int col = 0;

  struct EnterState *es = enter_state_new();

  // clang-format off
  struct EnterWindowData wdata = { buf, col, complete,
    multiple, m, files, numfiles, state, field, NULL, ENTER_REDRAW_NONE,
    (complete & MUTT_COMP_PASS), 0, NULL, 0, 0, 0, false, NULL };
  // clang-format on
  win->wdata = &wdata;

  notify_observer_add(state->notify, NT_ENTER, enter_state_observer, win);

  struct Notify *notify = enter_state_get_notify(es);
  notify_set_parent(notify, NeoMutt->notify);

  win->help_data = EditorHelp;
  win->help_menu = MENU_EDITOR;
  struct MuttWindow *old_focus = window_set_focus(win);

  enum MuttCursorState cursor = mutt_curses_set_cursor(MUTT_CURSOR_VISIBLE);

  /* Initialise wbuf from buf */
  state->wbuflen = 0;
  state->lastchar = mutt_mb_mbstowcs(&state->wbuf, &state->wbuflen, 0,
                                     buf_string(wdata.buf));
  state->curpos = state->lastchar;

  if (wdata.flags & MUTT_COMP_FILE)
    wdata.hclass = HC_FILE;
  else if (wdata.flags & MUTT_COMP_FILE_MBOX)
    wdata.hclass = HC_MBOX;
  else if (wdata.flags & MUTT_COMP_FILE_SIMPLE)
    wdata.hclass = HC_CMD;
  else if (wdata.flags & MUTT_COMP_ALIAS)
    wdata.hclass = HC_ALIAS;
  else if (wdata.flags & MUTT_COMP_COMMAND)
    wdata.hclass = HC_COMMAND;
  else if (wdata.flags & MUTT_COMP_PATTERN)
    wdata.hclass = HC_PATTERN;
  else
    wdata.hclass = HC_OTHER;

  msgcont_push_window(win);

  // ---------------------------------------------------------------------------
  // Event Loop
  do
  {
    // window_invalidate_all();
    window_redraw(NULL);
    struct KeyEvent event = km_dokey_event(MENU_EDITOR);
    if (event.op == OP_TIMEOUT)
      continue;

    if (event.op == OP_ABORT)
    {
      rc = -1;
      break;
    }
    int width = win->state.cols - col - 1;

    // clang-format off
    struct EnterWindowData wdata = { buf, col, complete,
      multiple, m, files, numfiles, es, ENTER_REDRAW_NONE,
      (complete & MUTT_COMP_PASS), 0, NULL, 0, 0, 0, false, NULL };
    // clang-format on
    win->wdata = &wdata;

    if (es->wbuf[0] == L'\0')
    {
      /* Initialise wbuf from buf */
      wdata.state->wbuflen = 0;
      wdata.state->lastchar = mutt_mb_mbstowcs(&wdata.state->wbuf, &wdata.state->wbuflen,
                                               0, buf_string(wdata.buf));
      wdata.redraw = ENTER_REDRAW_INIT;
    }
    else
    {
      wdata.redraw = ENTER_REDRAW_LINE;
    }

    if (event.op == OP_NULL)
    {
      if (wdata.flags & MUTT_COMP_CLEAR)
      {
        wdata.flags &= ~MUTT_COMP_CLEAR;
        editor_kill_whole_line(wdata.state);
      }
      if (complete & MUTT_COMP_PASS)
        mutt_debug(LL_DEBUG1, "Got char *\n");
      else
        mutt_debug(LL_DEBUG1, "Got char %c (0x%02x)\n", event.ch, event.ch);
      if (self_insert(&wdata, event.ch) == IR_ENTER)
        break;
      continue;
    }

    mutt_debug(LL_DEBUG1, "Got op %s (%d)\n", opcodes_get_name(event.op), event.op);

    if ((event.op != OP_EDITOR_COMPLETE) && (event.op != OP_EDITOR_COMPLETE_QUERY))
      wdata.tabs = 0;
    wdata.redraw = ENTER_REDRAW_LINE;

    int rc_disp = enter_function_dispatcher(win, event.op);
    switch (rc_disp)
    {
      case FR_NO_ACTION:
        if (wdata.flags & MUTT_COMP_CLEAR)
        {
          wdata.flags &= ~MUTT_COMP_CLEAR;
          editor_kill_whole_line(wdata.state);
        }
        if (self_insert(&wdata, event.ch) == IR_ENTER)
          wdata.done = true;
        break;

      case FR_CONTINUE:
        win->actions |= WA_RECALC;
        break;

      case FR_SUCCESS:
        break;

      case FR_UNKNOWN:
        rc_disp = global_function_dispatcher(win, event.op);
        break;

      case FR_ERROR:
      default:
        mutt_beep(false);
    }
  } while (!wdata.done);
  // ---------------------------------------------------------------------------

  /* Convert from wide characters */
  mutt_mb_wcstombs(wdata.state->wbuf, wdata.state->lastchar, wdata.buf);
  if (!wdata.pass)
    mutt_hist_add(wdata.hclass, buf_string(wdata.buf), true);

  if (wdata.multiple)
  {
    char **tfiles = NULL;
    *wdata.numfiles = 1;
    tfiles = mutt_mem_calloc(*wdata.numfiles, sizeof(char *));
    buf_expand_path_regex(wdata.buf, false);
    tfiles[0] = buf_strdup(wdata.buf);
    *wdata.files = tfiles;
  }

  msgcont_pop_window();
  mutt_window_free(&win);

  if (rc == 0)
    buf_fix_dptr(buf);
  else
    buf_reset(buf);

  enter_state_free(&es);
  mutt_hist_reset_state(wdata.hclass);
  FREE(&wdata.tempbuf);

  mutt_curses_set_cursor(cursor);
  window_set_focus(old_focus);
  OptIgnoreMacroEvents = old_oime;
  return rc;
}
