# JSON_parser


parse.c makes a complete JSON parser that generates a complete JSON "value" that will be printed by the main.c main function.  


json.h:  Contains the data structures for a JSON value.  Comments include the
         JSON grammar.

lex.c:   The lexer.  The lexer maintains the _line global variable which should
         indicate what line of the input the lexer is currently on, to help in
         debugging parser.

print.c: Functions to print the value.

main.c:  Loads a file, initializes the lexer and asks the parser to parse the
         input.
