# $@ is the left side of X: Y
# $^ is the right side of X: Y
# $< is the first item on the right side of X: Y

# get LFLAGS from `pkg-config --cflags --libs check`


OBJS = solution.o main.o

CC = gcc

CFLAGS = -Wall -O2
LFLAGS = -pthread -lcheck

%.o: src/%.c
	$(CC) $(CFLAGS) -c $<

solution: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

test.o: tests/test.c
	$(CC) -o $@ $(CFLAGS) -c $<

test: test.o solution.o
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	rm *.o