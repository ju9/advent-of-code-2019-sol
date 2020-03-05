
all:
	dune build

test:
	dune runtest

check: test

clean:
	dune clean

INPUT = "./input-05.txt"

run: all
	echo "Trying to run solutions to 05."
	echo "The input code for part 1 is '1'. For part 2 it is '5'."
	./main.exe $(INPUT)
	

.PHONY: check test all clean run