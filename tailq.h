#define TAILQ_HEAD_INITIALIZER(head)                                           \
  {                                                                            \
    NULL, &(head).tqh_first                                                    \
  }

#define TAILQ_HEAD(name, type)                                                 \
  struct name                                                                  \
  {                                                                            \
    struct type *tqh_first; /* first element */                                \
    struct type **tqh_last; /* addr of last next element */                    \
  }

#define TAILQ_ENTRY(type)                                                      \
  struct                                                                       \
  {                                                                            \
    struct type *tqe_next;  /* next element */                                 \
    struct type **tqe_prev; /* address of previous next element */             \
  }

#define TAILQ_FIRST(head) ((head)->tqh_first)

#define TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)

#define TAILQ_REMOVE(head, elm, field)                                         \
  do                                                                           \
  {                                                                            \
    if ((TAILQ_NEXT((elm), field)) != NULL)                                    \
      TAILQ_NEXT((elm), field)->field.tqe_prev = (elm)->field.tqe_prev;        \
    else                                                                       \
    {                                                                          \
      (head)->tqh_last = (elm)->field.tqe_prev;                                \
    }                                                                          \
    *(elm)->field.tqe_prev = TAILQ_NEXT((elm), field);                         \
  } while (0)

#define TAILQ_INSERT_TAIL(head, elm, field)                                    \
  do                                                                           \
  {                                                                            \
    TAILQ_NEXT((elm), field) = NULL;                                           \
    (elm)->field.tqe_prev = (head)->tqh_last;                                  \
    *(head)->tqh_last = (elm);                                                 \
    (head)->tqh_last = &TAILQ_NEXT((elm), field);                              \
  } while (0)

#define TAILQ_FOREACH(var, head, field)                                        \
  for ((var) = TAILQ_FIRST((head)); (var); (var) = TAILQ_NEXT((var), field))

