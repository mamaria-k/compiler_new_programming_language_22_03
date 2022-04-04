CFLAGS := -Wall -Wextra

.PHONY: all
all: program

program: obj/m.o obj/mixed.o
	g++ -std=c++20 obj/m.o obj/mixed.o -o program

obj:
	mkdir obj

obj/m.o: m.cpp mixed.h | obj
	g++ -std=c++20 -c $(CFLAGS) m.cpp -o obj/m.o

obj/mixed.o: mixed.cpp mixed.h | obj
	g++ -std=c++20 -c $(CFLAGS) mixed.cpp -o obj/mixed.o


.PHONY: clean
clean:
	rm -rf obj result


