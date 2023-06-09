CC=cc
CFLAGS=-Wall -Wextra -Og -g3
INC=-Iinclude
LIB=-lm
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c, %.o, $(SRC))
BIN=dexcalc

default: $(BIN)

$(BIN): $(OBJ)
	@echo Linking DexCalc executable...
	@$(CC) $(CFLAGS) $^ -o $@ $(INC) $(LIB)
	@echo Deleting object files...
	@rm -f *.o
	@echo Success!

%.o: src/%.c
	@echo Compiling "'"$^"'"...
	@$(CC) $(CFLAGS) -c $^ $(INC)

clean:
	@rm -f $(BIN) $(OBJ)
	@echo Cleaned up build files.
