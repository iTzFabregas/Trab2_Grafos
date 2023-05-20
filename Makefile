BIN = prog

SRCS = $(shell ls src/*.cpp)

########################################################

# Config flags
CC = g++
C_FLAGS = -Wall -Wextra -Wpedantic -ggdb

C_FLAGS += $(CFLAGS)
CPP_FLAGS += $(CPPFLAGS)
LD_FLAGS += $(LDFLAGS)

# Binarie-object dependencies
$(BIN) : $(SRCS:%.cpp=%.o)
	$(CC) $(LD_FLAGS) $^ -o $@

# Object-source dependencies
%.o : %.cpp
	$(CC) $(C_FLAGS) $(CPP_FLAGS) -c $< -o $@

# Object-header dependencies
deps = $(SRCS:%.cpp=%.d)
-include $(deps)

%.d : %.cpp
	$(CC) $(C_FLAGS) -MM -MT '$(<:%.cpp=%.o) $@' $< -o $@

.PHONY: all clean zip

all : $(BIN)

run: all
	@./$(BIN)

test:
	@run-cli test

zip: clean
	zip -r submission src/ Makefile

clean:
	rm -f submission.zip
	rm -f src/*.o
	rm -f src/*.d
	rm -f $(BIN)
