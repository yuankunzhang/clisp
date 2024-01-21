clisp: main.c clisp.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
