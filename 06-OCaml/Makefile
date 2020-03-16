
all:
	dune build

test:
	dune runtest

check: test

clean:
	dune clean

INPUT = "./input.txt"

run: all
	echo "trying to run solutions to 06a, then 06b"
	./main_a.exe < $(INPUT) && \
	./main_b.exe < $(INPUT)
	

.PHONY: check test all clean run