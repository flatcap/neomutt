FR_SUCCESS   - Completion changed, update text buffer
FR_NO_ACTION - No change, e.g. user cancelled dialog
FR_ERROR     - Error occurred

what about "selection made", finish?
  FR_DONE?
  or flag in struct?

static int complete_file_simple(struct EnterWindowData *wdata)
{
  const wchar_t *wbuf = editor_buffer_get_buffer(wdata->state);
  const int curpos = editor_buffer_get_cursor(wdata->state);
  const int lastchar = editor_buffer_get_lastchar(wdata->state);

  int rc = FR_SUCCESS;
  size_t i;
  for (i = curpos; (i > 0) && !mutt_mb_is_shell_char(wbuf[i - 1]); i--)
    ;

  mutt_mb_wcstombs(wbuf + i, curpos - i, wdata->buf);
  if (wdata->tempbuf && (wdata->templen == (lastchar - i)) && (memcmp(wdata->tempbuf, wbuf + i, (lastchar - i) * sizeof(wchar_t)) == 0))
  {
    buf_select_file(wdata->buf, MUTT_SEL_NO_FLAGS, wdata->m, NULL, NULL);
    if (!buf_is_empty(wdata->buf))
      editor_buffer_replace_part(wdata->state, i, buf_string(wdata->buf));
    return FR_CONTINUE;
  }

  if (mutt_complete(wdata->buf->data, wdata->buf->dsize) == 0)
  {
    wdata->templen = lastchar - i;
    mutt_mem_realloc(&wdata->tempbuf, wdata->templen * sizeof(wchar_t));
    memcpy(wdata->tempbuf, wbuf + i, wdata->templen * sizeof(wchar_t));
  }
  else
  {
    rc = FR_ERROR;
  }

  editor_buffer_replace_part(wdata->state, i, buf_string(wdata->buf));
  return rc;
}

static int complete_alias_complete(struct EnterWindowData *wdata)
{
  const wchar_t *wbuf = editor_buffer_get_buffer(wdata->state);
  const int curpos = editor_buffer_get_cursor(wdata->state);
  const int lastchar = editor_buffer_get_lastchar(wdata->state);

  size_t i;
  for (i = curpos; (i > 0) && (wbuf[i - 1] != ',') && (wbuf[i - 1] != ':'); i--)
    ;

  for (; (i < lastchar) && (wbuf[i] == ' '); i++)
    ;

  mutt_mb_wcstombs(wbuf + i, curpos - i, wdata->buf);

  int rc = alias_complete(wdata->buf->data, wdata->buf->dsize, NeoMutt->sub);

  editor_buffer_replace_part(wdata->state, i, buf_string(wdata->buf));
  if (rc != 1)
    return FR_CONTINUE;

  return FR_SUCCESS;
}

static int complete_label(struct EnterWindowData *wdata)
{
  const wchar_t *wbuf = editor_buffer_get_buffer(wdata->state);
  const int curpos = editor_buffer_get_cursor(wdata->state);
  const int lastchar = editor_buffer_get_lastchar(wdata->state);

  size_t i;
  for (i = curpos; (i > 0) && (wbuf[i - 1] != ',') && (wbuf[i - 1] != ':'); i--)
    ;

  for (; (i < lastchar) && (wbuf[i] == ' '); i++)
    ;

  mutt_mb_wcstombs(wbuf + i, curpos - i, wdata->buf);
  int rc = mutt_label_complete(wdata->buf->data, wdata->buf->dsize, wdata->tabs);
  editor_buffer_replace_part(wdata->state, i, buf_string(wdata->buf));
  if (rc != 1)
    return FR_CONTINUE;

  return FR_SUCCESS;
}

static int complete_pattern(struct EnterWindowData *wdata)
{
  const wchar_t *wbuf = editor_buffer_get_buffer(wdata->state);
  const int curpos = editor_buffer_get_cursor(wdata->state);

  size_t i = curpos;
  if (i && (wbuf[i - 1] == '~'))
  {
    if (dlg_select_pattern(wdata->buf->data, wdata->buf->dsize))
      editor_buffer_replace_part(wdata->state, i - 1, buf_string(wdata->buf));
    return FR_CONTINUE;
  }

  for (; (i > 0) && (wbuf[i - 1] != '~'); i--)
    ;

  if ((i > 0) && (i < curpos) && (wbuf[i - 1] == '~') && (wbuf[i] == 'y'))
  {
    i++;
    mutt_mb_wcstombs(wbuf + i, curpos - i, wdata->buf);
    int rc = mutt_label_complete(wdata->buf->data, wdata->buf->dsize, wdata->tabs);
    editor_buffer_replace_part(wdata->state, i, buf_string(wdata->buf));
    if (rc != 1)
      return FR_CONTINUE;
  }
  else
  {
    return FR_NO_ACTION;
  }

  return FR_SUCCESS;
}

