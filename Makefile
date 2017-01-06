CXX = clang++

INCLUDES = -I./loader -I./peripheral

CXXFLAGS = -std=c++11 -g -Wall -I/usr/local/include/SDL2 -D_THREAD_SAFE

LDFLAGS = -g
LDLIBS  = -L/usr/local/lib -lSDL2

SRC = $(wildcard *.cpp loader/*.cpp peripheral/*.cpp)
COMMON=$(patsubst cpu%,,${SRC})
TARGET = 6502
CPU = cpu.cpp
OBJECTS = $(COMMON:.cpp=.o) $(CPU)
HEADERS = $(wildcard *.h)

.PHONY: default
default: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJECTS) $(LDFLAGS) $(LDLIBS)

$(OBJECTS): $(HEADERS)

.PHONY: test
test: $(TEST) $(TARGET)
	python test/runner.py test/asm

.PHONY: clean
clean:
	rm -f *~ a.out core $(OBJECTS) $(TARGET)

.PHONY: all
all: clean default
