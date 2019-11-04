CC=g++
FLAGS=-g

all: 
	$(CC) $(FLAGS) main.cpp  um.cpp -o b.out

clean:
	rm -f *.out
