#include "main.h"

bool pre_indent = TRUE;

/**
 * char	  : any-Unicode-character-except-"-or-\-or-control-character
 *	  | \" | \\ | \/ | \b | \f | \n | \r | \t| \u four-hex-digits 
 */
int print_string(char *s)
{
  printf("\"");
  for(int i=0; s[i]; i++) {
    switch(s[i]) {
      case '"':
      case '\\':
      case '/':
	printf("\\%c",s[i]);
	break;
      case '\b': printf("\\b"); break;
      case '\f': printf("\\f"); break;
      case '\n': printf("\\n"); break;
      case '\r': printf("\\r"); break;
      case '\t': printf("\\t"); break;
      default:
	if (iscntrl(s[i])) printf("\\u%04o", s[i]);
	else printf("%c", s[i]);
    }
  }
  return printf("\"");
}


int pprint(char *s, int depth)
{
  if (pre_indent) {
    pre_indent = FALSE;
    return printf("%*s%s", depth*2, "", s);
  }
  return printf("%s", s);
}

int print_array(array_t *a, int depth)
{
  printf("[");
  for(int i=0; i < a->length; i++) {
    print_value(&(a->a[i]), depth);
    if (i+1 < a->length) printf(", ");
  }
  return printf("]");
}

int print_pair(pair_t *p, int depth)
{
  printf("%*s", depth*2, "");
  print_string(p->name);
  printf(":");
  pre_indent = FALSE;
  print_value(&(p->v),depth);
}

int print_object(pair_t *o, int depth)
{
  pprint("{\n",depth);
  pre_indent = TRUE;
  while(o) {
    print_pair(o, depth+1);
    if (o->next) printf(",\n");
    else printf("\n");
    pre_indent = TRUE;
    o = o->next;
  }
  return pprint("}", depth);
}

int print_value(value_t *v, int depth)
{
  switch(v->type) {
    case DT_NUMBER:	return printf("%lg", v->n);
    case DT_TRUE:	return printf("true");
    case DT_FALSE:	return printf("false");
    case DT_NULL:	return printf("null");
    case DT_STRING:	return print_string(v->s);
    case DT_ARRAY:	return print_array(v->a, depth);
    case DT_OBJECT:	return print_object(v->o, depth);
    default:		return -1;
  }
}
