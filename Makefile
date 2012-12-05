CC = g++
CFLAGS = -Wall -pedantic -fast -O3
OFLAGS = -Wall -pedantic -c -fast -O3

all: principal

principal: random input net plot growingng growingng_classify list linkedlist iterator
	$(CC) $(CFLAGS)  object/iterator.o object/list.o object/linkedlist.o object/random.o object/input.o object/net.o object/plot.o object/growingng.o object/growingng_classify.o src/main.cpp -o bin/growingng

iterator: src/iterator.cpp
	$(CC) $(OFLAGS) -o object/iterator.o src/iterator.cpp

linkedlist: src/linkedlist.cpp
	$(CC) $(OFLAGS) -o object/linkedlist.o src/linkedlist.cpp

list: src/list.cpp
	$(CC) $(OFLAGS) -o object/list.o src/list.cpp

random: src/random.c
	$(CC) $(OFLAGS) -o object/random.o src/random.c

input: src/input.c
	$(CC) $(OFLAGS) -o object/input.o src/input.c

lambda: src/lambda.cpp
	$(CC) $(OFLAGS) -o object/lambda.o src/lambda.cpp

net: src/net.cpp
	$(CC) $(OFLAGS) -o object/net.o src/net.cpp

plot: src/plot.c
	$(CC) $(OFLAGS) -o object/plot.o src/plot.c

growingng: src/growingng.cpp
	$(CC) $(OFLAGS) -o object/growingng.o src/growingng.cpp

growingng_classify: src/growingng_classify.cpp
	$(CC) $(OFLAGS) -o object/growingng_classify.o src/growingng_classify.cpp

clean:
	rm object/*.o;
