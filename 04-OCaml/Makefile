
all:
	dune build

test:
	dune runtest

check: test

clean:
	dune clean

RANGE_FROM = 171309
RANGE_TO = 643603

run: all
	echo "trying to run solutions to 04a, then 04b"
	./main_a.exe $(RANGE_FROM) $(RANGE_TO) && \
	./main_b.exe $(RANGE_FROM) $(RANGE_TO)
	

.PHONY: check test all clean run