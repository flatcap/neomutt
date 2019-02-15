#define CHECK_ACL(aclbit, action)
  if (!Context || !(Context->mailbox->rights & aclbit))
  {
    mutt_flushinp();
    /* L10N: %s is one of the CHECK_ACL entries below. */
    mutt_error(_("%s: Operation not permitted by ACL"), action);
    break;
  }

#define CHECK_ATTACH
  if (OptAttachMsg)
  {
    mutt_flushinp();
    mutt_error(_(Function_not_permitted_in_attach_message_mode));
    break;
  }

#define CHECK_READONLY
  if (!Context || Context->mailbox->readonly)
  {
    mutt_flushinp();
    mutt_error(_(Mailbox_is_read_only));
    break;
  }


CHECK_MODE

  if (!(x))
  {
    mutt_flushinp();
    mutt_error(_(Not_available_in_this_menu));
    break;
  }

struct Pager
{
  struct Context *ctx;    /**< current mailbox */
  struct Email *email;    /**< current message */
  struct Body *body;      /**< current attachment */
  FILE *fp;               /**< source stream */
  struct AttachCtx *actx; /**< attachment information */
};

#define IsAttach(x)    (x && (x)->body)
#define IsMsgAttach(x) (x && (x)->fp && (x)->body && (x)->body->email)
#define IsEmail(x)     (x && (x)->email && !(x)->body)

CHECK_MODE
	IsEmail
	IsEmail && !IsAttach
	IsEmail || IsAttach
	IsEmail || IsMsgAttach

IsEmail(Pager)

	(Pager && Pager->email && !Pager->body)

IsEmail(Pager) && !IsAttach(Pager)

	(Pager && Pager->email && !Pager->body) && !(Pager && Pager->body)

IsEmail(Pager) || IsAttach(Pager)

	(Pager && Pager->email && !Pager->body) || (Pager && Pager->body)

IsEmail(Pager) || IsMsgAttach(Pager)

	(Pager && Pager->email && !Pager->body) || (Pager && Pager->body && Pager->body->email && Pager->fp)

