CC = g++
CFLAGS = -g -Wextra -pedantic -Wall
OBJ = src/*.cpp
DEPS = src/*.h

all: $(OBJ)
	@$(CC) -o prog $^ $(CFLAGS)

o: $(OBJ) $(DEPS)
	@$(CC) -c -o $@ $< $(CFLAGS)

run:
	@./prog $${SIZE} $${QTT};

clean:
	@rm -f *.o prog *.zip

zip:
	zip trab2.zip Makefile ./src/*.cpp ./src/*.hpp
