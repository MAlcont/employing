TARGET = bin/dbview

SRC        = $(wildcard src/*.c)
EXTRA_SRC  = $(wildcard extra_tasks/*.c)

OBJ        = $(patsubst src/%.c,         obj/%.o, $(SRC)) \
             $(patsubst extra_tasks/%.c, obj/%.o, $(EXTRA_SRC))

run: clean default
	./$(TARGET) -f ./mynewdb.db -n
	./$(TARGET) -f ./mynewdb.db -a "Timmy.H, 123 Sheshire Lane, 1"
	./$(TARGET) -f ./mynewdb.db -a "Tommy.A, 321 Beeshire Lane, 12"
	./$(TARGET) -f ./mynewdb.db -a "Tammy.O, 1 Lane, 123"
	./$(TARGET) -f ./mynewdb.db -a "Ronny.B, 2 Chinguarito, 1234"
	./$(TARGET) -f ./mynewdb.db -a "Bonny.B, 265 Wajaud, 12345"

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude

obj/%.o : extra_tasks/%.c
	gcc -c $< -o $@ -Iinclude
