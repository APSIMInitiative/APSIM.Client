CC=gcc
CFLAGS= -Wall -O3 -fPIC
LDFLAGS=-lapsimclient
RM= rm -f
INCLUDES=src/
HEADERS={client,replacement}.h
INST=/usr/local
CHECKFLAGS=`pkg-config --cflags --libs check`
OUTPUT=libapsimclient.so

.PHONY: all clean install uninstall
all: $(OUTPUT) example unittests
clean:
	$(RM) $(OUTPUT) example unittests
install:
	install -m 755 $(OUTPUT) $(INST)/lib/$(OUTPUT)
	install -m 644 src/replacement.h src/apsimclient.h $(INST)/include/replacement.h
	install -m 644 src/apsimclient.pc $(INST)/lib/pkgconfig/apsimclient.pc
uninstall:
	$(RM) $(INST)/lib/$(OUTPUT)
	$(RM) $(INST)/include/replacement.h $(INST)/include/apsimclient.h

$(OUTPUT): src/client.c src/replacement.c
	$(CC) -I $(INCLUDES) $(CFLAGS) -shared $^ -o $@
example: examples/main.c $(OUTPUT)
	$(CC) -L. -I $(INCLUDES) $(CFLAGS) $(LDFLAGS) $^ -o $@
unittests: tests/tests.c tests/test_replacements.c tests/test_client.c $(OUTPUT)
	$(CC) -L. -I $(INCLUDES) $(CFLAGS) -pthread $(LDFLAGS) $^ $(CHECKFLAGS) -o $@
