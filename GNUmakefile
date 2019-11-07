
# this makefile will be read by GNU make on linux
# nmake on windows will read the Makefile

BIN = syscall
OBJ = syscall.o

all: $(BIN)

clean:
	$(RM) $(BIN) $(OBJ)

$(BIN): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -o $@ $< -c $(CFLAGS)
