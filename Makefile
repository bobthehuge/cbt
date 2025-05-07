CC = clang
GDB = gf
CRELFLAGS = -std=c99 -O3
CDEVFLAGS = -std=c99 -g
LDLIBS =

SRC = `find . -name '*.c' ! -path './samples/*'`
OBJ = `find . -name '*.o'`
EXE = cbtc

all: setrel comp

comp:
	$(CC) -o $(EXE) $(SRC) $(CFLAGS) $(LDLIBS)
rel: setrel comp
dev: setdev comp
run:
	./$(EXE)
gdb: dev
	$(GDB) ./$(EXE)
memcheck: dev
	valgrind --leak-check=full -s ./$(EXE) ../sample.ml
setdev:
	$(eval CFLAGS := $(CDEVFLAGS))
setrel:
	$(eval CFLAGS := $(CRELFLAGS))

.PHONY: setdev setrel debug mop clean

mop:
	$(RM) $(OBJ)
	$(RM) $(SSA)
	$(RM) $(ASM)

clean: mop
	$(RM) $(EXE)
