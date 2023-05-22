CC = gcc
CFLAGS = -Wall -g

SRC = main.c lista.c tabhash.c
EXE = tema1 

build:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)

run:
	./$(EXE)

.PHONY: clean

clean:
	rm -rf $(EXE)
