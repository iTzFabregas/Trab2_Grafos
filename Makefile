CC=g++					# C compiler
CFLAGS=-I. -g -Wall
DEPS= lib/*.h	
OBJ = src/*.cpp

all: $(OBJ)
	@$(CC) -o prog $^ $(CFLAGS) $(LIBS)

o: $(OBJ) $(DEPS)
	@$(CC) -c -o $@ $< $(CFLAGS)

run:
	@./prog $${SIZE} $${QTT};

clean:
	@rm -f *.o prog *.zip

zip:
	zip trab2.zip Makefile ./src/*.cpp ./lib/*.hpp