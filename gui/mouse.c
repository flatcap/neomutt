/**
 * @file
 * Mouse handling
 *
 * @authors
 * Copyright (C) 2020 Richard Russon <rich@flatcap.org>
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
 * @page gui_mouse Mouse handling
 *
 * Mouse handling
 */

#include "config.h"
#include <stdbool.h>
#include "mutt/lib.h"
#include "mouse.h"
#include "mutt_curses.h"
#include "mutt_window.h"
#include "rootwin.h"

/**
 * in_window - XXX
 */
static bool in_window(struct MuttWindow *win, int col, int row)
{
  if (!win)
    return false;
  if (!win->state.visible)
    return false;
  if ((col < win->state.col_offset) ||
      (col >= (win->state.col_offset + win->state.cols)))
  {
    return false;
  }
  if ((row < win->state.row_offset) ||
      (row >= (win->state.row_offset + win->state.rows)))
  {
    return false;
  }
  return true;
}

/**
 * window_by_posn - XXX
 */
static struct MuttWindow *window_by_posn(struct MuttWindow *win, int col, int row)
{
  if (!in_window(win, col, row))
    return NULL;

  struct MuttWindow *np = NULL;
  TAILQ_FOREACH(np, &win->children, entries)
  {
    if (in_window(np, col, row))
      return window_by_posn(np, col, row);
  }

  return win;
}

