/**
 * @file
 * Simple Pager Dialog
 *
 * @authors
 * Copyright (C) 2021 Richard Russon <rich@flatcap.org>
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
 * @page pager_simple Simple Pager Dialog
 *
 * Simple Pager Dialog
 */

#include "config.h"
#include <stddef.h>
#include <assert.h>
#include <inttypes.h> // IWYU pragma: keep
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mutt/lib.h"
#include "config/lib.h"
#include "email/lib.h"
#include "core/lib.h"
#include "alias/lib.h"
#include "gui/lib.h"
#include "mutt.h"
#include "lib.h"
#include "attach/lib.h"
#include "color/lib.h"
#include "index/lib.h"
#include "menu/lib.h"
#include "ncrypt/lib.h"
#include "question/lib.h"
#include "send/lib.h"
#include "commands.h"
#include "context.h"
#include "display.h"
#include "format_flags.h"
#include "functions.h"
#include "hdrline.h"
#include "hook.h"
#include "keymap.h"
#include "mutt_globals.h"
#include "mutt_header.h"
#include "mutt_logging.h"
#include "mutt_mailbox.h"
#include "muttlib.h"
#include "mx.h"
#include "opcodes.h"
#include "options.h"
#include "private_data.h"
#include "protos.h"
#include "recvcmd.h"
#include "status.h"
#ifdef ENABLE_NLS
#include <libintl.h>
#endif

/**
 * struct Resize - Keep track of screen resizing
 */
struct Resize
{
  int line;
  bool search_compiled;
  bool search_back;
};


static struct Resize *Resize = NULL;

/// Help Bar for the Pager's Help Page
static const struct Mapping PagerHelp[] = {
  // clang-format off
  { N_("Exit"),          OP_EXIT },
  { N_("PrevPg"),        OP_PREV_PAGE },
  { N_("NextPg"),        OP_NEXT_PAGE },
  { N_("Help"),          OP_HELP },
  { NULL, 0 },
  // clang-format on
};

/**
 * dopager_config_observer - Notification that a Config Variable has changed - Implements ::observer_t
 */
static int dopager_config_observer(struct NotifyCallback *nc)
{
  if ((nc->event_type != NT_CONFIG) || !nc->global_data || !nc->event_data)
    return -1;

  struct EventConfig *ev_c = nc->event_data;
  if (!mutt_str_equal(ev_c->name, "status_on_top"))
    return 0;

  struct MuttWindow *dlg = nc->global_data;
  window_status_on_top(dlg, NeoMutt->sub);
  mutt_debug(LL_DEBUG5, "config done, request WA_REFLOW\n");
  return 0;
}

/**
 * dopager_window_observer - Notification that a Window has changed - Implements ::observer_t
 */
static int dopager_window_observer(struct NotifyCallback *nc)
{
  if ((nc->event_type != NT_WINDOW) || !nc->global_data || !nc->event_data)
    return -1;

  if (nc->event_subtype != NT_WINDOW_DELETE)
    return 0;

  struct MuttWindow *dlg = nc->global_data;
  struct EventWindow *ev_w = nc->event_data;
  if (ev_w->win != dlg)
    return 0;

  notify_observer_remove(NeoMutt->notify, dopager_config_observer, dlg);
  notify_observer_remove(dlg->notify, dopager_window_observer, dlg);
  mutt_debug(LL_DEBUG5, "window delete done\n");

  return 0;
}

/**
 * pager_custom_redraw - Redraw the pager window
 * @param priv Private Pager data
 */