static int complete_alias_query(struct EnterWindowData *wdata)
{
  const wchar_t *wbuf = editor_buffer_get_buffer(wdata->state);
  const int curpos = editor_buffer_get_cursor(wdata->state);

  size_t i = curpos;
  if (i != 0)
  {
    for (; (i > 0) && (wbuf[i - 1] != ','); i--)
      ;

    for (; (i < curpos) && (wbuf[i] == ' '); i++)
      ;
  }

  mutt_mb_wcstombs(wbuf + i, curpos - i, wdata->buf);
  query_complete(wdata->buf, NeoMutt->sub);
  editor_buffer_replace_part(wdata->state, i, buf_string(wdata->buf));

  return FR_CONTINUE;
}

static int complete_command(struct EnterWindowData *wdata)
{
  const wchar_t *wbuf = editor_buffer_get_buffer(wdata->state);
  const int curpos = editor_buffer_get_cursor(wdata->state);

  int rc = FR_SUCCESS;
  mutt_mb_wcstombs(wbuf, curpos, wdata->buf);
  size_t i = strlen(buf_string(wdata->buf));
  if ((i != 0) && (wdata->buf->data[i - 1] == '=') && (mutt_var_value_complete(wdata->buf->data, wdata->buf->dsize, i) != 0))
  {
    wdata->tabs = 0;
  }
  else if (mutt_command_complete(wdata->buf->data, wdata->buf->dsize, i, wdata->tabs) == 0)
  {
    rc = FR_ERROR;
  }

  editor_buffer_replace_part(wdata->state, 0, buf_string(wdata->buf));
  return rc;
}

static int complete_file_mbox(struct EnterWindowData *wdata)
{
  const wchar_t *wbuf = editor_buffer_get_buffer(wdata->state);
  const int curpos = editor_buffer_get_cursor(wdata->state);
  const int lastchar = editor_buffer_get_lastchar(wdata->state);

  int rc = FR_SUCCESS;
  mutt_mb_wcstombs(wbuf, curpos, wdata->buf);

  if ((!wdata->tempbuf && !lastchar) || (wdata->tempbuf && (wdata->templen == lastchar) && (memcmp(wdata->tempbuf, wbuf, lastchar * sizeof(wchar_t)) == 0)))
  {
    mutt_select_file(wdata->buf->data, wdata->buf->dsize, ((wdata->flags & MUTT_COMP_FILE_MBOX) ? MUTT_SEL_FOLDER : MUTT_SEL_NO_FLAGS) | (wdata->multiple ? MUTT_SEL_MULTI : MUTT_SEL_NO_FLAGS), wdata->m, wdata->files, wdata->numfiles);
    if (!buf_is_empty(wdata->buf))
    {
      mutt_pretty_mailbox(wdata->buf->data, wdata->buf->dsize);
      if (!wdata->pass)
        mutt_hist_add(wdata->hclass, buf_string(wdata->buf), true);
      wdata->done = true;
      return FR_SUCCESS;
    }

    return FR_CONTINUE;
  }

  if (mutt_complete(wdata->buf->data, wdata->buf->dsize) == 0)
  {
    wdata->templen = lastchar;
    mutt_mem_realloc(&wdata->tempbuf, wdata->templen * sizeof(wchar_t));
    memcpy(wdata->tempbuf, wbuf, wdata->templen * sizeof(wchar_t));
  }
  else
  {
    return FR_ERROR;
  }
  editor_buffer_replace_part(wdata->state, 0, buf_string(wdata->buf));
  return rc;
}

static int complete_nm_query(struct EnterWindowData *wdata)
{
  const wchar_t *wbuf = editor_buffer_get_buffer(wdata->state);
  const int curpos = editor_buffer_get_cursor(wdata->state);

  int rc = FR_SUCCESS;
  mutt_mb_wcstombs(wbuf, curpos, wdata->buf);
  size_t len = strlen(buf_string(wdata->buf));
  if (!mutt_nm_query_complete(wdata->buf->data, wdata->buf->dsize, len, wdata->tabs))
    rc = FR_ERROR;

  editor_buffer_replace_part(wdata->state, 0, buf_string(wdata->buf));
  return rc;
}

static int complete_nm_tag(struct EnterWindowData *wdata)
{
  const wchar_t *wbuf = editor_buffer_get_buffer(wdata->state);
  const int curpos = editor_buffer_get_cursor(wdata->state);

  int rc = FR_SUCCESS;
  mutt_mb_wcstombs(wbuf, curpos, wdata->buf);
  if (!mutt_nm_tag_complete(wdata->buf->data, wdata->buf->dsize, wdata->tabs))
    rc = FR_ERROR;

  editor_buffer_replace_part(wdata->state, 0, buf_string(wdata->buf));
  return rc;
}
