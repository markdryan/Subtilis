DEPS = stream.h lexer.h config.h error.h utils.h keywords.h buffer.h
OBJ = stream.o lexer.o error.o utils.o keywords.o buffer.o

CFLAGS ?= -O3 -Wall -Werror

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

basicc: compiler.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

lexer_test: lexer_test.o $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm basicc *.o lexer_test

check: lexer_test
	./lexer_test