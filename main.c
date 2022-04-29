#include "main.h"

extern int _line;

value_t *json_decode_file(char *filename)
{
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("fopen");
    return NULL;
  }    
  
  startlex(fp);
  value_t *v = parse_value();
  fclose(fp);

  return v;
}

int main(int argc, char *argv[])
{
  value_t *v;

  if (argc < 2) {
    printf("Usage: json <file>\n");
    exit(1);
  }
  v = json_decode_file(argv[1]);
  if (v == NULL) {
    printf("Error parsing input on or before line %d.\n", _line);
    exit(1);
  }

  print_value(v, 0);
  return 0;
}
