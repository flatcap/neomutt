/**
 * @file
 * Filler Window
 *
 * @authors
 * Copyright (C) 2022 Richard Russon <rich@flatcap.org>
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
 * @page gui_filler Filler Window
 *
 * Filler Window
 */

#include "config.h"
#include <string.h>
#include "mutt/lib.h"
#include "lib.h"
#include "color/lib.h"

/**
 * struct FillerWindowData - Filler Window private data
 */
struct FillerWindowData
{
  char ch;   ///< Character to display
  int color; ///< Which QuotedN colour to use
};

/**
 * fw_data_new - Create a new Filler Window
 */
struct FillerWindowData *fw_data_new(void)
{
  struct FillerWindowData *wdata = mutt_mem_calloc(1, sizeof(struct FillerWindowData));

  return wdata;
}

/**
 * fw_data_free - Free the private data attached to the Filler Window - Implements MuttWindow::wdata_free() - @ingroup window_wdata_free
 */
void fw_data_free(struct MuttWindow *win, void **ptr)
{
  // struct FillerWindowData *wdata = *ptr;

  FREE(ptr);
}

/**
 * filler_window_recalc - Recalculate the Filler - Implements MuttWindow::recalc() - @ingroup window_recalc
 */
static int filler_window_recalc(struct MuttWindow *win)
{
  win->actions = WA_REPAINT;
  return 0;
}

/**
 * filler_window_repaint - Repaint the Filler - Implements MuttWindow::repaint() - @ingroup window_repaint
 */
static int filler_window_repaint(struct MuttWindow *win)
{
  char text[1024];

  struct FillerWindowData *wdata = win->wdata;

  size_t size = MIN(win->state.cols, sizeof(text) - 1);
  memset(text, wdata->ch, size);
  text[size] = '\0';

  mutt_curses_set_color(quoted_colors_get(wdata->color));
  for (int r = 0; r < win->state.rows; r++)
  {
    mutt_window_mvaddstr(win, 0, r, text);
  }
  mutt_curses_set_color_by_id(MT_COLOR_NORMAL);

  wdata->ch++;
  if (wdata->ch > 'Z')
    wdata->ch = 'A';

  return 0;
}

/**
 * filler_window_new - Create a new Filler Window
 * @param color Which QuotedN colour to use
 * @retval ptr New Window
 */
struct MuttWindow *filler_window_new(int color)
{
  struct MuttWindow *win = mutt_window_new(WT_STATUS_BAR, MUTT_WIN_ORIENT_VERTICAL,
                                           MUTT_WIN_SIZE_FIXED,
                                           MUTT_WIN_SIZE_UNLIMITED, 1 + (color % 2));
  win->recalc = filler_window_recalc;
  win->repaint = filler_window_repaint;
  win->actions = WA_REPAINT;

  struct FillerWindowData *wdata = fw_data_new();
  wdata->color = color;
  wdata->ch = 'A';

  win->wdata = wdata;
  win->wdata_free = fw_data_free;

  return win;
}
