#include "main.h"

struct cur_tok _cur;
int _cstack[100];
int _csp = 0;

/**
 * A Variable that keeps track of what line of input the lexer is on, you can
 * use this to print more useful error messages and help you debug your parser.
 */
int _line = 1;

FILE *input = NULL;

void startlex(FILE *fp)
{
  input = fp;
  _cur.tok = lex(_cur.buf, &_cur.val);
}

// Gets the next character, returns EOF (-1) on end of input, pops from the
// unget character stack first if it's not empty.
int get(void)
{
  int c;
  if (_csp) c = _cstack[--_csp];
  else c = fgetc(input);
  if (c == '\n') _line++;
  return c;
}

// Unget a character, pushes it to the unget character stack:
void unget(int c)
{
  if (c == '\n') _line--;
  if (c != EOF) _cstack[_csp++] = c;
}

// If the next character matches c, then move past it and return TRUE,
// otherwise put it back and return FALSE
bool next(int c)
{
  int ch = get();
  if (ch == c) return TRUE;
  unget(ch);
  return FALSE;
}

/**
 * JSON strings are always double-quoted and may contain 0 or more characters
 * with the following allowed for characters:
 * char	  : any-Unicode-character-except-"-or-\-or-control-character
 *	  | \" | \\ | \/ | \b | \f | \n | \r | \t| \u four-hex-digits 
 * For this assignment you do not need to support \u hex sequences, although
 * you may.
 */

token_t lexstring(char *word)
{
  int c, wp = 0, escape = FALSE;

  while ((c = get()) != EOF) {
    if (!escape && c == '\"') break;
    if (c == '\\' && !escape) {
      escape = TRUE;
      continue;
    }
    if (escape) {
      switch(c) {
	case '\"':
	case '\\':
	case '/':
	case '\n':
	  break;
	case 'b': c = '\b'; break;
	case 'f': c = '\f'; break;
	case 'n': c = '\n'; break;
	case 'r': c = '\r'; break;
	case 't': c = '\t'; break;
      }
      escape = FALSE;
    }
    // Add support for \x sequences
    word[wp++] = c;
  }
  word[wp] = '\0';
  return T_STRING;
}

static struct keyword {
  char *name;
  token_t tval;
} keywords[] = {
  {"true", T_TRUE},
  {"false", T_FALSE},
  {"null", T_NULL},
  {NULL, T_UNKNOWN}
};

static token_t lex(char *word, double *val)
{
  char stop[] = " \t\n[]{},:\"";
  int wp = 0;
  char c = get(), *end;
  word[wp] = '\0';

  while (isspace(c)) c = get();

  switch(c) {
    case '\0':
    case EOF: return T_EOI;
    case '"': return lexstring(word);
    case '[': return T_OBRAC;
    case ']': return T_CBRAC;
    case '{': return T_OCBRACE;
    case '}': return T_CCBRACE;
    case ',': return T_COMMA;
    case ':': return T_COLON;
    default:
      do {
	word[wp++] = c;
	c = get();
      } while (c != '\0' && c != EOF && strchr(stop,c) == NULL );
      unget(c);
      word[wp] = '\0';
      if (isdigit(word[0]) || word[0] == '-' || word[0] == '.') {
	*val = strtod(word, &end);
	if (*end != '\0') {
	  printf("Malformed number\n");
	  return T_UNKNOWN;
	}
	return T_NUMBER;
      }
      for(int i=0; keywords[i].name != NULL; i++)
	if (strcasecmp(word, keywords[i].name) == 0) return keywords[i].tval;
      printf("Lexed unknown token '%s'\n", word);
      return T_UNKNOWN;
  }
  // Not reached
  return T_EOI;
}

/**
 * Matches a given token or dies if it doesn't match
 */
void match(token_t tok)
{
  char *p;

  if (tok == _cur.tok) _cur.tok = lex(_cur.buf, &_cur.val);
  else {
    printf("Syntax error on line %d\n", _line);
    exit(1);
  }
}

/**
 * Accept (match) the current if it matches (and returns true (1))
 * otherwise indicates failure to match (returns 0), but is not fatal.
 */
int accept(token_t tok)
{
  if (_cur.tok == tok) {
    match(tok);
    return 1;
  }
  return 0;
}
