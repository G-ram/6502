CXX = clang++

INCLUDES = -I./loader -I./peripheral
TEST_INCLUDES = $(INCLUDES) -I./cpu

CXXFLAGS = -std=c++11 -g -Wall #-I/usr/local/include/SDL2 -D_THREAD_SAFE

LDFLAGS = -g
LDLIBS  = #-L/usr/local/lib -lSDL2

TARGET = 6502.a
TEST_TARGET = test-6502
SRC = $(wildcard *.cpp loader/*.cpp peripheral/*.cpp)
OBJECTS = $(SRC:.cpp=.o)
TEST_OBJECTS = $(OBJECTS) cpu/cpu_test.cpp
HEADERS = $(wildcard *.h loader/*.h peripheral/*.h)
TEST_HEADERS = $(HEADERS) $(wildcard cpu/*.h)

.PHONY: lib
lib: $(TARGET)

$(TARGET): $(OBJECTS)
	ar rvs $(TARGET) $(OBJECTS)

$(OBJECTS): $(HEADERS)

.PHONY: test
test: $(TEST_TARGET)
	python test/runner.py test/asm

$(TEST_TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) -o $(TEST_TARGET) $(TEST_OBJECTS) $(LDFLAGS) $(LDLIBS)

$(TEST_OBJECTS): $(TEST_HEADERS)

.PHONY: clean
clean:
	rm -f *~ *.o core $(OBJECTS) $(TARGET) $(TEST_TARGET)

.PHONY: all
all: clean default