static void pager_custom_redraw(struct PagerPrivateData *priv)
{
  //---------------------------------------------------------------------------
  // ASSUMPTIONS & SANITY CHECKS
  //---------------------------------------------------------------------------
  // Since pager_custom_redraw() is a static function and it is always called
  // after pager_simple() we can rely on a series of sanity checks in
  // pager_simple(), namely:
  // - PAGER_MODE_OTHER  guarantees (!data->email) and (!data->body)
  //
  // Additionally, while refactoring is still in progress the following checks
  // are still here to ensure data model consistency.
  assert(priv);        // Redraw function can't be called without its data.
  assert(priv->pview); // Redraw data can't exist separately without the view.
  assert(priv->pview->pdata); // View can't exist without its data
  //---------------------------------------------------------------------------

  char buf[1024] = { 0 };

  const bool c_tilde = cs_subset_bool(NeoMutt->sub, "tilde");

  if (priv->redraw & MENU_REDRAW_FULL)
  {
    mutt_curses_set_color(MT_COLOR_NORMAL);
    mutt_window_clear(priv->pview->win_pager);

    if (Resize)
    {
      priv->search_compiled = Resize->search_compiled;
      if (priv->search_compiled)
      {
        uint16_t flags = mutt_mb_is_lower(priv->search_str) ? REG_ICASE : 0;
        const int err = REG_COMP(&priv->search_re, priv->search_str, REG_NEWLINE | flags);
        if (err == 0)
        {
          priv->search_flag = MUTT_SEARCH;
          priv->search_back = Resize->search_back;
        }
        else
        {
          regerror(err, &priv->search_re, buf, sizeof(buf));
          mutt_error("%s", buf);
          priv->search_compiled = false;
        }
      }
      priv->win_height = Resize->line;
      pager_queue_redraw(priv, MENU_REDRAW_FLOW);

      FREE(&Resize);
    }

    pager_queue_redraw(priv, MENU_REDRAW_BODY | MENU_REDRAW_INDEX);
  }

  if (priv->redraw & MENU_REDRAW_FLOW)
  {
    if (!(priv->pview->flags & MUTT_PAGER_RETWINCH))
    {
      priv->win_height = -1;
      for (int i = 0; i <= priv->top_line; i++)
        if (!priv->lines[i].cont_line)
          priv->win_height++;
      for (int i = 0; i < priv->lines_max; i++)
      {
        priv->lines[i].offset = 0;
        priv->lines[i].color = -1;
        priv->lines[i].cont_line = 0;
        priv->lines[i].syntax_arr_size = 0;
        priv->lines[i].search_arr_size = -1;
        priv->lines[i].quote = NULL;

        mutt_mem_realloc(&(priv->lines[i].syntax), sizeof(struct TextSyntax));
        if (priv->search_compiled && priv->lines[i].search)
          FREE(&(priv->lines[i].search));
      }

      priv->lines_used = 0;
      priv->top_line = 0;
    }
    int i = -1;
    int j = -1;
    while (display_line(priv->fp, &priv->bytes_read, &priv->lines, ++i,
                        &priv->lines_used, &priv->lines_max,
                        priv->has_types | priv->search_flag | (priv->pview->flags & MUTT_PAGER_NOWRAP),
                        &priv->quote_list, &priv->q_level, &priv->force_redraw,
                        &priv->search_re, priv->pview->win_pager) == 0)
    {
      if (!priv->lines[i].cont_line && (++j == priv->win_height))
      {
        priv->top_line = i;
        if (!priv->search_flag)
          break;
      }
    }
  }

  if ((priv->redraw & MENU_REDRAW_BODY) || (priv->top_line != priv->old_top_line))
  {
    do
    {
      mutt_window_move(priv->pview->win_pager, 0, 0);
      priv->cur_line = priv->top_line;
      priv->old_top_line = priv->top_line;
      priv->win_height = 0;
      priv->force_redraw = false;

      while ((priv->win_height < priv->pview->win_pager->state.rows) &&
             (priv->lines[priv->cur_line].offset <= priv->st.st_size - 1))
      {
        if (display_line(priv->fp, &priv->bytes_read, &priv->lines,
                         priv->cur_line, &priv->lines_used, &priv->lines_max,
                         (priv->pview->flags & MUTT_DISPLAYFLAGS) | priv->search_flag |
                         (priv->pview->flags & MUTT_PAGER_NOWRAP),
                         &priv->quote_list, &priv->q_level, &priv->force_redraw,
                         &priv->search_re, priv->pview->win_pager) > 0)
        {
          priv->win_height++;
        }
        priv->cur_line++;
        mutt_window_move(priv->pview->win_pager, 0, priv->win_height);
      }
    } while (priv->force_redraw);

    mutt_curses_set_color(MT_COLOR_TILDE);
    while (priv->win_height < priv->pview->win_pager->state.rows)
    {
      mutt_window_clrtoeol(priv->pview->win_pager);
      if (c_tilde)
        mutt_window_addch(priv->pview->win_pager, '~');
      priv->win_height++;
      mutt_window_move(priv->pview->win_pager, 0, priv->win_height);
    }
    mutt_curses_set_color(MT_COLOR_NORMAL);

    /* We are going to update the pager status bar, so it isn't
     * necessary to reset to normal color now. */
  }

  priv->redraw = MENU_REDRAW_NO_FLAGS;
  mutt_debug(LL_DEBUG5, "repaint done\n");
}

/**
 * pager_simple - Display some text in a window
 * @param pview Pager view settings
 * @retval  0 Success
 * @retval -1 Error
 */
