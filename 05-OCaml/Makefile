
all:
	dune build

test:
	dune runtest

check: test

clean:
	dune clean

INPUT = "./input-05.txt"

run: all
	echo "trying to run solutions to 05a, then 05b"
	./main_a.exe $(INPUT) && \
	./main_b.exe $(INPUT)
	

.PHONY: check test all clean run