CFLAGS := -Wall -Wextra

.PHONY: all
all: program

program: obj/main.o obj/mixed.o
	g++ obj/main.o obj/mixed.o -o program

obj:
	mkdir obj

obj/main.o: main.cpp mixed.h | obj
	g++ -c $(CFLAGS) main.cpp -o obj/main.o

obj/mixed.o: mixed.cpp mixed.h | obj
	g++ -c $(CFLAGS) mixed.cpp -o obj/mixed.o


.PHONY: clean
clean:
	rm -rf obj result


