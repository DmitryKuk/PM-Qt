# Author: Dmitry Kukovinets (d1021976@gmail.com)

# This is simple makefile for build cryptokernel

# Sources
SOURCES_CPP = \
	cryptokernel.cpp \
	types.cpp \
	generator/generator.cpp \
	generator/dev_random.cpp

# Static library
TARGET_LIB = cryptokernel.a

# Tests
TEST_SOURCES_CPP = \
	test_dev_random.cpp \
	test_random_map.cpp \
	test_double_map.cpp \
	test_cryptokernel.cpp


# Compiler and library archiever
GPP = g++ -Wall -std=c++11
AR = ar cr

# Objects and target files
OBJECTS = $(SOURCES_CPP:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES_CPP:.cpp=.o)
TEST_TARGETS = $(basename $(TEST_SOURCES_CPP))

# Targets
.PHONY: all clean

all: $(TARGET_LIB)

clean:
	rm -f "$(TARGET_LIB)" $(TEST_OBJECTS) $(TEST_TARGETS)


$(TARGET_LIB): $(TARGET_LIB)($(OBJECTS))

$(TARGET_LIB)(%.o): %.o
	$(AR) "$@" "$<"

# Objects compilation
%.o: %.cpp
	$(GPP) -c -o "$@" "$<"


# Tests targets
tests: $(TEST_TARGETS)

$(TEST_TARGETS): %: %.o $(TARGET_LIB)
	$(GPP) -o "$@" $^

$(TEST_OBJECTS): %.o: %.cpp
	$(GPP) -c -o "$@" "$<"
