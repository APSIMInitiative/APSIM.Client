CC=gcc
CFLAGS= -Wall -O3 -fPIC
LDFLAGS=-lapsimclient
RM= rm -f
INCLUDES=src/
HEADERS={client,replacement}.h
INST=/usr/local
CHECKFLAGS=`pkg-config --cflags --libs check`

.PHONY: all clean install uninstall
all: libapsimclient.so example unittests
clean:
	$(RM) libapsimclient.so example unittests
install:
	install -m 755 libapsimclient.so $(INST)/lib
	install -m 644 src/$(HEADERS) $(INST)/include
uninstall:
	$(RM) $(INST)/lib/libapsimclient.so
	$(RM) $(INST)/include/$(HEADERS)

libapsimclient.so: src/client.c src/replacement.c
	$(CC) -I $(INCLUDES) $(CFLAGS) -shared $^ -o $@
example: examples/main.c libapsimclient.so
	$(CC) -L. -I $(INCLUDES) $(CFLAGS) $(LDFLAGS) $^ -o $@
unittests: tests/tests.c tests/test_replacements.c tests/test_client.c libapsimclient.so
	$(CC) -L. -I $(INCLUDES) $(CFLAGS) -pthread $(LDFLAGS) $^ $(CHECKFLAGS) -o $@