int pager_simple(struct PagerView *pview)
{
  //===========================================================================
  // ACT 1 - Ensure sanity of the caller and determine the mode
  //===========================================================================
  assert(pview);
  assert(pview->mode == PAGER_MODE_OTHER);
  assert(pview->pdata); // view can't exist in a vacuum
  assert(pview->win_pager);
  assert(pview->win_pbar);
  assert(!pview->pdata->body);

  //===========================================================================
  // ACT 2 - Declare, initialize local variables, read config, etc.
  //===========================================================================

  //---------- local variables ------------------------------------------------
  int op = 0;
  struct PagerPrivateData *priv = pview->win_pager->parent->wdata;
  priv->rc = -1;
  priv->searchctx = 0;
  priv->wrapped = false;

  {
    // Wipe any previous state info
    memset(priv, 0, sizeof(*priv));
    priv->win_pbar = pview->win_pbar;
  }

  //---------- setup flags ----------------------------------------------------
  if (!(pview->flags & MUTT_SHOWCOLOR))
    pview->flags |= MUTT_SHOWFLAT;

  //---------- setup help menu ------------------------------------------------
  pview->win_pager->help_data = PagerHelp;
  pview->win_pager->help_menu = MENU_PAGER;

  //---------- initialize redraw pdata  -----------------------------------------
  pview->win_pager->size = MUTT_WIN_SIZE_MAXIMISE;
  priv->pview = pview;
  priv->lines_max = LINES; // number of lines on screen, from curses
  priv->lines = mutt_mem_calloc(priv->lines_max, sizeof(struct Line));
  priv->fp = fopen(pview->pdata->fname, "r");
  priv->has_types = ((pview->flags & MUTT_SHOWCOLOR)) ? MUTT_TYPES : 0; // main message or rfc822 attachment

  for (size_t i = 0; i < priv->lines_max; i++)
  {
    priv->lines[i].color = -1;
    priv->lines[i].search_arr_size = -1;
    priv->lines[i].syntax = mutt_mem_malloc(sizeof(struct TextSyntax));
    (priv->lines[i].syntax)[0].first = -1;
    (priv->lines[i].syntax)[0].last = -1;
  }

  // ---------- try to open the pdata file -------------------------------------
  if (!priv->fp)
  {
    mutt_perror(pview->pdata->fname);
    return -1;
  }

  if (stat(pview->pdata->fname, &priv->st) != 0)
  {
    mutt_perror(pview->pdata->fname);
    mutt_file_fclose(&priv->fp);
    return -1;
  }
  unlink(pview->pdata->fname);

  //---------- show windows, set focus and visibility --------------------------
  window_set_focus(pview->win_pager);

  //---------- jump to the bottom if requested ------------------------------
  if (pview->flags & MUTT_PAGER_BOTTOM)
  {
    jump_to_bottom(priv, pview);
  }

  //-------------------------------------------------------------------------
  // ACT 3: Read user input and decide what to do with it
  //        ...but also do a whole lot of other things.
  //-------------------------------------------------------------------------
  while (op != -1)
  {
    mutt_curses_set_cursor(MUTT_CURSOR_INVISIBLE);

    pager_queue_redraw(priv, MENU_REDRAW_FULL);
    pager_custom_redraw(priv);
    notify_send(priv->notify, NT_PAGER, NT_PAGER_VIEW, priv);
    window_redraw(NULL);

    const bool c_braille_friendly =
        cs_subset_bool(NeoMutt->sub, "braille_friendly");
    if (c_braille_friendly)
    {
      if (braille_row != -1)
      {
        mutt_window_move(priv->pview->win_pager, braille_col, braille_row + 1);
        braille_row = -1;
      }
    }
    else
      mutt_window_move(priv->pview->win_pbar, priv->pview->win_pager->state.cols - 1, 0);

    // force redraw of the screen at every iteration of the event loop
    mutt_refresh();

    //-------------------------------------------------------------------------
    // Check if information in the status bar needs an update
    // This is done because pager is a single-threaded application, which
    // tries to emulate concurrency.
    //-------------------------------------------------------------------------

    if (SigWinch)
    {
      SigWinch = false;
      mutt_resize_screen();
      clearok(stdscr, true); /* force complete redraw */
      msgwin_clear_text();

      pager_queue_redraw(priv, MENU_REDRAW_FLOW);
      if (pview->flags & MUTT_PAGER_RETWINCH)
      {
        /* Store current position. */
        priv->win_height = -1;
        for (size_t i = 0; i <= priv->top_line; i++)
          if (!priv->lines[i].cont_line)
            priv->win_height++;

        Resize = mutt_mem_malloc(sizeof(struct Resize));

        Resize->line = priv->win_height;
        Resize->search_compiled = priv->search_compiled;
        Resize->search_back = priv->search_back;

        op = -1;
        priv->rc = OP_REFORMAT_WINCH;
      }
      else
      {
        /* note: mutt_resize_screen() -> mutt_window_reflow() sets
         * MENU_REDRAW_FULL and MENU_REDRAW_FLOW */
        op = 0;
      }
      continue;
    }
    //-------------------------------------------------------------------------
    // Finally, read user's key press
    //-------------------------------------------------------------------------
    // km_dokey() reads not only user's key strokes, but also a MacroBuffer
    // MacroBuffer may contain OP codes of the operations.
    // MacroBuffer is global
    // OP codes inserted into the MacroBuffer by various functions.
    // One of such functions is `mutt_enter_command()`
    // Some OP codes are not handled by pager, they cause pager to quit returning
    // OP code to index. Index handles the operation and then restarts pager
    op = km_dokey(MENU_PAGER);

    if (op >= 0)
    {
      mutt_clear_error();
      mutt_debug(LL_DEBUG1, "Got op %s (%d)\n", OpStrings[op][0], op);
    }
    mutt_curses_set_cursor(MUTT_CURSOR_VISIBLE);

    if (op < 0)
    {
      op = 0;
      mutt_timeout_hook();
      continue;
    }

    priv->rc = op;

    if (op == OP_NULL)
    {
      km_error_key(MENU_PAGER);
      continue;
    }

    int rc = pager_function_dispatcher(priv->pview->win_pager, op);
    if (rc == IR_DONE)
      break;
    if (rc == IR_UNKNOWN)
      break;
  }
  //-------------------------------------------------------------------------
  // END OF ACT 3: Read user input loop - while (op != -1)
  //-------------------------------------------------------------------------

  mutt_file_fclose(&priv->fp);

  for (size_t i = 0; i < priv->lines_max; i++)
  {
    FREE(&(priv->lines[i].syntax));
    if (priv->search_compiled && priv->lines[i].search)
      FREE(&(priv->lines[i].search));
  }
  if (priv->search_compiled)
  {
    regfree(&priv->search_re);
    priv->search_compiled = false;
  }
  FREE(&priv->lines);

  return (priv->rc != -1) ? priv->rc : 0;
}