static void dump_event(MEVENT *event)
{
  mutt_debug(LL_DEBUG1, "x = %d, y = %d, z = %d\n", event->x, event->y, event->z);

  unsigned long s = event->bstate;

  // clang-format off
  if (s & BUTTON_CTRL)            { mutt_debug(LL_DEBUG1, "CTRL\n");         s &= ~BUTTON_CTRL;            }
  if (s & BUTTON_SHIFT)           { mutt_debug(LL_DEBUG1, "SHIFT\n");        s &= ~BUTTON_SHIFT;           }
  if (s & BUTTON_ALT)             { mutt_debug(LL_DEBUG1, "ALT\n");          s &= ~BUTTON_ALT;             }

  if (s & BUTTON1_RELEASED)       { mutt_debug(LL_DEBUG1, "BUT1-release\n"); s &= ~BUTTON1_RELEASED;       }
  if (s & BUTTON1_PRESSED)        { mutt_debug(LL_DEBUG1, "BUT1-pressed\n"); s &= ~BUTTON1_PRESSED;        }
  if (s & BUTTON1_CLICKED)        { mutt_debug(LL_DEBUG1, "BUT1-clicked\n"); s &= ~BUTTON1_CLICKED;        }
  if (s & BUTTON1_DOUBLE_CLICKED) { mutt_debug(LL_DEBUG1, "BUT1-double\n");  s &= ~BUTTON1_DOUBLE_CLICKED; }
  if (s & BUTTON1_TRIPLE_CLICKED) { mutt_debug(LL_DEBUG1, "BUT1-triple\n");  s &= ~BUTTON1_TRIPLE_CLICKED; }

  if (s & BUTTON2_RELEASED)       { mutt_debug(LL_DEBUG1, "BUT2-release\n"); s &= ~BUTTON2_RELEASED;       }
  if (s & BUTTON2_PRESSED)        { mutt_debug(LL_DEBUG1, "BUT2-pressed\n"); s &= ~BUTTON2_PRESSED;        }
  if (s & BUTTON2_CLICKED)        { mutt_debug(LL_DEBUG1, "BUT2-clicked\n"); s &= ~BUTTON2_CLICKED;        }
  if (s & BUTTON2_DOUBLE_CLICKED) { mutt_debug(LL_DEBUG1, "BUT2-double\n");  s &= ~BUTTON2_DOUBLE_CLICKED; }
  if (s & BUTTON2_TRIPLE_CLICKED) { mutt_debug(LL_DEBUG1, "BUT2-triple\n");  s &= ~BUTTON2_TRIPLE_CLICKED; }

  if (s & BUTTON3_RELEASED)       { mutt_debug(LL_DEBUG1, "BUT3-release\n"); s &= ~BUTTON3_RELEASED;       }
  if (s & BUTTON3_PRESSED)        { mutt_debug(LL_DEBUG1, "BUT3-pressed\n"); s &= ~BUTTON3_PRESSED;        }
  if (s & BUTTON3_CLICKED)        { mutt_debug(LL_DEBUG1, "BUT3-clicked\n"); s &= ~BUTTON3_CLICKED;        }
  if (s & BUTTON3_DOUBLE_CLICKED) { mutt_debug(LL_DEBUG1, "BUT3-double\n");  s &= ~BUTTON3_DOUBLE_CLICKED; }
  if (s & BUTTON3_TRIPLE_CLICKED) { mutt_debug(LL_DEBUG1, "BUT3-triple\n");  s &= ~BUTTON3_TRIPLE_CLICKED; }

  if (s & BUTTON4_RELEASED)       { mutt_debug(LL_DEBUG1, "BUT4-release\n"); s &= ~BUTTON4_RELEASED;       }
  if (s & BUTTON4_PRESSED)        { mutt_debug(LL_DEBUG1, "BUT4-pressed\n"); s &= ~BUTTON4_PRESSED;        }
  if (s & BUTTON4_CLICKED)        { mutt_debug(LL_DEBUG1, "BUT4-clicked\n"); s &= ~BUTTON4_CLICKED;        }
  if (s & BUTTON4_DOUBLE_CLICKED) { mutt_debug(LL_DEBUG1, "BUT4-double\n");  s &= ~BUTTON4_DOUBLE_CLICKED; }
  if (s & BUTTON4_TRIPLE_CLICKED) { mutt_debug(LL_DEBUG1, "BUT4-triple\n");  s &= ~BUTTON4_TRIPLE_CLICKED; }

  if (s & BUTTON5_RELEASED)       { mutt_debug(LL_DEBUG1, "BUT5-release\n"); s &= ~BUTTON5_RELEASED;       }
  if (s & BUTTON5_PRESSED)        { mutt_debug(LL_DEBUG1, "BUT5-pressed\n"); s &= ~BUTTON5_PRESSED;        }
  if (s & BUTTON5_CLICKED)        { mutt_debug(LL_DEBUG1, "BUT5-clicked\n"); s &= ~BUTTON5_CLICKED;        }
  if (s & BUTTON5_DOUBLE_CLICKED) { mutt_debug(LL_DEBUG1, "BUT5-double\n");  s &= ~BUTTON5_DOUBLE_CLICKED; }
  if (s & BUTTON5_TRIPLE_CLICKED) { mutt_debug(LL_DEBUG1, "BUT5-triple\n");  s &= ~BUTTON5_TRIPLE_CLICKED; }

  if (s & 268435456)              { mutt_debug(LL_DEBUG1, "268435456\n"); s &= ~268435456;               }
  // clang-format on

  if (s != 0)
    mutt_debug(LL_DEBUG1, "s = %lu\n", s);
}

/**
 * mouse_handle_event - Process mouse events
 * @param ch Character, from getch()
 * @retval true If it is a mouse Event
 */
bool mouse_handle_event(int ch)
{
  if (ch != KEY_MOUSE)
    return false;

  if (!RootWindow)
    return true;

  MEVENT event = { 0 };
  if (getmouse(&event) != OK)
    return true;

  dump_event(&event);

  struct MuttWindow *win = window_by_posn(RootWindow, event.x, event.y);

  for (; win; win = win->parent)
  {
    if (win->mouse)
    {
      struct EventMouse em = { MUTT_MOUSE_NO_FLAGS, MB_BUTTON1, ME_CLICK,
                               event.x - win->state.col_offset,
                               event.y - win->state.row_offset };
      if (win->mouse(win, &em))
        return true;
    }
  }

  // Not handled
  return true;
}
