CC=gcc
CFLAGS=-ggdb
OBJS=lex.o parse.o print.o main.o

h7: $(OBJS)
	$(CC) -o h7 $(OBJS)

lex.o: lex.c main.h json.h
	$(CC) $(CFLAGS) -c lex.c

parse.o: parse.c main.h json.h
	$(CC) $(CFLAGS) -c parse.c

print.o: print.c main.h json.h
	$(CC) $(CFLAGS) -c print.c

main.o: main.c main.h json.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o h7
