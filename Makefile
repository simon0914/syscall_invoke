
# this makefile will be read by nmake on windows
# GNU make on linux will read the GNUmakefile

BIN = syscall.exe
OBJ = syscall.obj syscall_.obj

all: $(BIN)

clean:
	del $(BIN) $(OBJ)

$(BIN): $(OBJ)
	cl.exe /Fe$(BIN) $(OBJ)

syscall.obj: syscall.c
	cl.exe /c syscall.c

syscall_.obj: syscall_win10_64.asm syscall_win7_32.asm
	REM note that this is a horrible idea.
	ml64.exe /Fo $@ /c syscall_win10_64.asm || ml.exe /Fo $@ /c syscall_win7_32.asm
