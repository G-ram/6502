CXX = clang++

INCLUDES = -I./loader -I./peripheral
TEST_INCLUDES = $(INCLUDES) -I./cpu

CXXFLAGS = -std=c++11 -g -Wall

LDFLAGS = -g
LDLIBS  =

TARGET = 6502.a
TEST_TARGET = test-6502
COMMON = $(wildcard *.cpp peripheral/*.cpp)
LOADER = $(wildcard loader/*.cpp)

OBJECTS = $(COMMON:.cpp=.o)
LOADER_OBJECTS = $(LOADER:.cpp=.o)
TEST_OBJECTS = $(OBJECTS) $(LOADER_OBJECTS) cpu/cpu_test.cpp
HEADERS = $(wildcard *.h)
TEST_HEADERS = $(HEADERS) $(wildcard loader/*.h peripheral/*.h cpu/*.h)

.PHONY: lib
lib: $(TARGET)

$(TARGET): $(OBJECTS)
	ar rvs $(TARGET) $(OBJECTS)

$(OBJECTS): $(HEADERS)

.PHONY: test
test: $(TEST_TARGET)
	python test/runner.py test/asm

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) -o $(TEST_TARGET) $(TEST_OBJECTS) $(LDFLAGS) $(LDLIBS)

$(TEST_OBJECTS): $(TEST_HEADERS)

.PHONY: clean
clean:
	rm -f *~ *.o core $(TARGET) $(TEST_TARGET)

.PHONY: all
all: clean lib
