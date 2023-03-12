#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint16_t TokenFlags;          ///< Flags for mutt_extract_token(), e.g. #TOKEN_EQUAL
#define TOKEN_NO_FLAGS            0   ///< No flags are set
#define TOKEN_EQUAL         (1 << 0)  ///< Treat '=' as a special
#define TOKEN_CONDENSE      (1 << 1)  ///< ^(char) to control chars (macros)
#define TOKEN_SPACE         (1 << 2)  ///< Don't treat whitespace as a term
#define TOKEN_QUOTE         (1 << 3)  ///< Don't interpret quotes
#define TOKEN_PATTERN       (1 << 4)  ///< ~%=!| are terms (for patterns)
#define TOKEN_COMMENT       (1 << 5)  ///< Don't reap comments
#define TOKEN_SEMICOLON     (1 << 6)  ///< Don't treat ; as special
#define TOKEN_BACKTICK_VARS (1 << 7)  ///< Expand variables within backticks
#define TOKEN_NOSHELL       (1 << 8)  ///< Don't expand environment variables
#define TOKEN_QUESTION      (1 << 9)  ///< Treat '?' as a special
#define TOKEN_PLUS          (1 << 10) ///< Treat '+' as a special
#define TOKEN_MINUS         (1 << 11) ///< Treat '-' as a special

#define IS_SPACE(ch) isspace((unsigned char) ch)

#define MoreArgs(ch) ((ch != ';') && (ch != '#'))

#define MoreArgsF(ch,flags) (                       \
    (!IS_SPACE(ch) || ((flags) & TOKEN_SPACE)) &&   \
    ((ch != '#') ||  ((flags) & TOKEN_COMMENT)) &&  \
    ((ch != '+') || !((flags) & TOKEN_PLUS)) &&     \
    ((ch != '-') || !((flags) & TOKEN_MINUS)) &&    \
    ((ch != '=') || !((flags) & TOKEN_EQUAL)) &&    \
    ((ch != '?') || !((flags) & TOKEN_QUESTION)) && \
    ((ch != ';') || ((flags) & TOKEN_SEMICOLON)) && \
    (!((flags) & TOKEN_PATTERN) || strchr("~%=!|", ch)))

bool parse_more_args(char ch)
{
    (ch != ';') && (ch != '#');
  ((ch != '#') || (flags & TOKEN_COMMENT))
  ((ch != ';') || (flags & TOKEN_SEMICOLON))

  (!IS_SPACE(ch) || (flags & TOKEN_SPACE))


  ((ch != '+') || !(flags & TOKEN_PLUS))
  ((ch != '-') || !(flags & TOKEN_MINUS))
  ((ch != '=') || !(flags & TOKEN_EQUAL))
  ((ch != '?') || !(flags & TOKEN_QUESTION))
  (!(flags & TOKEN_PATTERN) || strchr("~%=!|", ch))


      if ((IS_SPACE(ch) && !(flags & TOKEN_SPACE)) ||
          ((ch == '#') && !(flags & TOKEN_COMMENT)) ||
          ((ch == '+') && (flags & TOKEN_PLUS)) ||
          ((ch == '-') && (flags & TOKEN_MINUS)) ||
          ((ch == '=') && (flags & TOKEN_EQUAL)) ||
          ((ch == '?') && (flags & TOKEN_QUESTION)) ||
          ((ch == ';') && !(flags & TOKEN_SEMICOLON)) ||
          ((flags & TOKEN_PATTERN) && strchr("~%=!|", ch)))
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    return 1;

  static const int flags[] = {
    TOKEN_NO_FLAGS,  TOKEN_EQUAL,    TOKEN_SPACE, TOKEN_PATTERN, TOKEN_COMMENT,
    TOKEN_SEMICOLON, TOKEN_QUESTION, TOKEN_PLUS,  TOKEN_MINUS, -1,
  };

  static const char *names[] = {
    "NO_FLAGS",  "EQUAL",    "SPACE", "PATTERN", "COMMENT",
    "SEMICOLON", "QUESTION", "PLUS",  "MINUS",   "",
  };

  const bool ma = MoreArgs(argv[1][0]);
  if (ma)
    printf("MA '\033[1;32m%s\033[0m'\n", argv[1]);
  else
    printf("MA '\033[1;31m%s\033[0m'\n", argv[1]);

  for (int i = 0; flags[i] >= 0; i++)
  {
    const bool mf = MoreArgsF(argv[1][0], flags[i]);
    if (mf)
      printf("MF '\033[1;32m%s\033[0m' - %s\n", argv[1], names[i]);
    else
      printf("MF '\033[1;31m%s\033[0m' - %s\n", argv[1], names[i]);
  }
  printf("\n");

  return 0;
}
