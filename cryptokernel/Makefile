# Author: Dmitry Kukovinets (d1021976@gmail.com)

# This is simple makefile for build cryptokernel

# Sources
SOURCES_CPP = \
	cryptokernel.cpp \
	types.cpp \
	generator/generator.cpp \
	generator/dev_random.cpp

# Tests
TEST_SOURCES_CPP = \
	test_dev_random.cpp \
	test_random_map.cpp \
	test_double_map.cpp \
	test_cryptokernel.cpp


# Compiler
GPP = g++ -Wall -std=c++11

# Objects and target files
OBJECTS = $(SOURCES_CPP:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES_CPP:.cpp=.o)
TEST_TARGETS = $(basename $(TEST_SOURCES_CPP))

# Targets
.PHONY: all clean

all: $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TEST_TARGETS)


# Objects compilation
%.o: %.cpp
	$(GPP) -c -o "$@" "$<"


# Tests targets
tests: $(TEST_TARGETS)

$(TEST_TARGETS): %: %.o $(OBJECTS)
	$(GPP) -o "$@" $^

$(TEST_OBJECTS): %.o: %.cpp
	$(GPP) -c -o "$@" "$<"
