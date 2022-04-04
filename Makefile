CFLAGS := -Wall -Wextra -Iinclude

.PHONY: all
all: converter

converter: obj/main.o obj/converter.o obj/exceptions.o
	g++ -std=c++20 obj/main.o obj/converter.o obj/exceptions.o -o converter.out

obj:
	mkdir obj

obj/main.o: src/main.cpp include/exceptions.h include/converter.h | obj
	g++ -std=c++20 -c $(CFLAGS) src/main.cpp -o obj/main.o

obj/exceptions.o: src/exceptions.cpp include/exceptions.h  | obj
	g++ -std=c++20 -c $(CFLAGS) src/exceptions.cpp -o obj/exceptions.o

obj/converter.o: src/converter.cpp include/converter.h include/exceptions.h  | obj
	g++ -std=c++20 -c $(CFLAGS) src/converter.cpp -o obj/converter.o


.PHONY: clean
clean:
	rm -rf obj converter.out