/**
 * pager_do_simple - Display some page-able text to the user (help or attachment)
 * @param pview PagerView to construct Pager object
 * @retval  0 Success
 * @retval -1 Error
 */
int pager_do_simple(struct PagerView *pview)
{
  assert(pview);
  assert(pview->pdata);
  assert(pview->pdata->fname);
  assert(pview->mode == PAGER_MODE_OTHER);

  struct MuttWindow *dlg =
      mutt_window_new(WT_DLG_DO_PAGER, MUTT_WIN_ORIENT_VERTICAL, MUTT_WIN_SIZE_MAXIMISE,
                      MUTT_WIN_SIZE_UNLIMITED, MUTT_WIN_SIZE_UNLIMITED);

  const bool c_status_on_top = cs_subset_bool(NeoMutt->sub, "status_on_top");
  struct MuttWindow *panel_pager = ppanel_new(c_status_on_top, NULL);
  dlg->focus = panel_pager;
  mutt_window_add_child(dlg, panel_pager);

  notify_observer_add(NeoMutt->notify, NT_CONFIG, dopager_config_observer, dlg);
  notify_observer_add(dlg->notify, NT_WINDOW, dopager_window_observer, dlg);
  dialog_push(dlg);

  pview->win_index = NULL;
  pview->win_pbar = window_find_child(panel_pager, WT_STATUS_BAR);
  pview->win_pager = window_find_child(panel_pager, WT_CUSTOM);

  int rc;

  const char *const c_pager = cs_subset_string(NeoMutt->sub, "pager");
  if (!c_pager || mutt_str_equal(c_pager, "builtin"))
  {
    rc = pager_simple(pview);
  }
  else
  {
    struct Buffer *cmd = mutt_buffer_pool_get();

    mutt_endwin();
    mutt_buffer_file_expand_fmt_quote(cmd, c_pager, pview->pdata->fname);
    if (mutt_system(mutt_buffer_string(cmd)) == -1)
    {
      mutt_error(_("Error running \"%s\""), mutt_buffer_string(cmd));
      rc = -1;
    }
    else
      rc = 0;
    mutt_file_unlink(pview->pdata->fname);
    mutt_buffer_pool_release(&cmd);
  }

  dialog_pop();
  mutt_window_free(&dlg);
  return rc;
}
