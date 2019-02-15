case OP_SORT_REVERSE:
  CHECK_MODE(IsEmail(extra))

case OP_BOUNCE_MESSAGE:
  CHECK_MODE(IsEmail(extra) || IsMsgAttach(extra))
  CHECK_ATTACH;

case OP_CREATE_ALIAS:
  CHECK_MODE(IsEmail(extra) || IsMsgAttach(extra));

case OP_DELETE:
  CHECK_MODE(IsEmail(extra));
  CHECK_READONLY;
  CHECK_ACL(MUTT_ACL_DELETE, _("Cannot delete message"));

case OP_MAIN_CLEAR_FLAG:
  CHECK_MODE(IsEmail(extra));
  CHECK_READONLY;

case OP_PURGE_THREAD:
  CHECK_MODE(IsEmail(extra));
  CHECK_READONLY;
  CHECK_ACL(MUTT_ACL_DELETE, _("Cannot delete messages"));

case OP_DISPLAY_ADDRESS:
  CHECK_MODE(IsEmail(extra) || IsMsgAttach(extra));

case OP_FLAG_MESSAGE:
  CHECK_MODE(IsEmail(extra));
  CHECK_READONLY;
  CHECK_ACL(MUTT_ACL_WRITE, "Cannot flag message");

case OP_PIPE:
  CHECK_MODE(IsEmail(extra) || IsAttach(extra));

case OP_PRINT:
  CHECK_MODE(IsEmail(extra) || IsAttach(extra));

case OP_MAIL:
  CHECK_MODE(IsEmail(extra) && !IsAttach(extra));
  CHECK_ATTACH;

case OP_POST:
  CHECK_MODE(IsEmail(extra) && !IsAttach(extra));
  CHECK_ATTACH;

case OP_FORWARD_TO_GROUP:
  CHECK_MODE(IsEmail(extra) || IsMsgAttach(extra));
  CHECK_ATTACH;

case OP_FOLLOWUP:
  CHECK_MODE(IsEmail(extra) || IsMsgAttach(extra));
  CHECK_ATTACH;

case OP_REPLY:
  CHECK_MODE(IsEmail(extra) || IsMsgAttach(extra));
  CHECK_ATTACH;

case OP_RECALL_MESSAGE:
  CHECK_MODE(IsEmail(extra) && !IsAttach(extra));
  CHECK_ATTACH;

case OP_GROUP_REPLY:
  CHECK_MODE(IsEmail(extra) || IsMsgAttach(extra));
  CHECK_ATTACH;

case OP_LIST_REPLY:
  CHECK_MODE(IsEmail(extra) || IsMsgAttach(extra));
  CHECK_ATTACH;

case OP_FORWARD_MESSAGE:
  CHECK_MODE(IsEmail(extra) || IsMsgAttach(extra));
  CHECK_ATTACH;

case OP_TAG:
  CHECK_MODE(IsEmail(extra));

case OP_TOGGLE_NEW:
  CHECK_MODE(IsEmail(extra));
  CHECK_READONLY;
  CHECK_ACL(MUTT_ACL_SEEN, _("Cannot toggle new"));

case OP_UNDELETE:
  CHECK_MODE(IsEmail(extra));
  CHECK_READONLY;
  CHECK_ACL(MUTT_ACL_DELETE, _("Cannot undelete message"));

case OP_UNDELETE_SUBTHREAD:
  CHECK_MODE(IsEmail(extra));
  CHECK_READONLY;
  CHECK_ACL(MUTT_ACL_DELETE, _("Cannot undelete messages"));

case OP_VIEW_ATTACHMENTS:
  CHECK_MODE(IsEmail(extra));

case OP_MAIL_KEY:
  CHECK_MODE(IsEmail(extra));
  CHECK_ATTACH;

