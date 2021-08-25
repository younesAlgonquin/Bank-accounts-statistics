CC = gcc
CC_FLAGS = -g -Wall -ansi -pedantic
OUT_EXE = part2
FILES = Lab05_Part2.o

build:$(FILES)
	$(CC) $(CC_FLAGS) -o $(OUT_EXE) $(FILES)

clean:
	rm $(OUT_EXE) *.o *.exe