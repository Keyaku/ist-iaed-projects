CC = gcc
CFLAGS= -g -Wall -Wextra -ansi -pedantic
OBJ = bld/main.o bld/node.o bld/hash.o
EXE = proj

# Rules
all: $(EXE)

$(EXE): $(OBJ)
	$(LINK.c) -o $@ $^

bld/main.o: src/main.c src/main.h src/node.h
bld/node.o: src/node.c src/main.h src/node.h src/hash.h
bld/hash.o: src/hash.c src/hash.h src/node.h

bld/%.o:
	$(LINK.c) -c $< -o $@

zip:
	cd src && zip proj2.zip *.c *.h

clean:
	rm -f bld/*.o $(EXE)
