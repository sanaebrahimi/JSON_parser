#include "json.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define K	1024

typedef enum { TRUE=1, FALSE=0 } bool;

typedef enum {
  T_STRING, T_NUMBER, T_OBRAC, T_CBRAC, T_OCBRACE, T_CCBRACE, T_COMMA, T_COLON,
  T_TRUE, T_FALSE, T_NULL,
  T_EOI, T_UNKNOWN
} token_t;

struct cur_tok {
  token_t tok;
  char buf[K];
  double val;
};

// lex.c:
void startlex(FILE *fp);
static token_t lex(char *word, double *val);
void match(token_t tok);
int accept(token_t tok);

// print.c:
int print_value(value_t *v, int depth);

// parse.c:
value_t *parse_value(void);
